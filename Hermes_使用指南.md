# Hermes 使用指南

> 适用目录：`/home/tian/Recording_Datas/Code_Set`
> 
> 本机当前 Hermes 安装位置：`/home/tian/Projects/Hermes`
> 
> 本机当前默认模型：**DeepSeek / deepseek-chat**

---

## 1. Hermes 是什么

Hermes 可以理解成一个在终端里工作的 AI 助手。它不只是聊天，还可以：

- 读写文件
- 调用终端命令
- 帮你整理配置
- 保存会话
- 切换不同模型提供商
- 做多步任务

如果你平时会写代码、改配置、查问题、写脚本，它比普通网页聊天更实用。

---

## 2. 你这台电脑上的当前状态

目前已经帮你配好：

- 默认 provider：`deepseek`
- 默认 model：`deepseek-chat`
- 配置文件：`~/.hermes/config.yaml`
- 环境变量文件：`~/.hermes/.env`
- 项目目录：`/home/tian/Projects/Hermes`

你现在直接输入下面命令就能开始用：

```bash
hermes
```

---

## 3. 最常用的几种用法

### 3.1 直接进入交互模式

```bash
hermes
```

进入后就像一个终端版 AI 助手，可以直接提问，例如：

- 帮我解释这个报错
- 帮我写一个 Python 脚本
- 帮我整理这个目录结构
- 帮我分析这个项目

### 3.2 单次快速提问

```bash
hermes chat -q "你好，介绍一下你自己"
```

适合只问一句，不想进入交互界面的时候。

### 3.3 让它只回一句结果

```bash
hermes chat -q "Reply exactly OK." --max-turns 1
```

适合做快速测试或脚本检测。

---

## 4. 你最值得先记住的命令

### 4.1 检查 Hermes 是否正常

```bash
hermes doctor
```

用途：
- 检查模型配置
- 检查 API key
- 检查命令是否安装正常
- 检查依赖是否齐全

### 4.2 查看当前配置

```bash
hermes config show
```

### 4.3 查看配置文件路径

```bash
hermes config path
hermes config env-path
```

### 4.4 查看日志

```bash
hermes logs
```

### 4.5 查看历史会话

```bash
hermes sessions list
```

---

## 5. DeepSeek 的常用操作

### 5.1 你当前默认就是 DeepSeek

当前默认配置相当于：

```yaml
model:
  provider: deepseek
  default: deepseek-chat
  base_url: ""
```

也就是说，平时直接：

```bash
hermes
```

就会优先走 DeepSeek。

### 5.2 切换成 DeepSeek 推理模型

如果你之后想试更偏推理的模型，可以切换成：

```bash
hermes config set model.provider deepseek
hermes config set model.default deepseek-reasoner
```

如果想切回现在这个默认聊天模型：

```bash
hermes config set model.provider deepseek
hermes config set model.default deepseek-chat
```

### 5.3 DeepSeek 相关环境变量

保存在：

```bash
~/.hermes/.env
```

常见是：

```env
DEEPSEEK_API_KEY=你的key
DEEPSEEK_BASE_URL=https://api.deepseek.com/v1
```

> 不要把这个文件截图发给别人，也不要把 key 贴到公开仓库。

---

## 6. 以后如果想换模型怎么做

### 6.1 用交互方式切换

```bash
hermes model
```

这个命令适合：
- 切换 provider
- 切换 model
- 配置 custom endpoint

### 6.2 手动切换 provider

例如切回 DeepSeek：

```bash
hermes config set model.provider deepseek
hermes config set model.default deepseek-chat
```

### 6.3 如果以后接豆包 / 自定义 OpenAI 兼容接口

通常走：

```bash
hermes model
```

然后选择：

- `Custom endpoint`

或者手动在配置里设置：

```yaml
model:
  provider: custom
  default: 你的模型名
  base_url: 你的接口地址
```

---

## 7. 推荐你的实际使用场景

对于你这种本地开发/学习环境，Hermes 最适合这些事：

### 7.1 排错

例如：
- 帮我分析这个编译错误
- 帮我解释这个 Python traceback
- 帮我检查这个配置为什么不生效

### 7.2 写脚本

例如：
- 写一个批量重命名文件脚本
- 写一个统计目录大小脚本
- 写一个处理文本的小工具

### 7.3 项目阅读

例如：
- 帮我概括这个项目结构
- 帮我找入口文件
- 帮我解释某个函数在干什么

### 7.4 改配置

例如：
- 帮我调整 Neovim / VSCode / 终端配置
- 帮我整理 shell 环境变量
- 帮我写 README 或使用说明

---

## 8. 常见问题

### 8.1 为什么 `hermes doctor` 里还有 OpenRouter warning？

如果你现在只打算用 DeepSeek，那这个提示通常可以先忽略。

只要：
- DeepSeek 检测通过
- `hermes` 能正常启动
- `hermes chat -q "..."` 能返回结果

就说明你当前能正常用。

### 8.2 Hermes 卡住了怎么办？

先试：

```bash
Ctrl + C
```

如果只是想看状态：

```bash
hermes logs
hermes doctor
```

### 8.3 配置改乱了怎么办？

先看：

```bash
hermes config show
```

常见关键文件：

- `~/.hermes/config.yaml`
- `~/.hermes/.env`

---

## 9. 建议你养成的习惯

### 建议 1：先用 `hermes doctor`
很多问题都能先排出来。

### 建议 2：重要配置只改这两个地方

- `~/.hermes/config.yaml`
- `~/.hermes/.env`

### 建议 3：遇到奇怪问题先看日志

```bash
hermes logs
```

### 建议 4：做长期任务时尽量把需求说清楚
例如不要只说“帮我改一下”，而是说：

- 帮我把这个脚本改成支持批量输入
- 帮我把这个目录整理成按日期分类
- 帮我写成 Markdown 使用说明

这样效果会更好。

---

## 10. 一套你可以直接照着用的命令

### 快速确认当前配置

```bash
hermes config show
```

### 检查环境是否正常

```bash
hermes doctor
```

### 进入 Hermes

```bash
hermes
```

### 单次问一句

```bash
hermes chat -q "帮我写一个 Python 文件去重脚本"
```

### 查看历史会话

```bash
hermes sessions list
```

### 查看日志

```bash
hermes logs
```

### 切成 DeepSeek 推理模型

```bash
hermes config set model.provider deepseek
hermes config set model.default deepseek-reasoner
```

### 切回 DeepSeek 聊天模型

```bash
hermes config set model.provider deepseek
hermes config set model.default deepseek-chat
```

---

## 11. 最后给你的建议

对你现在来说，最实用的路线不是把 Hermes 当“玩具聊天工具”，而是把它当：

- **终端里的 AI 助手**
- **配置和排错帮手**
- **脚本和文档生成器**
- **项目阅读助手**

你现在已经把 DeepSeek 接好了，所以最简单的开始方式就是：

```bash
hermes
```

然后直接说你要它干什么。

