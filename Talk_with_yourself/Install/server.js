const fs = require("fs");
const http = require("http");
const path = require("path");
const { execFile, exec } = require("child_process");
const { URL } = require("url");
const { distillPersona, generateReply, buildPersonaExportFiles, sanitizeText } = require("./lib/distiller");
const { createZip } = require("./lib/zip");

const ROOT = __dirname;
const DATA_DIR = path.join(ROOT, "data");
const PERSONA_DIR = path.join(DATA_DIR, "personas");
const UPLOAD_DIR = path.join(DATA_DIR, "uploads");
const PUBLIC_DIR = path.join(ROOT, "public");
const PORT = Number(process.env.PORT || 8787);
const TEXT_IMPORT_EXTS = new Set([".txt", ".csv", ".json", ".md"]);
const WECHAT_DB_EXTS = new Set([".db", ".sqlite", ".sqlite3"]);

ensureDir(DATA_DIR);
ensureDir(PERSONA_DIR);
ensureDir(UPLOAD_DIR);

const server = http.createServer(async (req, res) => {
  try {
    const url = new URL(req.url, `http://${req.headers.host || "localhost"}`);

    if (req.method === "GET" && url.pathname === "/api/status") {
      const model = loadModelConfig();
      return sendJson(res, 200, {
        ok: true,
        app: "Talk with yourself",
        version: "0.1.0",
        port: PORT,
        dataDir: DATA_DIR,
        model: {
          provider: model.provider,
          baseUrl: model.baseUrl,
          model: model.model,
          hasKey: model.hasKey,
          enabled: model.enabled
        }
      });
    }

    if (req.method === "POST" && url.pathname === "/api/ingest") {
      return handleIngest(req, res);
    }

    if (req.method === "GET" && url.pathname === "/api/personas") {
      return sendJson(res, 200, listPersonas());
    }

    const personaMatch = url.pathname.match(/^\/api\/personas\/([^/]+)$/);
    if (req.method === "GET" && personaMatch) {
      const persona = readPersona(personaMatch[1]);
      if (!persona) return sendJson(res, 404, { error: "Persona not found" });
      return sendJson(res, 200, persona);
    }

    const exportMatch = url.pathname.match(/^\/api\/personas\/([^/]+)\/export$/);
    if (req.method === "GET" && exportMatch) {
      const persona = readPersona(exportMatch[1]);
      if (!persona) return sendJson(res, 404, { error: "Persona not found" });
      const zip = createZip(buildPersonaExportFiles(persona));
      const fileName = `${asciiFileName(persona.displayName)}-${persona.id}.zip`;
      res.writeHead(200, {
        "Content-Type": "application/zip",
        "Content-Disposition": `attachment; filename="${fileName}"`,
        "Content-Length": zip.length
      });
      return res.end(zip);
    }

    if (req.method === "POST" && url.pathname === "/api/chat") {
      return handleChat(req, res);
    }

    if (req.method === "GET" && url.pathname === "/api/device/wechat-scan") {
      return sendJson(res, 200, scanWechatDevice());
    }

    if (req.method === "POST" && url.pathname === "/api/device/import-local") {
      return handleImportLocal(req, res);
    }

    if (req.method === "GET" && url.pathname === "/api/wechat/sessions") {
      return handleWechatSessions(req, res);
    }

    if (req.method === "POST" && url.pathname === "/api/wechat/init") {
      return handleWechatInit(req, res);
    }

    if (req.method === "POST" && url.pathname === "/api/wechat/import-session") {
      return handleWechatImportSession(req, res);
    }

    if (req.method === "POST" && url.pathname === "/api/personas/delete") {
      return handleDelete(req, res);
    }

    return serveStatic(req, res, url.pathname);
  } catch (error) {
    console.error(error);
    return sendJson(res, 500, {
      error: "Internal server error",
      message: error.message
    });
  }
});

server.listen(PORT, () => {
  console.log(`Talk with yourself is running at http://localhost:${PORT}`);
  console.log(`Data directory: ${DATA_DIR}`);
});

async function handleIngest(req, res) {
  const contentType = req.headers["content-type"] || "";
  if (!contentType.includes("multipart/form-data")) {
    return sendJson(res, 415, { error: "Please upload multipart/form-data" });
  }

  const body = await readRequestBuffer(req, 30 * 1024 * 1024);
  const form = parseMultipart(body, contentType);
  const rawBuffer = form.files.chatFile?.content || Buffer.from(form.fields.rawText || "", "utf8");
  const rawText = decodeText(rawBuffer);

  if (!rawText.trim()) {
    return sendJson(res, 400, { error: "聊天记录为空，请上传 txt/csv 文本文件。" });
  }

  const consent = {
    allowHumanReview: form.fields.allowHumanReview === "true" || form.fields.allowHumanReview === "on",
    allowTraining: form.fields.allowTraining === "true" || form.fields.allowTraining === "on",
    uploadedByOwner: form.fields.uploadedByOwner !== "false",
    acceptedAt: new Date().toISOString(),
    policyVersion: "local-mvp-v1"
  };

  const persona = distillPersona({
    displayName: form.fields.displayName || "我的数字分身",
    ownerEmail: form.fields.ownerEmail || "",
    notes: form.fields.notes || "",
    rawText,
    consent
  });

  const personaPath = path.join(PERSONA_DIR, `${persona.id}.json`);
  const uploadBase = path.join(UPLOAD_DIR, persona.id);
  ensureDir(uploadBase);

  const originalFileName = form.files.chatFile?.filename || "raw.txt";
  fs.writeFileSync(path.join(uploadBase, safeFileName(originalFileName)), rawText, "utf8");
  fs.writeFileSync(path.join(uploadBase, "sanitized.txt"), sanitizeText(rawText), "utf8");
  fs.writeFileSync(path.join(uploadBase, "consent.json"), JSON.stringify(consent, null, 2), "utf8");
  fs.writeFileSync(personaPath, JSON.stringify(persona, null, 2), "utf8");

  return sendJson(res, 201, {
    ok: true,
    persona,
    saved: {
      personaPath,
      uploadDir: uploadBase
    }
  });
}

async function handleChat(req, res) {
  const body = await readJson(req);
  const persona = readPersona(body.personaId);
  if (!persona) return sendJson(res, 404, { error: "Persona not found" });

  const modelConfig = loadModelConfig();
  let mode = "local-rule-fallback";
  let note = "当前为本地规则回复；后续可接入 cc 环境中的云端模型 API。";
  let reply = "";

  if (modelConfig.enabled) {
    try {
      reply = await callAnthropicCompatibleModel(modelConfig, persona, body.message || "");
      mode = "cc-compatible-model";
      note = `已通过 ${modelConfig.baseUrl} 调用云端模型。`;
    } catch (error) {
      console.warn(`Model call failed, using local fallback: ${error.message}`);
      reply = generateReply(persona, body.message || "");
      note = `云端模型调用失败，已使用本地规则 fallback：${error.message}`;
    }
  } else {
    reply = generateReply(persona, body.message || "");
  }

  appendChatLog(persona.id, {
    at: new Date().toISOString(),
    user: String(body.message || ""),
    assistant: reply,
    mode
  });

  return sendJson(res, 200, {
    reply,
    mode,
    note
  });
}

async function handleDelete(req, res) {
  const body = await readJson(req);
  const id = String(body.personaId || "");
  if (!id || !/^[a-zA-Z0-9_-]+$/.test(id)) {
    return sendJson(res, 400, { error: "Invalid personaId" });
  }

  const personaPath = path.join(PERSONA_DIR, `${id}.json`);
  const uploadBase = path.join(UPLOAD_DIR, id);
  const chatLog = path.join(DATA_DIR, "chatlogs", `${id}.jsonl`);
  safeRemoveFile(personaPath);
  safeRemoveDir(uploadBase);
  safeRemoveFile(chatLog);
  return sendJson(res, 200, { ok: true });
}

async function handleImportLocal(req, res) {
  const body = await readJson(req);
  const filePath = String(body.filePath || "");
  const resolved = path.resolve(filePath);
  const allowedRoots = getWechatSearchRoots().map((item) => path.resolve(item));

  if (!allowedRoots.some((root) => resolved === root || resolved.startsWith(`${root}${path.sep}`))) {
    return sendJson(res, 403, { error: "只能导入扫描到的微信相关目录中的文件。" });
  }

  if (!fs.existsSync(resolved) || !fs.statSync(resolved).isFile()) {
    return sendJson(res, 404, { error: "文件不存在。" });
  }

  const ext = path.extname(resolved).toLowerCase();
  if (!TEXT_IMPORT_EXTS.has(ext)) {
    return sendJson(res, 415, { error: "当前只能直接导入 txt/csv/json/md 文本文件；微信数据库需要先导出或解密。" });
  }

  const rawBuffer = fs.readFileSync(resolved);
  const rawText = decodeText(rawBuffer);
  if (!rawText.trim()) {
    return sendJson(res, 400, { error: "文件为空。" });
  }

  const consent = {
    allowHumanReview: body.allowHumanReview === true,
    allowTraining: body.allowTraining === true,
    uploadedByOwner: true,
    acceptedAt: new Date().toISOString(),
    policyVersion: "local-device-import-v1"
  };

  const persona = distillPersona({
    displayName: body.displayName || path.basename(resolved, ext) || "本机微信分身",
    ownerEmail: body.ownerEmail || "local-device",
    notes: body.notes || `从本机文件导入：${resolved}`,
    rawText,
    consent
  });

  const personaPath = path.join(PERSONA_DIR, `${persona.id}.json`);
  const uploadBase = path.join(UPLOAD_DIR, persona.id);
  ensureDir(uploadBase);
  fs.writeFileSync(path.join(uploadBase, safeFileName(path.basename(resolved))), rawText, "utf8");
  fs.writeFileSync(path.join(uploadBase, "sanitized.txt"), sanitizeText(rawText), "utf8");
  fs.writeFileSync(path.join(uploadBase, "consent.json"), JSON.stringify(consent, null, 2), "utf8");
  fs.writeFileSync(path.join(uploadBase, "source.json"), JSON.stringify({ filePath: resolved }, null, 2), "utf8");
  fs.writeFileSync(personaPath, JSON.stringify(persona, null, 2), "utf8");

  return sendJson(res, 201, {
    ok: true,
    persona,
    saved: {
      personaPath,
      uploadDir: uploadBase
    }
  });
}

async function handleWechatSessions(req, res) {
  try {
    const output = await runWxCli(["sessions", "-n", "50", "--json"], 60_000);
    const parsed = parseJsonFromOutput(output.stdout);
    const sessions = Array.isArray(parsed?.sessions) ? parsed.sessions : Array.isArray(parsed) ? parsed : [];
    return sendJson(res, 200, {
      ok: true,
      sessions: sessions.map((item) => ({
        chat: item.chat || item.name || item.username || "",
        username: item.username || "",
        chatType: item.chat_type || "",
        isGroup: Boolean(item.is_group),
        summary: item.summary || "",
        time: item.time || "",
        unread: item.unread || 0,
        lastMsgType: item.last_msg_type || ""
      })).filter((item) => item.chat)
    });
  } catch (error) {
    return sendJson(res, 500, { error: `读取微信会话失败：${error.message}` });
  }
}

async function handleWechatInit(req, res) {
  try {
    const output = await runWxCli(["init", "--force"], 180_000);
    return sendJson(res, 200, {
      ok: true,
      message: "微信数据库初始化完成。",
      output: redactWxCliOutput(`${output.stdout}\n${output.stderr}`)
    });
  } catch (error) {
    return sendJson(res, 500, { error: `微信初始化失败：${redactWxCliOutput(error.message)}` });
  }
}

async function handleWechatImportSession(req, res) {
  try {
    const body = await readJson(req);
    const chat = String(body.username || body.chat || "").trim();
    const displayChat = String(body.chat || body.username || "").trim();
    if (!chat) return sendJson(res, 400, { error: "缺少会话名称。" });

    const limit = Math.max(5, Math.min(Number(body.limit || 1000), 5000));
    const exportDir = path.join(DATA_DIR, "wechat-exports");
    ensureDir(exportDir);
    const outputFile = path.join(exportDir, `${safeFileName(displayChat || chat)}-${Date.now()}.txt`);
    const args = ["export", chat, "-f", "txt", "-n", String(limit), "-o", outputFile];
    if (body.since) args.push("--since", String(body.since));
    if (body.until) args.push("--until", String(body.until));

    await runWxCli(args, 180_000);

    if (!fs.existsSync(outputFile)) {
      return sendJson(res, 500, { error: "微信导出失败：没有生成输出文件。" });
    }

    const rawText = fs.readFileSync(outputFile, "utf8");
    if (!rawText.trim()) {
      return sendJson(res, 400, { error: "导出的聊天记录为空。" });
    }

    const consent = {
      allowHumanReview: body.allowHumanReview === true,
      allowTraining: body.allowTraining === true,
      uploadedByOwner: true,
      acceptedAt: new Date().toISOString(),
      policyVersion: "local-wechat-db-import-v1"
    };

    const persona = distillPersona({
      displayName: body.displayName || `${displayChat || chat} 的聊天分身`,
      ownerEmail: body.ownerEmail || "local-wechat",
      notes: body.notes || `从本机微信数据库导入会话：${displayChat || chat}`,
      rawText,
      consent
    });

    const personaPath = path.join(PERSONA_DIR, `${persona.id}.json`);
    const uploadBase = path.join(UPLOAD_DIR, persona.id);
    ensureDir(uploadBase);
    fs.copyFileSync(outputFile, path.join(uploadBase, "wechat-session.txt"));
    fs.writeFileSync(path.join(uploadBase, "sanitized.txt"), sanitizeText(rawText), "utf8");
    fs.writeFileSync(path.join(uploadBase, "consent.json"), JSON.stringify(consent, null, 2), "utf8");
    fs.writeFileSync(path.join(uploadBase, "source.json"), JSON.stringify({
      source: "wx-cli",
      chat: displayChat || chat,
      username: chat,
      limit,
      exportedAt: new Date().toISOString()
    }, null, 2), "utf8");
    fs.writeFileSync(personaPath, JSON.stringify(persona, null, 2), "utf8");

    return sendJson(res, 201, {
      ok: true,
      persona,
      saved: {
        personaPath,
        uploadDir: uploadBase,
        exportFile: outputFile
      }
    });
  } catch (error) {
    return sendJson(res, 500, { error: `微信会话导入失败：${error.message}` });
  }
}

function scanWechatDevice() {
  const roots = getWechatSearchRoots().filter((root) => fs.existsSync(root));
  const files = [];
  const databaseFiles = [];
  const directories = [];

  for (const root of roots) {
    walkLimited(root, {
      maxDepth: 5,
      maxFiles: 4000,
      onDirectory: (dirPath) => {
        if (/WeChat Files|FileStorage|Msg|Backup|微信|WeChat/i.test(dirPath)) {
          directories.push(describePath(dirPath, "directory"));
        }
      },
      onFile: (filePath, stat) => {
        const ext = path.extname(filePath).toLowerCase();
        const name = path.basename(filePath);
        const textLooksRelevant = looksLikeChatExport(filePath);
        const dbLooksRelevant = looksLikeWechatMessageDatabase(filePath);
        if (TEXT_IMPORT_EXTS.has(ext) && textLooksRelevant) {
          files.push({
            ...describePath(filePath, "text"),
            size: stat.size,
            importable: stat.size > 0 && stat.size <= 30 * 1024 * 1024,
            reason: stat.size > 30 * 1024 * 1024 ? "文件超过 30MB，建议先拆分。" : "可直接导入"
          });
        } else if (dbLooksRelevant) {
          databaseFiles.push({
            ...describePath(filePath, "database"),
            size: stat.size,
            importable: false,
            reason: "微信数据库通常是加密数据，当前 MVP 只识别位置，不直接解密。"
          });
        }
      }
    });
  }

  const uniqueFiles = uniqueByPath(files)
    .sort((a, b) => Number(b.importable) - Number(a.importable) || b.modifiedAt.localeCompare(a.modifiedAt))
    .slice(0, 200);
  const uniqueDatabaseFiles = uniqueByPath(databaseFiles);

  return {
    roots,
    directories: uniqueByPath(directories).slice(0, 100),
    files: uniqueFiles,
    summary: {
      rootCount: roots.length,
      fileCount: uniqueFiles.length + uniqueDatabaseFiles.length,
      importableCount: uniqueFiles.filter((item) => item.importable).length,
      databaseCount: uniqueDatabaseFiles.length
    }
  };
}

function getWechatSearchRoots() {
  const home = process.env.USERPROFILE || "";
  const documents = process.env.USERPROFILE ? path.join(process.env.USERPROFILE, "Documents") : "";
  const appData = process.env.APPDATA || "";
  const localAppData = process.env.LOCALAPPDATA || "";
  const candidates = [
    path.join(documents, "WeChat Files"),
    path.join(home, "Documents", "WeChat Files"),
    path.join(home, "WeChat Files"),
    path.join(appData, "Tencent", "WeChat"),
    path.join(appData, "Tencent", "xwechat"),
    path.join(localAppData, "Tencent", "WeChat"),
    path.join(localAppData, "Tencent", "xwechat")
  ];
  const packageRoot = path.join(localAppData, "Packages");
  try {
    for (const entry of fs.readdirSync(packageRoot, { withFileTypes: true })) {
      if (entry.isDirectory() && /wechat|weixin|tencent/i.test(entry.name)) {
        candidates.push(path.join(packageRoot, entry.name));
      }
    }
  } catch {
    // Ignore missing Windows app package root.
  }

  const xwechatConfig = path.join(appData, "Tencent", "xwechat", "config");
  try {
    for (const entry of fs.readdirSync(xwechatConfig, { withFileTypes: true })) {
      if (!entry.isFile() || !entry.name.toLowerCase().endsWith(".ini")) continue;
      const content = fs.readFileSync(path.join(xwechatConfig, entry.name), "utf8").trim();
      if (/^[a-zA-Z]:\\/.test(content)) candidates.push(content);
    }
  } catch {
    // Ignore missing xwechat config.
  }

  return [...new Set(candidates.filter(Boolean))];
}

function looksLikeChatExport(filePath) {
  const base = path.basename(filePath).toLowerCase();
  const normalized = filePath.toLowerCase();
  const ext = path.extname(base);
  if (!TEXT_IMPORT_EXTS.has(ext)) return false;
  if (/cloud_account|charset|sohu|md5|file_component|config|manifest|package|emoji|lottie|locale|license|readme/i.test(base)) {
    return false;
  }
  return /聊天记录|聊天|chatlog|chat[-_ ]?record|message[-_ ]?export|messages?|msg[-_ ]?export|record[-_ ]?export|wechat[-_ ]?export|微信[-_ ]?导出/i.test(base)
    || /\\(chat|message|messages|msg|record|records|export|聊天记录|聊天)\\/i.test(normalized.replace(/\//g, "\\"));
}

function looksLikeWechatMessageDatabase(filePath) {
  const base = path.basename(filePath).toLowerCase();
  const normalized = filePath.toLowerCase();
  const ext = path.extname(base);
  if (!WECHAT_DB_EXTS.has(ext)) return false;
  if (!/wechat|weixin|xwechat|微信|tencent/i.test(normalized)) return false;
  if (/heavy_ad|cache|cookie|quota|media|appindex|settings|store|installed|telemetry|webview|browser|network/i.test(base)) {
    return false;
  }
  return /^msg\d*\.db$/i.test(base)
    || /^micromsg\.db$/i.test(base)
    || /^message.*\.db$/i.test(base)
    || /^chat.*\.db$/i.test(base)
    || /\\(msg|message|messages|chat|micromsg|db_storage|dbstorage)\\/i.test(normalized.replace(/\//g, "\\"));
}

function walkLimited(root, options) {
  let seenFiles = 0;
  const ignored = /node_modules|\.git|Cache|Code Cache|GPUCache|IndexedDB|Service Worker/i;

  function walk(current, depth) {
    if (seenFiles >= options.maxFiles || depth > options.maxDepth || ignored.test(current)) return;
    let entries = [];
    try {
      entries = fs.readdirSync(current, { withFileTypes: true });
    } catch {
      return;
    }

    if (options.onDirectory) options.onDirectory(current);

    for (const entry of entries) {
      if (seenFiles >= options.maxFiles) return;
      const next = path.join(current, entry.name);
      try {
        if (entry.isDirectory()) {
          walk(next, depth + 1);
        } else if (entry.isFile()) {
          seenFiles += 1;
          options.onFile(next, fs.statSync(next));
        }
      } catch {
        // Ignore unreadable files in a local discovery scan.
      }
    }
  }

  walk(root, 0);
}

function describePath(filePath, kind) {
  let stat = null;
  try {
    stat = fs.statSync(filePath);
  } catch {
    stat = null;
  }
  return {
    kind,
    path: filePath,
    name: path.basename(filePath),
    modifiedAt: stat ? stat.mtime.toISOString() : ""
  };
}

function uniqueByPath(items) {
  const seen = new Set();
  const output = [];
  for (const item of items) {
    const key = item.path.toLowerCase();
    if (seen.has(key)) continue;
    seen.add(key);
    output.push(item);
  }
  return output;
}

function runWxCli(args, timeoutMs) {
  return new Promise((resolve, reject) => {
    const maxBuffer = 20 * 1024 * 1024;
    if (process.platform === "win32") {
      const command = ["npx", "wx", ...args].map(quoteCmdArg).join(" ");
      exec(command, {
        cwd: ROOT,
        windowsHide: true,
        timeout: timeoutMs,
        maxBuffer
      }, (error, stdout, stderr) => {
        if (error) {
          reject(new Error(`${stderr || stdout || error.message}`.trim()));
          return;
        }
        resolve({ stdout, stderr });
      });
      return;
    }

    try {
      const child = execFile("npx", ["wx", ...args], {
        cwd: ROOT,
        windowsHide: true,
        timeout: timeoutMs,
        maxBuffer
      }, (error, stdout, stderr) => {
        if (error) {
          reject(new Error(`${stderr || stdout || error.message}`.trim()));
          return;
        }
        resolve({ stdout, stderr });
      });
      child.on("error", reject);
    } catch (error) {
      reject(error);
    }
  });
}

function quoteCmdArg(value) {
  const text = String(value);
  if (/^[A-Za-z0-9_./:=@-]+$/.test(text)) return text;
  return `"${text.replace(/"/g, '\\"')}"`;
}

function parseJsonFromOutput(text) {
  const trimmed = String(text || "").trim();
  if (!trimmed) return null;
  try {
    return JSON.parse(trimmed);
  } catch {
    const start = Math.min(
      ...[trimmed.indexOf("{"), trimmed.indexOf("[")].filter((index) => index >= 0)
    );
    if (!Number.isFinite(start)) throw new Error("wx-cli 未返回 JSON。");
    return JSON.parse(trimmed.slice(start));
  }
}

function redactWxCliOutput(text) {
  return String(text || "")
    .replace(/[a-fA-F0-9]{64}/g, "<key-redacted>")
    .replace(/("key"\s*:\s*")[^"]+(")/gi, "$1<key-redacted>$2")
    .trim();
}

function listPersonas() {
  ensureDir(PERSONA_DIR);
  return fs.readdirSync(PERSONA_DIR)
    .filter((name) => name.endsWith(".json"))
    .map((name) => readJsonFile(path.join(PERSONA_DIR, name)))
    .filter(Boolean)
    .sort((a, b) => String(b.createdAt).localeCompare(String(a.createdAt)))
    .map((persona) => ({
      id: persona.id,
      displayName: persona.displayName,
      createdAt: persona.createdAt,
      messageCount: persona.source?.messageCount || 0,
      primarySpeaker: persona.source?.primarySpeaker || "",
      allowTraining: Boolean(persona.consent?.allowTraining),
      allowHumanReview: Boolean(persona.consent?.allowHumanReview),
      tone: persona.style?.tone || [],
      keywords: (persona.style?.keywords || []).slice(0, 8),
      catchphrases: (persona.style?.catchphrases || []).slice(0, 5)
    }));
}

function readPersona(id) {
  if (!id || !/^[a-zA-Z0-9_-]+$/.test(id)) return null;
  return readJsonFile(path.join(PERSONA_DIR, `${id}.json`));
}

function appendChatLog(personaId, item) {
  const dir = path.join(DATA_DIR, "chatlogs");
  ensureDir(dir);
  fs.appendFileSync(path.join(dir, `${personaId}.jsonl`), `${JSON.stringify(item)}\n`, "utf8");
}

function serveStatic(req, res, pathname) {
  const requested = pathname === "/" ? "/index.html" : pathname;
  const normalized = path.normalize(decodeURIComponent(requested)).replace(/^(\.\.[/\\])+/, "");
  const filePath = path.join(PUBLIC_DIR, normalized);

  if (!filePath.startsWith(PUBLIC_DIR)) {
    return sendText(res, 403, "Forbidden");
  }

  if (!fs.existsSync(filePath) || !fs.statSync(filePath).isFile()) {
    const fallback = path.join(PUBLIC_DIR, "index.html");
    if (fs.existsSync(fallback)) {
      return sendFile(res, fallback);
    }
    return sendText(res, 404, "Not found");
  }

  return sendFile(res, filePath);
}

function sendFile(res, filePath) {
  const ext = path.extname(filePath).toLowerCase();
  const types = {
    ".html": "text/html; charset=utf-8",
    ".css": "text/css; charset=utf-8",
    ".js": "application/javascript; charset=utf-8",
    ".json": "application/json; charset=utf-8",
    ".svg": "image/svg+xml",
    ".png": "image/png",
    ".ico": "image/x-icon"
  };
  const content = fs.readFileSync(filePath);
  res.writeHead(200, {
    "Content-Type": types[ext] || "application/octet-stream",
    "Content-Length": content.length,
    "Cache-Control": "no-store, max-age=0"
  });
  res.end(content);
}

function sendJson(res, status, payload) {
  const content = Buffer.from(JSON.stringify(payload, null, 2), "utf8");
  res.writeHead(status, {
    "Content-Type": "application/json; charset=utf-8",
    "Content-Length": content.length,
    "Cache-Control": "no-store, max-age=0"
  });
  res.end(content);
}

function sendText(res, status, text) {
  const content = Buffer.from(text, "utf8");
  res.writeHead(status, {
    "Content-Type": "text/plain; charset=utf-8",
    "Content-Length": content.length
  });
  res.end(content);
}

function readJsonFile(filePath) {
  try {
    return JSON.parse(fs.readFileSync(filePath, "utf8"));
  } catch {
    return null;
  }
}

function readJson(req) {
  return readRequestBuffer(req, 5 * 1024 * 1024).then((buffer) => {
    if (!buffer.length) return {};
    return JSON.parse(buffer.toString("utf8"));
  });
}

function readRequestBuffer(req, maxBytes) {
  return new Promise((resolve, reject) => {
    const chunks = [];
    let size = 0;
    req.on("data", (chunk) => {
      size += chunk.length;
      if (size > maxBytes) {
        reject(new Error("Request body too large"));
        req.destroy();
        return;
      }
      chunks.push(chunk);
    });
    req.on("end", () => resolve(Buffer.concat(chunks)));
    req.on("error", reject);
  });
}

function parseMultipart(buffer, contentType) {
  const boundaryMatch = contentType.match(/boundary=(?:"([^"]+)"|([^;]+))/i);
  if (!boundaryMatch) throw new Error("Missing multipart boundary");
  const boundary = Buffer.from(`--${boundaryMatch[1] || boundaryMatch[2]}`);
  const result = { fields: {}, files: {} };

  let start = buffer.indexOf(boundary);
  while (start !== -1) {
    start += boundary.length;
    if (buffer[start] === 45 && buffer[start + 1] === 45) break;
    if (buffer[start] === 13 && buffer[start + 1] === 10) start += 2;

    const headerEnd = buffer.indexOf(Buffer.from("\r\n\r\n"), start);
    if (headerEnd === -1) break;
    const headerText = buffer.slice(start, headerEnd).toString("utf8");
    const contentStart = headerEnd + 4;
    let next = buffer.indexOf(boundary, contentStart);
    if (next === -1) break;
    let contentEnd = next;
    if (buffer[contentEnd - 2] === 13 && buffer[contentEnd - 1] === 10) {
      contentEnd -= 2;
    }

    const dispositionLine = headerText.split(/\r?\n/).find((line) => /^content-disposition:/i.test(line)) || "";
    const nameMatch = dispositionLine.match(/\bname="([^"]+)"/i);
    if (nameMatch) {
      const name = nameMatch[1];
      const filename = (dispositionLine.match(/\bfilename="([^"]*)"/i) || [])[1];
      const content = buffer.slice(contentStart, contentEnd);
      if (filename !== undefined && filename !== "") {
        result.files[name] = {
          filename,
          content,
          contentType: (headerText.match(/content-type:\s*([^\n\r]+)/i) || [])[1] || "application/octet-stream"
        };
      } else {
        result.fields[name] = content.toString("utf8");
      }
    }

    start = next;
  }

  return result;
}

function decodeText(buffer) {
  if (buffer.length >= 3 && buffer[0] === 0xef && buffer[1] === 0xbb && buffer[2] === 0xbf) {
    return buffer.slice(3).toString("utf8");
  }
  if (buffer.length >= 2 && buffer[0] === 0xff && buffer[1] === 0xfe) {
    return buffer.slice(2).toString("utf16le");
  }
  if (buffer.length >= 2 && buffer[0] === 0xfe && buffer[1] === 0xff) {
    return swapUtf16Be(buffer.slice(2)).toString("utf16le");
  }
  return buffer.toString("utf8");
}

function swapUtf16Be(buffer) {
  const output = Buffer.from(buffer);
  for (let i = 0; i + 1 < output.length; i += 2) {
    const a = output[i];
    output[i] = output[i + 1];
    output[i + 1] = a;
  }
  return output;
}

function loadModelConfig() {
  const baseUrl = process.env.TALK_MODEL_BASE_URL || process.env.ANTHROPIC_BASE_URL || "";
  const model = process.env.TALK_MODEL || process.env.ANTHROPIC_MODEL || process.env.CLAUDE_MODEL || "claude-3-5-sonnet-latest";
  const apiKey = process.env.TALK_MODEL_API_KEY || process.env.ANTHROPIC_API_KEY || process.env.OPENAI_API_KEY || process.env.GEMINI_API_KEY || "";
  const hasKey = Boolean(apiKey);
  return {
    provider: baseUrl ? "cc-compatible-env" : "local-rule-fallback",
    baseUrl,
    model,
    apiKey,
    hasKey,
    enabled: Boolean(baseUrl && hasKey)
  };
}

async function callAnthropicCompatibleModel(config, persona, userMessage) {
  const endpoint = `${config.baseUrl.replace(/\/+$/, "")}/v1/messages`;
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), Number(process.env.TALK_MODEL_TIMEOUT_MS || 25000));

  try {
    const response = await fetch(endpoint, {
      method: "POST",
      signal: controller.signal,
      headers: {
        "Content-Type": "application/json",
        "x-api-key": config.apiKey,
        "anthropic-version": process.env.ANTHROPIC_VERSION || "2023-06-01"
      },
      body: JSON.stringify({
        model: config.model,
        max_tokens: Number(process.env.TALK_MODEL_MAX_TOKENS || 700),
        temperature: Number(process.env.TALK_MODEL_TEMPERATURE || 0.7),
        system: persona.systemPrompt,
        messages: [
          {
            role: "user",
            content: String(userMessage || "")
          }
        ]
      })
    });

    const text = await response.text();
    if (!response.ok) {
      throw new Error(`${response.status} ${response.statusText}: ${text.slice(0, 300)}`);
    }

    const payload = JSON.parse(text);
    const output = extractAnthropicText(payload);
    if (!output) throw new Error("模型返回为空");
    return output;
  } finally {
    clearTimeout(timeout);
  }
}

function extractAnthropicText(payload) {
  if (typeof payload?.completion === "string") return payload.completion.trim();
  if (typeof payload?.content === "string") return payload.content.trim();
  if (Array.isArray(payload?.content)) {
    return payload.content
      .map((part) => {
        if (typeof part === "string") return part;
        if (part?.type === "text") return part.text || "";
        return "";
      })
      .join("")
      .trim();
  }
  if (typeof payload?.message === "string") return payload.message.trim();
  return "";
}

function ensureDir(dir) {
  fs.mkdirSync(dir, { recursive: true });
}

function safeFileName(name) {
  return String(name || "file")
    .replace(/[<>:"/\\|?*\x00-\x1F]/g, "_")
    .replace(/\s+/g, "_")
    .slice(0, 120) || "file";
}

function asciiFileName(name) {
  const ascii = safeFileName(name)
    .replace(/[^\x20-\x7E]/g, "")
    .replace(/_+/g, "_")
    .replace(/^_+|_+$/g, "");
  return ascii || "persona";
}

function safeRemoveFile(filePath) {
  try {
    if (fs.existsSync(filePath) && fs.statSync(filePath).isFile()) fs.unlinkSync(filePath);
  } catch {
    // Ignore best-effort delete errors in local MVP.
  }
}

function safeRemoveDir(dirPath) {
  try {
    if (fs.existsSync(dirPath) && fs.statSync(dirPath).isDirectory()) {
      fs.rmSync(dirPath, { recursive: true, force: true });
    }
  } catch {
    // Ignore best-effort delete errors in local MVP.
  }
}
