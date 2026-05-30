const state = {
  personas: [],
  selected: null
};

const els = {
  status: document.querySelector("#status"),
  uploadForm: document.querySelector("#uploadForm"),
  refreshBtn: document.querySelector("#refreshBtn"),
  initWechatBtn: document.querySelector("#initWechatBtn"),
  scanWechatBtn: document.querySelector("#scanWechatBtn"),
  scanResult: document.querySelector("#scanResult"),
  wechatSessions: document.querySelector("#wechatSessions"),
  personaList: document.querySelector("#personaList"),
  chatTitle: document.querySelector("#chatTitle"),
  chatSubtitle: document.querySelector("#chatSubtitle"),
  personaMeta: document.querySelector("#personaMeta"),
  chatLog: document.querySelector("#chatLog"),
  chatForm: document.querySelector("#chatForm"),
  chatInput: document.querySelector("#chatInput"),
  exportLink: document.querySelector("#exportLink")
};

init();

function init() {
  loadStatus();
  loadPersonas();
  els.refreshBtn.addEventListener("click", loadPersonas);
  els.initWechatBtn.addEventListener("click", initWechatReader);
  els.scanWechatBtn.addEventListener("click", scanWechatDevice);
  els.uploadForm.addEventListener("submit", submitUpload);
  els.chatForm.addEventListener("submit", sendMessage);
}

async function initWechatReader() {
  els.initWechatBtn.disabled = true;
  els.initWechatBtn.textContent = "初始化中";
  els.scanResult.className = "scan-result empty";
  els.scanResult.textContent = "正在初始化本机微信读取。请确保微信已登录。";

  try {
    const result = await api("/api/wechat/init", { method: "POST" });
    els.scanResult.className = "scan-result empty";
    els.scanResult.textContent = result.output || result.message || "初始化完成。";
    await loadWechatSessions();
  } catch (error) {
    els.scanResult.className = "scan-result empty";
    els.scanResult.textContent = error.message || "初始化失败";
  } finally {
    els.initWechatBtn.disabled = false;
    els.initWechatBtn.textContent = "初始化微信读取";
  }
}

async function loadStatus() {
  try {
    const status = await api("/api/status");
    els.status.textContent = status.model.enabled ? "云端配置已检测" : "本地模式";
    els.status.classList.add("ok");
    if (status.model.enabled) {
      els.chatSubtitle.textContent = `已检测到 cc 兼容模型配置：${status.model.baseUrl}`;
    }
  } catch {
    els.status.textContent = "服务未连接";
    els.status.classList.remove("ok");
  }
}

async function loadPersonas() {
  const personas = await api("/api/personas");
  state.personas = personas;
  renderPersonaList();
  if (!state.selected && personas.length) {
    await selectPersona(personas[0].id);
  }
}

async function submitUpload(event) {
  event.preventDefault();
  const submit = els.uploadForm.querySelector("button[type='submit']");
  submit.disabled = true;
  submit.textContent = "生成中";

  try {
    const file = els.uploadForm.elements.chatFile.files[0];
    const rawText = els.uploadForm.elements.rawText.value.trim();
    if (!file && !rawText) {
      alert("请上传聊天记录文件，或直接粘贴聊天文本。");
      return;
    }

    const form = new FormData(els.uploadForm);
    for (const key of ["uploadedByOwner", "allowHumanReview", "allowTraining"]) {
      form.set(key, els.uploadForm.elements[key].checked ? "true" : "false");
    }
    const result = await api("/api/ingest", {
      method: "POST",
      body: form
    });
    els.uploadForm.reset();
    els.uploadForm.elements.uploadedByOwner.checked = true;
    await loadPersonas();
    await selectPersona(result.persona.id);
    pushBubble("assistant", `已生成“${result.persona.displayName}”。你可以直接测试聊天，也可以导出人格包。`);
  } catch (error) {
    alert(error.message || "上传失败");
  } finally {
    submit.disabled = false;
    submit.textContent = "生成可聊天人格";
  }
}

async function scanWechatDevice() {
  els.scanWechatBtn.disabled = true;
  els.scanWechatBtn.textContent = "扫描中";
  els.scanResult.className = "scan-result empty";
  els.scanResult.textContent = "正在扫描常见微信目录...";

  try {
    const result = await api("/api/device/wechat-scan");
    renderScanResult(result);
    await loadWechatSessions();
  } catch (error) {
    els.scanResult.className = "scan-result empty";
    els.scanResult.textContent = error.message || "扫描失败";
  } finally {
    els.scanWechatBtn.disabled = false;
    els.scanWechatBtn.textContent = "扫描本机";
  }
}

async function loadWechatSessions() {
  els.wechatSessions.className = "scan-result empty";
  els.wechatSessions.textContent = "正在读取已解密的微信会话...";
  try {
    const result = await api("/api/wechat/sessions");
    renderWechatSessions(result.sessions || []);
  } catch (error) {
    els.wechatSessions.className = "scan-result empty";
    els.wechatSessions.textContent = `微信数据库还不能直接读取：${error.message}`;
  }
}

function renderWechatSessions(sessions) {
  if (!sessions.length) {
    els.wechatSessions.className = "scan-result empty";
    els.wechatSessions.textContent = "没有读取到微信会话。请确认微信已登录，然后先点“初始化微信读取”。";
    return;
  }

  els.wechatSessions.className = "scan-result";
  els.wechatSessions.innerHTML = "";
  const title = document.createElement("p");
  title.innerHTML = `<strong>可从微信数据库导入</strong><br>选择一个会话，导出最近消息并生成人格。这里显示的是会话列表，不再展示加密数据库文件。`;
  els.wechatSessions.appendChild(title);

  const list = document.createElement("div");
  list.className = "session-list";
  for (const session of sessions.slice(0, 50)) {
    const item = document.createElement("div");
    item.className = "session-item";

    const info = document.createElement("div");
    const label = session.isGroup ? "群聊" : session.chatType || "私聊";
    info.innerHTML = [
      `<strong>${escapeHtml(session.chat)}</strong>`,
      `<p>${escapeHtml(label)} · ${escapeHtml(session.time || "")} · ${escapeHtml(session.lastMsgType || "")}</p>`,
      `<p>${escapeHtml((session.summary || "").slice(0, 120))}</p>`
    ].join("");
    item.appendChild(info);

    const button = document.createElement("button");
    button.type = "button";
    button.className = "primary";
    button.textContent = "导入";
    button.addEventListener("click", () => importWechatSession(session, button));
    item.appendChild(button);
    list.appendChild(item);
  }
  els.wechatSessions.appendChild(list);
}

async function importWechatSession(session, button) {
  const chat = session.chat || session.username;
  button.disabled = true;
  button.textContent = "导入中";
  try {
    const displayName = els.uploadForm.elements.displayName.value.trim() || `${chat} 的分身`;
    const result = await api("/api/wechat/import-session", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        chat,
        username: session.username,
        displayName,
        ownerEmail: els.uploadForm.elements.ownerEmail.value.trim(),
        notes: els.uploadForm.elements.notes.value.trim(),
        allowHumanReview: els.uploadForm.elements.allowHumanReview.checked,
        allowTraining: els.uploadForm.elements.allowTraining.checked,
        limit: 1000
      })
    });
    await loadPersonas();
    await selectPersona(result.persona.id);
    pushBubble("assistant", `已从微信会话“${chat}”导入并生成“${result.persona.displayName}”。`);
  } catch (error) {
    alert(error.message || "微信会话导入失败");
  } finally {
    button.disabled = false;
    button.textContent = "导入";
  }
}

function renderScanResult(result) {
  const files = (result.files || []).filter((file) => file.importable);
  const summaryData = result.summary || {};
  const databaseCount = Number(summaryData.databaseCount || 0);
  const importableCount = Number(summaryData.importableCount || files.length);
  const roots = (result.roots || []).map((root) => `<br><code>${escapeHtml(root)}</code>`).join("");

  if (!files.length) {
    els.scanResult.className = "scan-result empty";
    els.scanResult.innerHTML = [
      databaseCount
        ? `已识别到 ${databaseCount} 个微信数据库文件。`
        : "没有找到可直接导入的聊天文本文件。",
      "<br>本机微信数据库会通过下面的“可从微信数据库导入”会话列表读取；不要从这里手动点 .db/.wal 文件。",
      "<br>如果下面没有会话，先点“初始化微信读取”，并确认电脑微信处于登录状态。",
      roots ? `<br><br>已识别到这些微信相关目录：${roots}` : ""
    ].join("");
    return;
  }

  els.scanResult.className = "scan-result";
  els.scanResult.innerHTML = "";

  const summary = document.createElement("p");
  summary.textContent = `发现 ${importableCount} 个可直接导入的聊天文本；另识别到 ${databaseCount} 个微信数据库文件，数据库请在下面选择会话导入。`;
  els.scanResult.appendChild(summary);

  for (const file of files.slice(0, 20)) {
    const item = document.createElement("div");
    item.className = "scan-item";

    const title = document.createElement("strong");
    title.textContent = `可导入 · ${file.name}`;
    item.appendChild(title);

    const pathLine = document.createElement("code");
    pathLine.textContent = file.path;
    item.appendChild(pathLine);

    const reason = document.createElement("p");
    reason.textContent = `${file.reason} · ${formatBytes(file.size || 0)}`;
    item.appendChild(reason);

    const actions = document.createElement("div");
    actions.className = "scan-actions";
    const importBtn = document.createElement("button");
    importBtn.className = "primary";
    importBtn.type = "button";
    importBtn.textContent = "一键导入";
    importBtn.addEventListener("click", () => importLocalFile(file.path, importBtn));
    actions.appendChild(importBtn);
    item.appendChild(actions);
    els.scanResult.appendChild(item);
  }
}

async function importLocalFile(filePath, button) {
  const displayName = els.uploadForm.elements.displayName.value.trim() || "本机微信分身";
  button.disabled = true;
  button.textContent = "导入中";
  try {
    const result = await api("/api/device/import-local", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        filePath,
        displayName,
        ownerEmail: els.uploadForm.elements.ownerEmail.value.trim(),
        notes: els.uploadForm.elements.notes.value.trim(),
        allowHumanReview: els.uploadForm.elements.allowHumanReview.checked,
        allowTraining: els.uploadForm.elements.allowTraining.checked
      })
    });
    await loadPersonas();
    await selectPersona(result.persona.id);
    pushBubble("assistant", `已从本机文件导入“${result.persona.displayName}”。`);
  } catch (error) {
    alert(error.message || "导入失败");
  } finally {
    button.disabled = false;
    button.textContent = "一键导入";
  }
}

function renderPersonaList() {
  if (!state.personas.length) {
    els.personaList.className = "persona-list empty";
    els.personaList.textContent = "暂无人格，先上传一份聊天记录。";
    return;
  }

  els.personaList.className = "persona-list";
  els.personaList.innerHTML = "";

  for (const persona of state.personas) {
    const item = document.createElement("button");
    item.type = "button";
    item.className = `persona-item ${state.selected?.id === persona.id ? "active" : ""}`;
    item.addEventListener("click", () => selectPersona(persona.id));

    const title = document.createElement("div");
    title.className = "persona-title";
    title.innerHTML = `<span>${escapeHtml(persona.displayName)}</span><span>${persona.messageCount} 条</span>`;
    item.appendChild(title);

    const meta = document.createElement("p");
    meta.textContent = `${formatDate(persona.createdAt)} · 主说话人：${persona.primarySpeaker || "未知"}`;
    item.appendChild(meta);

    const tags = document.createElement("div");
    tags.className = "tags";
    for (const tag of [
      ...(persona.tone || []).slice(0, 3),
      persona.allowTraining ? "可训练" : "不可训练",
      persona.allowHumanReview ? "可人工质检" : "不看原文"
    ]) {
      const chip = document.createElement("span");
      chip.className = "tag";
      chip.textContent = tag;
      tags.appendChild(chip);
    }
    item.appendChild(tags);
    els.personaList.appendChild(item);
  }
}

async function selectPersona(id) {
  const persona = await api(`/api/personas/${encodeURIComponent(id)}`);
  state.selected = persona;
  renderPersonaList();
  renderSelectedPersona();
}

function renderSelectedPersona() {
  const persona = state.selected;
  if (!persona) return;

  els.chatTitle.textContent = `和 ${persona.displayName} 聊天`;
  els.exportLink.href = `/api/personas/${encodeURIComponent(persona.id)}/export`;
  els.exportLink.classList.remove("hidden");
  els.chatInput.disabled = false;
  els.chatForm.querySelector("button").disabled = false;
  els.chatLog.innerHTML = "";

  const keywords = (persona.style.keywords || []).slice(0, 10).map((item) => item.word).join("、") || "暂无";
  const catchphrases = (persona.style.catchphrases || []).slice(0, 6).map((item) => item.phrase).join("、") || "暂无";
  els.personaMeta.className = "meta";
  els.personaMeta.innerHTML = [
    `<strong>${escapeHtml(persona.displayName)}</strong>`,
    `消息数：${persona.source.messageCount}`,
    `语气：${escapeHtml((persona.style.tone || []).join("、"))}`,
    `口头禅：${escapeHtml(catchphrases)}`,
    `高频词：${escapeHtml(keywords)}`,
    `训练授权：${persona.consent.allowTraining ? "允许" : "不允许"}`
  ].join("<br>");

  pushBubble("assistant", `我是 ${persona.displayName} 的本地数字分身测试版。你可以先问我是谁，或者直接开始聊天。`);
}

async function sendMessage(event) {
  event.preventDefault();
  const message = els.chatInput.value.trim();
  if (!message || !state.selected) return;
  els.chatInput.value = "";
  pushBubble("user", message);

  const button = els.chatForm.querySelector("button");
  button.disabled = true;
  try {
    const result = await api("/api/chat", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        personaId: state.selected.id,
        message
      })
    });
    pushBubble("assistant", result.reply);
  } catch (error) {
    pushBubble("assistant", error.message || "发送失败");
  } finally {
    button.disabled = false;
    els.chatInput.focus();
  }
}

function pushBubble(role, text) {
  const bubble = document.createElement("div");
  bubble.className = `bubble ${role}`;
  bubble.textContent = text;
  els.chatLog.appendChild(bubble);
  els.chatLog.scrollTop = els.chatLog.scrollHeight;
}

async function api(url, options = {}) {
  const response = await fetch(url, options);
  if (!response.ok) {
    let message = `${response.status} ${response.statusText}`;
    try {
      const payload = await response.json();
      message = payload.error || payload.message || message;
    } catch {
      // Keep default HTTP message.
    }
    throw new Error(message);
  }
  return response.json();
}

function formatDate(value) {
  if (!value) return "";
  return new Date(value).toLocaleString("zh-CN", {
    month: "2-digit",
    day: "2-digit",
    hour: "2-digit",
    minute: "2-digit"
  });
}

function formatBytes(size) {
  if (!size) return "0 B";
  const units = ["B", "KB", "MB", "GB"];
  let value = size;
  let index = 0;
  while (value >= 1024 && index < units.length - 1) {
    value /= 1024;
    index += 1;
  }
  return `${value.toFixed(index === 0 ? 0 : 1)} ${units[index]}`;
}

function escapeHtml(value) {
  return String(value || "")
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}
