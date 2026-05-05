# Local Agent Bridge (VS Code Chat)

This extension adds a chat participant named `@local` that routes requests to local tools:

- `qwen` CLI
- `aider` CLI
- `cline` workflow (open/focus + copy prompt)

## Commands in Chat

- `/qwen <prompt>`: run local qwen one-shot.
- `/aider <prompt>`: run local aider one-shot.
- `/cline <prompt>`: open Cline and copy the prompt to clipboard.
- `/help`: show usage.

If no slash command is used, the extension uses `localAgentBridge.defaultProvider`.

## Settings

- `localAgentBridge.defaultProvider`
- `localAgentBridge.commandTimeoutMs`
- `localAgentBridge.qwenCommand`
- `localAgentBridge.qwenArgs`
- `localAgentBridge.aiderCommand`
- `localAgentBridge.aiderArgs`
- `localAgentBridge.aiderPromptPrefix`

## Quick Start

1. Open this folder in VS Code.
2. Press `F5` to launch Extension Development Host.
3. In the new host window, open Chat and select `@local`.
4. Try `/help`, then `/qwen 介绍一下这个项目`.
