const vscode = require("vscode");
const { spawn } = require("child_process");

const PROVIDERS = new Set(["qwen", "aider", "cline"]);

function activate(context) {
  const participant = vscode.chat.createChatParticipant(
    "local-agent-bridge.agent",
    handleChatRequest
  );
  participant.followupProvider = {
    provideFollowups() {
      return [
        { prompt: "/qwen 解释当前项目目录结构", label: "Try Qwen" },
        { prompt: "/aider 分析当前项目下一步重构建议", label: "Try Aider" },
        { prompt: "/cline 帮我打开 Cline 并准备这个需求", label: "Send To Cline" }
      ];
    }
  };
  context.subscriptions.push(participant);
}

async function handleChatRequest(request, chatContext, stream, token) {
  const cfg = vscode.workspace.getConfiguration("localAgentBridge");
  const selectedProvider = resolveProvider(request, cfg.get("defaultProvider", "qwen"));

  if (selectedProvider === "help") {
    stream.markdown(buildHelpText(cfg.get("defaultProvider", "qwen")));
    return;
  }

  const prompt = (request.prompt || "").trim();
  if (!prompt) {
    stream.markdown("请先输入问题，再发送给本地 agent。");
    stream.markdown(buildHelpText(cfg.get("defaultProvider", "qwen")));
    return;
  }

  if (selectedProvider === "cline") {
    await routeToCline(prompt, stream);
    return;
  }

  const cwd = workspaceRoot();
  const timeoutMs = cfg.get("commandTimeoutMs", 120000);
  stream.progress(`Running ${selectedProvider}...`);

  const execution = selectedProvider === "qwen"
    ? runQwen(prompt, cfg, cwd, timeoutMs, token)
    : runAider(prompt, cfg, cwd, timeoutMs, token);

  const result = await execution;
  if (result.error) {
    stream.markdown(`运行失败: ${escapeInline(result.error.message || String(result.error))}`);
    return;
  }

  const combined = [result.stdout, result.stderr].filter(Boolean).join("\n").trim();
  if (combined) {
    stream.markdown("```text\n" + clipText(combined, 12000) + "\n```");
  } else {
    stream.markdown("命令已执行，但没有可显示输出。");
  }

  if (result.code !== 0) {
    stream.markdown(`Exit code: \`${result.code}\``);
  }
}

function resolveProvider(request, defaultProvider) {
  if (request.command === "help") {
    return "help";
  }
  if (request.command && PROVIDERS.has(request.command)) {
    return request.command;
  }
  if (PROVIDERS.has(defaultProvider)) {
    return defaultProvider;
  }
  return "qwen";
}

async function routeToCline(prompt, stream) {
  await vscode.env.clipboard.writeText(prompt);
  try {
    await vscode.commands.executeCommand("cline.focusChatInput", false);
  } catch {
    try {
      await vscode.commands.executeCommand("claude-dev.SidebarProvider.focus");
    } catch {
      stream.markdown("无法自动打开 Cline。请先确认 Cline 扩展已启用。");
      return;
    }
  }

  stream.markdown(
    [
      "已切到 Cline，并把你的提示词复制到剪贴板。",
      "在 Cline 输入框里直接粘贴发送即可。"
    ].join("\n\n")
  );
}

function runQwen(prompt, cfg, cwd, timeoutMs, token) {
  const command = cfg.get("qwenCommand", "/home/tian/.npm-global/bin/qwen");
  const args = [
    ...cfg.get("qwenArgs", ["--output-format", "text"]),
    prompt
  ];
  return runProcess(command, args, cwd, timeoutMs, token);
}

function runAider(prompt, cfg, cwd, timeoutMs, token) {
  const command = cfg.get("aiderCommand", "/home/tian/.local/bin/aider");
  const prefix = cfg.get("aiderPromptPrefix", "/ask ");
  const args = [
    ...cfg.get("aiderArgs", [
      "--config",
      "/home/tian/.aider.conf.yml",
      "--no-pretty",
      "--no-stream",
      "--yes-always"
    ]),
    "--message",
    `${prefix}${prompt}`
  ];
  return runProcess(command, args, cwd, timeoutMs, token);
}

function runProcess(command, args, cwd, timeoutMs, token) {
  return new Promise((resolve) => {
    let stdout = "";
    let stderr = "";
    let killedByTimeout = false;

    let child;
    try {
      child = spawn(command, args, {
        cwd,
        env: process.env
      });
    } catch (error) {
      resolve({ code: -1, stdout: "", stderr: "", error });
      return;
    }

    const timer = setTimeout(() => {
      killedByTimeout = true;
      child.kill("SIGKILL");
    }, timeoutMs);

    const onCancel = token.onCancellationRequested(() => {
      child.kill("SIGKILL");
    });

    child.stdout.on("data", (chunk) => {
      stdout += chunk.toString();
    });
    child.stderr.on("data", (chunk) => {
      stderr += chunk.toString();
    });

    child.on("error", (error) => {
      clearTimeout(timer);
      onCancel.dispose();
      resolve({ code: -1, stdout, stderr, error });
    });

    child.on("close", (code) => {
      clearTimeout(timer);
      onCancel.dispose();
      if (killedByTimeout) {
        stderr = [stderr, `Process timed out after ${timeoutMs}ms`].filter(Boolean).join("\n");
      }
      resolve({ code: code ?? -1, stdout, stderr });
    });
  });
}

function workspaceRoot() {
  const folder = vscode.workspace.workspaceFolders && vscode.workspace.workspaceFolders[0];
  return folder ? folder.uri.fsPath : process.cwd();
}

function buildHelpText(defaultProvider) {
  return [
    "本地桥接已启用，可用命令：",
    "- `/qwen 你的问题`：走本地 Qwen CLI",
    "- `/aider 你的问题`：走本地 Aider CLI（默认 `/ask` 安全模式）",
    "- `/cline 你的问题`：打开 Cline 并复制提示词到剪贴板",
    "",
    `当前默认 provider：\`${defaultProvider}\`（不写 slash 命令时使用）`
  ].join("\n");
}

function escapeInline(text) {
  return text.replace(/`/g, "\\`");
}

function clipText(text, maxLength) {
  if (text.length <= maxLength) {
    return text;
  }
  return `${text.slice(0, maxLength)}\n...<truncated>`;
}

function deactivate() {}

module.exports = {
  activate,
  deactivate
};
