# Codex 与 Claude Code 的代理配置

> 更新日期：2026年05月04日

---

## 1. 三个 AI 工具代理汇总

| 工具 | 模型 | API | 国内直连 | Clash 规则 | 需代理？ |
|------|------|-----|---------|-----------|---------|
| **OpenCode** | DeepSeek V4 Pro | api.deepseek.com | ✅ | DIRECT | ❌ |
| **Claude Code** | DeepSeek（Anthropic 协议） | api.deepseek.com/anthropic | ✅ | DIRECT | ❌ |
| **Codex** | GPT-5.5（OpenAI） | api.openai.com / chatgpt.com | ❌ | 飞猪云 | ✅ |

---

## 2. Shell 层面配置（~/.zshrc）

### Claude Code — 直连

Claude Code 底层用 DeepSeek API，国内可直接访问：

```bash
# ~/.zshrc: claude() 函数清除所有代理变量
claude() {
  command env -u http_proxy -u https_proxy -u all_proxy \
    -u HTTP_PROXY -u HTTPS_PROXY -u ALL_PROXY \
    "$CLAUDE_BIN" "$@"
}
```

### OpenCode — 直连

OpenCode 直接连接 `api.deepseek.com`。配置文件 `~/.config/opencode/opencode.json` 中的 provider 使用 `baseURL: https://api.deepseek.com`，无需代理。

### Codex — 需要代理

```bash
# Codex 用 OpenAI API，必须走代理
https_proxy=http://127.0.0.1:7897 codex

# 或者 export 后启动
export https_proxy=http://127.0.0.1:7897
codex
```

### DeepSeek / 国内 AI — no_proxy 豁免

```bash
# ~/.zshrc 中的 no_proxy 豁免列表
export no_proxy="$no_proxy,api.deepseek.com,platform.deepseek.com,deepseek.com,glm.ai,dashscope.aliyuncs.com"
```

当 `http_proxy` 开启时，DeepSeek 等国内 API 自动走直连。

---

## 3. Clash 规则层

### 进程名规则（rbNfRMyQHAqC.yaml）

```yaml
prepend:
  - PROCESS-NAME,claude,DIRECT        # Claude Code → 直连
  - PROCESS-NAME,opencode,DIRECT      # OpenCode → 直连
  - PROCESS-NAME,.opencode,DIRECT     # OpenCode 子进程 → 直连
  - PROCESS-NAME,codex,飞猪云         # Codex → 代理（2026-05-04 修）
  # 原: PROCESS-NAME,codex,DIRECT     # ← 错误！Codex 用 OpenAI API 必须代理
```

**关键**：`PROCESS-NAME,codex,飞猪云` 不能被改成 DIRECT——Codex 用 OpenAI API，国内被墙。
> 历史：2026-05-02 某 agent 加了这个规则指向 DIRECT，导致规则模式下 Codex 无法使用。

### Codex 流量路径

```
codex 启动
  │
  ├── https_proxy=http://127.0.0.1:7897
  │
  ├── 请求 api.openai.com / chatgpt.com
  │
  ├── Clash 规则匹配:
  │   ├── PROCESS-NAME,codex → 飞猪云（命中！）
  │   └── DOMAIN-SUFFIX,openai.com → OpenAI 组（域名也匹配）
  │
  ├── 飞猪云 → 代理节点
  │
  └── 出站: Trojan → OpenAI 服务器
```

---

## 4. 规则模式 vs 全局模式

| 模式 | 工作方式 | Codex | 国内流量 |
|------|----------|-------|---------|
| **规则模式** | 按规则匹配 | ✅（PROCESS-NAME,codex,飞猪云） | 直连，省流量 |
| **全局模式** | 全部走代理 | ✅ | 浪费流量 |

> 如果 Codex 在规则模式下不能用，检查规则文件中是否有 `PROCESS-NAME,codex,DIRECT` 错误地把 Codex 指向直连。

---

## 5. 排障

### Codex 连不上

```bash
# 1. 确认代理端口活着
ss -tlnp | grep 7897

# 2. 确认环境变量
echo $https_proxy

# 3. 测试 OpenAI 可达性
curl --proxy socks5h://127.0.0.1:7897 -s -o /dev/null -w "%{http_code}" \
  https://api.openai.com/v1/models -H "Authorization: Bearer test"
# 应返回 401（未授权但可达）

# 4. 检查进程规则
grep "codex" ~/.local/share/io.github.clash-verge-rev.clash-verge-rev/clash-verge.yaml
# 应该是 PROCESS-NAME,codex,飞猪云
```

### 全局模式能用，规则模式不能用

说明进程规则把流量指向了 DIRECT（直连）而不是代理。检查 `rbNfRMyQHAqC.yaml` 中 `PROCESS-NAME,codex` 的目标。
