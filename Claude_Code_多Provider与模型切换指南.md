# Claude Code 多 Provider 与模型切换指南

这份指南对应当前这台 Arch 机器上已经配好的 `Claude Code` 使用方式。

## 当前默认行为

- `claude` 和 `cc`：默认走 `GLM / Z.AI`
- 默认 provider 配置文件：`~/.claude/settings.json`
- 当前默认 provider：`glm`
- 当前 `cc` 默认模型优先级：
  1. `glm-5.1`
  2. `glm-5`
  3. `glm-4.7`
  4. `glm-4.6`
  5. `glm-4.5-air`

说明：

- `claude` 启动时会自动清掉系统代理变量，默认按直连方式访问
- 如果当前模型资源包用完，可以手动切到下一个优先模型
- `Claude Code` 交互模式本身不能自动在第三方模型之间无感回退

## 最常用命令

### 1. 默认启动

```bash
cc
```

作用：

- 按当前默认 provider 和默认模型启动
- 启动前会显示当前使用的 provider 和 model

### 2. 查看 `cc` 当前默认模型

```bash
ccwhich
```

### 3. 手动指定 `cc` 的默认模型

```bash
ccuse glm-4.7
```

### 4. 切到下一个优先模型

```bash
ccnext
```

适合场景：

- 当前模型资源包用完
- 想临时降一级继续用

### 5. 恢复到最新优先的默认模型

```bash
ccreset
```

## 按 Provider 启动

### GLM / Z.AI

```bash
ccglm glm-5.1
ccglm glm-4.7
ccglm glm-4.5-air
```

### Qwen / DashScope

```bash
ccq qwen3.6-plus
ccq qwen3-coder-plus
ccq qwen-plus
```

### MiniMax

```bash
ccmini MiniMax-M2.7
ccmini MiniMax-M2.5
```

### 通用格式

```bash
ccp <provider> <model-id>
```

例如：

```bash
ccp glm glm-4.7
ccp qwen qwen3-coder-plus
ccp minimax MiniMax-M2.7
```

## 两级选择器

如果不想记命令，直接用：

```bash
ccpick
```

或者：

```bash
ccswitch
```

它会分两步：

1. 先选 provider
2. 再选该 provider 下的 model

当前内置列表：

- `glm`：`glm-5.1` `glm-5` `glm-4.7` `glm-4.6` `glm-4.5-air`
- `qwen`：`qwen3.6-plus` `qwen3.5-plus` `qwen3-coder-plus` `qwen3-coder-next` `qwen-flash` `qwen-plus`
- `minimax`：`MiniMax-M2.7` `MiniMax-M2.7-highspeed` `MiniMax-M2.5` `MiniMax-M2.1`

也支持 `manual` 手动输入新的模型 ID。

## 进入 Claude Code 后怎么切模型

### 同一个 provider 内切模型

在 Claude Code 里输入：

```text
/model <model-id>
```

例如：

```text
/model glm-4.7
/model qwen3-coder-plus
```

注意：

- `/model` 只能切当前会话所在 provider 下的模型
- `/model` 不能顺便切换 provider

### 跨 provider 切换

退出当前会话，再重新启动：

```bash
ccglm glm-4.7
ccq qwen3.6-plus
ccmini MiniMax-M2.7
```

## API Key 变量

不同 provider 需要各自的 key。

### Z.AI / GLM

默认读取：

- `~/.claude/settings.json` 里的 `ANTHROPIC_AUTH_TOKEN`
- 或环境变量 `ZAI_API_KEY`

### DashScope / Qwen

需要：

```bash
export DASHSCOPE_API_KEY="你的Key"
```

### MiniMax

需要：

```bash
export MINIMAX_API_KEY="你的Key"
```

如果想长期生效，可以把这些 `export` 写到 `~/.zshrc`。

## 关于模型名

### 可以随便写的

- 本地快捷命令名字
- 自己起的显示名

### 不能乱写的

- 真正发给 provider 的 `model-id`

例如这些必须是准确值：

- `glm-5.1`
- `glm-4.7`
- `glm-4.5-air`
- `qwen3.6-plus`
- `qwen3-coder-plus`
- `MiniMax-M2.7`

如果 `model-id` 写错，通常会报模型不存在或请求失败。

## 关于资源包和默认模型

你的 GLM 资源包页面显示的是按模型或适用场景拆开的，不是一个统一总余额。

实际含义是：

- 用哪个模型，就优先消耗那个模型适用的资源包
- 如果没有专属包，通常才会落到通用 token 包或普通计费

因此：

- `cc` 默认模型不一定要永远最强
- 更实用的是按你资源包覆盖情况来安排优先级

当前已经配置成“最新优先”，不够用时直接：

```bash
ccnext
```

## 直连与代理

当前 shell 已做处理：

- `claude` / `cc` 启动时会自动清掉 `http_proxy` / `https_proxy` / `all_proxy`
- 默认按直连方式访问

如果 provider 本身在当前网络环境不能直连，再考虑单独用代理。

## 常见排查

### 1. `cc` 启动后显示的不是你想要的模型

先看当前默认模型：

```bash
ccwhich
```

如果不对，改成你要的：

```bash
ccuse glm-4.7
```

### 2. 进入会话后想换模型

同 provider 内：

```text
/model <model-id>
```

跨 provider：

- 退出当前会话
- 重新用 `ccglm` / `ccq` / `ccmini` 启动

### 3. 某个模型资源包用完

```bash
ccnext
```

### 4. 新增模型后想临时使用

```bash
ccm <model-id>
```

或者：

```bash
ccp <provider> <model-id>
```

## 建议用法

日常默认：

```bash
cc
```

资源包打完切下一个：

```bash
ccnext
```

想明确指定 provider：

```bash
ccglm glm-4.7
ccq qwen3-coder-plus
ccmini MiniMax-M2.7
```

不想记命令：

```bash
ccpick
```
