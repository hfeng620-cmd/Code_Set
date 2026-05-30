# Kangentic 使用与部署指南

## 先说结论

Kangentic 不是一个大模型平台，也不是一个统一的 API 管理器。

它更像一个本地的任务调度面板：

- 你在 Kangentic 里建任务卡、拖动列、看进度
- Kangentic 在后台调用你电脑里已经装好的 agent CLI
- 每个任务通常跑在独立的 git worktree 里
- 你可以让不同列用不同 agent，实现多 agent 串行或并行

最重要的一点：

**Kangentic 不要求你把所有 agent 都配置一遍。你只需要配置你实际要用的那些 CLI。**

例如：

- 你只想用 `codex`，那就只要 `codex` 能在终端里正常用
- 你想同时用 `claude`、`codex`、`gemini`，那就把这三个 CLI 各自先配置好
- 没装的 agent，Kangentic 也不会神奇替你补出来

它吃的是“你本机已经能用的 CLI”，不是它自己内部再代管一套 API key。

## 先理解多 agent 到底怎么用

很多人第一次看 Kangentic，会误以为它是“一个任务同时让 5 个 agent 围着同一份代码一起改”。这不是最稳的理解。

更稳的理解是：

1. 一个任务卡在某一时刻通常只对应一个正在运行的 agent 会话
2. 多 agent 的主要价值是：
   - 多个任务并行跑
   - 同一个任务在不同阶段交给不同 agent
3. 常见用法不是“所有 agent 一起改同一个文件”，而是：
   - Claude 先做规划
   - Codex 做实现
   - Gemini 做复查或补测试

也就是说，多 agent 最常见的两种模式是：

### 模式 1：多个任务并行

例如你有 4 个任务：

- 修登录页
- 补接口类型
- 修测试
- 重构配置模块

你可以在 Kangentic 里建 4 张卡，每张卡都拖到执行列，让它们各自在独立 worktree 里跑。  
这才是 Kangentic 最直接、最有价值的能力。

### 模式 2：同一个任务分阶段交接

例如一个任务从左到右流过这些列：

- `Planning`
- `Executing`
- `Code Review`
- `Tests`
- `Done`

你可以把它设置成：

- `Planning` 用 Claude
- `Executing` 用 Codex
- `Code Review` 用 Gemini

这样不是三个 agent 同时乱改，而是让任务带着上下文往后流。

## API 到底要不要每个都配

### 不需要在 Kangentic 里统一再配一遍

Kangentic 官方说明的核心意思是：

- 它使用的是你已经安装好的原生 CLI
- 没有自己的 OAuth 流程
- 没有自己的 API 转发层
- 没有自己的云账户体系

所以你不是在 Kangentic 里给所有 agent 再配一次 API key。

### 但你想用哪个 agent，就得先让哪个 CLI 在终端里能正常工作

这是关键区别。

例如：

- 想用 `claude`，先让 `claude` 在终端里能跑
- 想用 `codex`，先让 `codex` 在终端里能跑
- 想用 `gemini`，先让 `gemini` 在终端里能跑

Kangentic 只是调用它们。

所以正确顺序是：

1. 先在普通终端里把某个 CLI 跑通
2. 完成它自己的登录、订阅或 API key 配置
3. 再回到 Kangentic 里用它

### 并不是每个 agent 都必须配置

如果你当前只准备用：

- `codex`
- `gemini`

那你完全可以只配置这两个。

其他没配置的：

- 不影响 Kangentic 本身打开
- 只是对应 agent 不能被正常启动

## Kangentic 和你现在的 Cline 是什么关系

这点很容易混。

你现在能正常用的是 **VS Code 里的 Cline 扩展**。  
它不是 Kangentic 里的一种“原生 agent CLI”。

Kangentic 官方主打支持的是这些 CLI：

- Claude Code
- Codex CLI
- Gemini CLI
- Cursor CLI
- GitHub Copilot CLI
- Aider
- Warp / Oz CLI

所以要明确：

- **Kangentic 主要调度的是终端里的 agent CLI**
- **你现在 VS Code 里的 Cline 扩展，是另一套使用方式**

实际工作里可以这么配合：

- 用 Kangentic 负责拆任务、并行和 worktree 管理
- 用 VS Code + Cline 扩展做你手动盯着处理的细活

## 你在这台机器上应该怎么用

### 你当前可用的相关命令

这台机器上已经确认有这些命令：

- `node`
- `npm`
- `git`
- `claude`
- `codex`
- `gemini`

另外我给你加了一个本地启动命令：

- `kangentic`

它的作用是直接进入你本地的 Kangentic 源码目录并执行 `npm start`。

### 这台机器上最推荐的启动方式

直接执行：

```bash
kangentic
```

等价于：

```bash
cd /home/tian/Extra/migrated/Projects/Kangentic
npm start
```

### 为什么不推荐你现在用 `npx kangentic`

因为你这台机器不是 Debian/Ubuntu 那种 `dpkg` 路线的环境。  
之前实测：

- `npx kangentic` 会去下载 `.deb`
- 然后尝试执行 `sudo dpkg -i ...`
- 你的系统里没有 `dpkg`
- 所以这条安装路线会失败

因此在你这台机器上：

- `npx kangentic` 不是首选
- `kangentic` 或本地源码 `npm start` 才是首选

## 第一次真正使用 Kangentic 的建议流程

### 第 1 步：先不要急着同时上很多 agent

第一次用时，建议只选一个 agent，比如：

- 只用 `codex`

目的很简单：

- 先确认 Kangentic 的项目、列、任务卡、worktree、终端都理解了
- 等单 agent 跑顺了，再上多 agent

### 第 2 步：选一个真实 git 项目

Kangentic 的价值跟 git worktree 强相关。  
所以不要拿一个随便的空目录来理解它，最好拿一个真实项目仓库。

### 第 3 步：创建项目

在 Kangentic 里新建一个 project，指向你的仓库目录。

你需要关心的几个点：

- 仓库根目录是不是对的
- 基础分支一般是不是 `main`
- 是否开启 worktree

通常建议：

- 保持 worktree 开启

这样每个任务都能在自己的分支/工作区里跑，冲突少很多。

### 第 4 步：先做一个最小列流

第一次不必建太复杂的板。

建议先用三个列：

- `To Do`
- `Executing`
- `Done`

如果你已经熟一点，可以再加：

- `Planning`
- `Code Review`
- `Tests`

### 第 5 步：先确定默认 agent

官方思路是：

- 每个项目可以有一个默认 agent
- 每一列还可以覆盖这个默认 agent

最简单的起手方式是：

- 整个项目默认用 `codex`
- 暂时不做列级覆盖

等你熟了再改成：

- `Planning` = `claude`
- `Executing` = `codex`
- `Code Review` = `gemini`

### 第 6 步：建任务卡

任务卡不要写太虚。

建议一张卡里写清楚：

- 目标是什么
- 约束是什么
- 最好改哪些文件
- 不要动什么

例如：

```text
修复登录页在 1024px 以下侧边栏遮挡主内容的问题。
优先改布局样式，不改接口逻辑。
修改完成后跑相关前端测试。
```

### 第 7 步：把卡拖到执行列

拖到执行列后，Kangentic 会：

- 创建或进入对应 worktree
- 选择该列绑定的 agent
- 拉起对应 CLI
- 在内置终端里显示过程

这时你就不是在“手动切 5 个终端”，而是在一个看板里看多个会话。

### 第 8 步：盯住的不是“输出有多酷”，而是三个关键点

看这三个就够了：

1. 任务是不是启动了
2. 改动是不是在正确 worktree / 分支里
3. 结果能不能 review 和合并

不要一开始就追求全自动。

## 多 agent 的推荐起手方式

### 起手方式 A：先单 agent，再双 agent

先这样：

- 全部任务默认 `codex`

等顺了以后，再改成：

- `Planning` 用 `claude`
- `Executing` 用 `codex`

这是最稳的升级路径。

### 起手方式 B：按任务类型分 agent

例如：

- 规划类任务给 `claude`
- 代码实现给 `codex`
- 解释、补充或复查给 `gemini`

这种方式适合你已经知道每个 agent 的长处。

### 起手方式 C：并行多个独立任务

这是最值得用 Kangentic 的地方。

例如你一次放 3 张卡：

- 修一个前端 bug
- 补一个后端类型生成
- 清理一个测试问题

让三张卡分别跑，比让一个 agent 串行干完更能体现 Kangentic 的价值。

## 什么时候需要 handoff

你看到官方提到 handoff context，不要把它想得太神秘。

它的含义基本是：

- 一个任务从列 A 移到列 B
- 列 B 对应另一个 agent
- 新 agent 可以拿到前一个 agent 的历史上下文

适合场景：

- 规划交给实现
- 实现交给复查

不适合场景：

- 只是很小的任务
- 你自己都还没看明白

第一次用时，handoff 不是必需项。  
先把单 agent 和多任务并行跑顺，再考虑 handoff。

## 配置 agent 的现实建议

### 最好的办法不是在 Kangentic 里死磕配置页面

而是先在普通终端里逐个确认。

例如先手动试：

```bash
claude
codex
gemini
```

看它们是否：

- 能启动
- 已登录
- 已有订阅或 API 配置

只要某个 CLI 在普通终端里都跑不起来，Kangentic 里大概率也跑不顺。

### 如果 CLI 装在非标准路径

Kangentic 源码里有明确逻辑：

- 它会自动探测 CLI
- 如果探测不到，可以在设置里显式指定 `cliPath`

所以：

- 标准 PATH 里有命令时，一般直接能用
- 不在 PATH 里时，再去设置里手动指路径

## 你这台机器上的实际部署记录

下面是我在这台机器上真实做过的事情，不是纸面步骤。

### 1. 把源码放到指定目录

我把 Kangentic 官方仓库放到了：

`/home/tian/Extra/migrated/Projects/Kangentic`

这个目录现在包含：

- 源码
- `package.json`
- `kangentic.json`
- `node_modules`

### 2. 安装依赖

在这个目录里执行了：

```bash
npm install
```

安装完成后，`better-sqlite3` 也做了对应重建。

### 3. 代码层面的最小验证

我执行了：

```bash
npm run typecheck
```

这一步通过了。  
说明至少从源码和 TypeScript 角度看，当前目录是完整可用的，不是只克隆了个空壳。

### 4. 试了官方 `npx kangentic` 路线

这一步暴露了第一个关键问题。

`npx kangentic` 的行为是：

1. 下载官方发布的 `.deb`
2. 试图执行：

```bash
sudo dpkg -i ...
```

但你这台机器上没有 `dpkg`，所以这条路失败了。

也就是说：

- 不是 Kangentic 代码本身坏了
- 是它的“Linux 安装器路径”假设你的系统有 `dpkg`

### 5. 为了绕开 `.deb` 安装器，我加了本地命令

我创建了：

`/home/tian/bin/kangentic`

这个脚本做的事情很简单：

```bash
cd /home/tian/Extra/migrated/Projects/Kangentic
npm start
```

因为 `/home/tian/bin` 已经在你的 PATH 里，所以你现在可以直接敲：

```bash
kangentic
```

### 6. 启动验证时又遇到一个沙箱环境问题

我在当前 agent 的运行环境里测试 `kangentic` 时，启动脚本已经进入了项目，也开始执行：

```bash
npm start
```

但随后报了：

- `listen EPERM: operation not permitted ::1:5173`

这个错误出现在当前工具沙箱里，说明这里的本地端口监听受限。  
它更像是“我这个自动化执行环境的限制”，不一定等于你本机桌面环境也会这样。

所以这一步我能确认的是：

- `kangentic` 命令本身已经接对了
- 它确实会进入本地源码目录并启动开发入口
- 但在我这里的受限沙箱里，Vite 监听 `::1:5173` 被拦了

### 7. 你现在最该怎么理解当前状态

当前状态不是“没有装好”，而是：

- 源码已经部署到位
- 依赖已经装好
- 本地启动命令已经配置好
- 官方 `npx kangentic` 安装器不适合你这台机器
- 在我的受限沙箱测试里，开发服务监听端口被限制

这几件事要分开看。

## 当前与你最相关的命令

### 查看帮助

```bash
clineguide
```

### 启动 Kangentic

```bash
kangentic
```

### 进入源码目录手动启动

```bash
cd /home/tian/Extra/migrated/Projects/Kangentic
npm start
```

## 最后的建议

第一次不要一上来就追求：

- 5 个 agent 同时跑
- 每一列都换一种模型
- 自动 handoff + 自动 review + 自动测试全开

更稳的顺序是：

1. 先让一个项目用一个 agent 跑通
2. 再让多个独立任务并行
3. 最后再做列级 agent 切换和 handoff

如果你后面要继续深入，最值得补的一份文档不是“更复杂的配置大全”，而是：

- 以你自己常用的 `claude`、`codex`、`gemini` 为例
- 写一份“哪个列用哪个 agent 最顺手”的个人工作流

这比机械罗列设置项更实用。
