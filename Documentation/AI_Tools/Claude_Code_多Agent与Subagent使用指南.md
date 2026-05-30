# Claude Code 多 Agent 与 Subagent 使用指南

这份指南只对应你**现在这台机器上已经配好的 `Claude Code` 多 agent 方案**。

重点先说结论：

- 你现在的 `Claude Code` 多 agent，主力是 **Claude 会话内的 subagents**
- 它的使用习惯已经尽量做成接近 `oh-my-codex`
- **先不要动你现有的 Codex / OMX team**
- 先把 `Claude Code` 当成一套“会话内 leader + subagents”的系统来用

也就是说，你现在最应该习惯的是：

- `/deep-interview`
- `/ralplan`
- `/ralph`
- `/team`
- `/analyze`
- `/ultraqa`
- `/code-review`
- `/security-review`

而不是去追求 tmux pane 版本的 team。

---

## 1. 你现在这套 Claude Code 是什么结构

你这台机器上的 `Claude Code`，现在大致分成 4 层：

### 第 1 层：模型与全局设置

配置文件：

- `~/.claude/settings.json`

当前关键配置：

- 主模型：`deepseek-v4-pro[1m]`
- 子 agent / 轻量模型：`deepseek-v4-flash`
- effort：`max`

意思就是：

- 主线程负责更重的任务
- 子 agent 默认走更轻的模型

---

### 第 2 层：全局工作流说明

文件：

- `~/.claude/CLAUDE.md`

这个文件的作用类似你熟悉的全局 `AGENTS.md`：

- 定义默认工作姿势
- 定义什么时候该分析、什么时候该规划、什么时候该执行
- 定义多 agent 时 leader 应该怎么分工

---

### 第 3 层：workflow / skills

目录：

- `~/.claude/skills/`

这层就是你以后在 `Claude Code` 里最常用的入口。

你现在已经配好的有：

- `deep-interview`
- `ralplan`
- `ralph`
- `team`
- `analyze`
- `ultraqa`
- `code-review`
- `security-review`
- `project-setup`

可以把它们理解成：

- 不是单纯的 prompt
- 而是“带工作流语义的命令入口”

---

### 第 4 层：subagents

目录：

- `~/.claude/agents/`

你现在已经有这些角色：

- `omx-coordinator`
- `omx-analyst`
- `omx-planner`
- `omx-executor`
- `omx-verifier`
- `omx-reviewer`
- `omx-debugger`
- `omx-security-reviewer`

你可以把它们理解成：

- `coordinator`：leader
- `analyst`：读代码、分析现状
- `planner`：先出方案
- `executor`：真正改代码
- `verifier`：跑测试、确认结果
- `reviewer`：代码审查
- `debugger`：找根因
- `security-reviewer`：安全审查

---

## 2. 你现在最应该怎么理解“多 agent”

你当前这套 `Claude Code` 多 agent，**不是** `oh-my-codex team` 那种：

- 一个 leader
- 多个真实 tmux pane
- 每个 pane 一个终端 worker

你现在这套更准确地说是：

- 一个主 Claude 会话
- 里面可以协调多个 subagents
- 它们有不同角色
- 但你不会像 `tmux` 那样看到多个真实终端窗格

所以现在先用这个心智模型：

### Claude Code 现在的多 agent = 会话内协作

- leader：当前主会话
- workers：subagents
- 分工：靠 `skills + agents`
- 结果：主会话收敛并汇总

### Codex / OMX team = tmux 终端编排

- leader：一个 pane
- workers：几个真实终端 pane
- 分工：靠 `omx team`
- 结果：你能直接看见每个 pane

这两者不要混在一起理解。

你现在既然说了**不要动现有的 Codex team**，那当前最合适的路线就是：

- 把 `Claude Code` 先当成“会话内 subagent 系统”来用
- tmux team 以后再做完全隔离版

---

## 3. 现在每个 workflow 是干什么的

### 3.1 `/deep-interview`

适合：

- 你的要求还不够清楚
- 你自己也没完全想明白
- 不希望 AI 乱假设

作用：

- 先问清楚
- 先列约束
- 先定义验收条件
- **不会急着动代码**

你可以这样理解：

- 这是“先问清楚再干”

示例：

```text
/deep-interview 我想把这个项目的登录逻辑重构一下，但不要影响现在的接口行为
```

---

### 3.2 `/ralplan`

适合：

- 需求已经差不多清楚
- 你想先看方案
- 不想上来就乱改

作用：

- 先读代码
- 找会改哪些文件
- 给出方案
- 给出验证计划
- **默认不直接开改**

你可以这样理解：

- 这是“先方案后动手”

示例：

```text
/ralplan 给这个 Flask 项目加一个登录失败锁定机制，先出方案和验证计划
```

---

### 3.3 `/ralph`

适合：

- 需求已经清楚
- 你想让它一路干到底
- 让它自己实现并验证

作用：

- 短计划
- 直接改
- 跑验证
- 不通过就继续修

你可以这样理解：

- 这是“单执行者一路干到底”

示例：

```text
/ralph 修复这个 Python 脚本对空文件输入会崩溃的问题，并验证修好
```

---

### 3.4 `/team`

适合：

- 任务能拆成几个独立子问题
- 你想让 leader 在一个会话里做多 agent 分工
- 但你不要求看到 tmux pane

作用：

- 主会话充当 coordinator
- 自动考虑 analyst / planner / executor / verifier 这些分工
- 更适合复杂任务

你可以这样理解：

- 这是“Claude 会话内版 team”

示例：

```text
/team 先分析这个仓库的启动流程，再找测试问题，最后汇总一个修复方向
```

注意：

- 这个不是 `omx team`
- 不会自动分出几个真实终端 pane

---

### 3.5 `/analyze`

适合：

- 你只想知道“为什么”
- 先不改代码
- 先做只读分析

作用：

- 区分 evidence / inference / unknown
- 排名解释可能性
- 只读，不乱改

你可以这样理解：

- 这是“先查清楚再说”

示例：

```text
/analyze 为什么这个仓库的测试在 Linux 上会过，在 Windows 上会挂
```

---

### 3.6 `/ultraqa`

适合：

- 你已经知道目标
- 但需要反复测试、修、重跑

作用：

- 测试
- 定位
- 修复
- 再测
- 一直循环到通过或明确卡住

你可以这样理解：

- 这是“QA 循环模式”

示例：

```text
/ultraqa 把这个 pytest 失败修到通过
```

---

### 3.7 `/code-review`

适合：

- 你要它审代码
- 你想看 findings first

作用：

- 先列问题
- 再说风险
- 最后才给简短总结

示例：

```text
/code-review 审一下当前分支的改动，重点看回归风险和缺测试的问题
```

---

### 3.8 `/security-review`

适合：

- 看 secret
- 看 auth
- 看命令执行风险
- 看信任边界

示例：

```text
/security-review 看一下这个上传接口有没有明显安全问题
```

---

### 3.9 `/project-setup`

适合：

- 你进入一个新仓库
- 想给它快速生成项目级 Claude 配置

作用：

- 生成 repo-local `CLAUDE.md`
- 生成 repo-local `.claude/settings.json`
- 记录常用命令和敏感路径

示例：

```text
/project-setup
```

---

## 4. 现在你的 subagent 分工是怎样的

你可以把当前角色这样记：

### `omx-coordinator`

适合：

- 任务可拆分
- 要做总调度

作用：

- 作为 leader
- 决定谁去分析、谁去执行、谁去验证

---

### `omx-analyst`

适合：

- 读代码
- 查调用链
- 查影响范围

特点：

- 偏只读
- 偏证据导向

---

### `omx-planner`

适合：

- 你要先出方案
- 不想一上来就动代码

特点：

- 偏 planning
- 偏验证路径设计

---

### `omx-executor`

适合：

- 真正改代码
- 做 feature
- 修 bug

特点：

- 是默认执行者

---

### `omx-verifier`

适合：

- 跑测试
- 检查结果
- 判断“到底是不是真的完成了”

特点：

- 对“口头完成”比较严格

---

### `omx-reviewer`

适合：

- 看 diff
- 挑问题
- 看回归

---

### `omx-debugger`

适合：

- 根因分析
- 缩小问题范围
- 查失败链路

---

### `omx-security-reviewer`

适合：

- secrets
- 权限
- shell / 文件 / 网络安全边界

---

## 5. 你现在最推荐的使用套路

这是最适合你目前这套机器配置的方式。

### 套路 A：模糊需求

```text
/deep-interview
```

然后：

```text
/ralplan
```

最后：

```text
/ralph
```

适合：

- 需求还没讲透
- 不希望 AI 一上来乱做

---

### 套路 B：复杂任务，但先不追求 tmux pane

```text
/team
```

适合：

- 你想让它像 leader 一样思考
- 想让它自动考虑 analyst / planner / executor / verifier
- 但先接受它是在一个 Claude 会话里完成，而不是几个终端 pane

---

### 套路 C：只查原因

```text
/analyze
```

适合：

- 先想知道“为什么”
- 不想直接改

---

### 套路 D：修测试

```text
/ultraqa
```

适合：

- 目标明确
- 但修复需要循环验证

---

### 套路 E：看改动质量

```text
/code-review
/security-review
```

适合：

- 改完之后做审查

---

## 6. 你现在先不要怎么用

为了**不影响你当前现有的 Codex / OMX team**，现在先不要把 `Claude Code` 当成 tmux team 系统来折腾。

也就是说，当前阶段先不要把注意力放在：

- “为什么它没有几个 pane”
- “为什么不像 `omx team` 那样可视化”
- “能不能现在就搞一个 Claude 版 tmux team”

因为你已经明确说了：

- 不要影响现有 Codex team

所以当前最合理的路线就是：

### 先适应 Claude 原生 subagent

先把下面这些用顺手：

- `/deep-interview`
- `/ralplan`
- `/ralph`
- `/team`
- `/analyze`
- `/ultraqa`

等你确定这套会话内多 agent 你已经顺手了，再去做完全隔离的 Claude tmux team。

---

## 7. 你现在有哪些辅助层

除了上面的 workflow 和 agents，你现在还有这些辅助：

### 7.1 `claudeguide`

终端命令：

```bash
claudeguide
```

作用：

- 快速看当前 `Claude Code` 怎么用

---

### 7.2 启动上下文 hook

文件：

- `~/.claude/hooks/session-start-context.sh`

作用：

- 进入一个没有 `CLAUDE.md` 的项目时
- 给出项目类型和可能的命令提示

---

### 7.3 安全护栏 hook

文件：

- `~/.claude/hooks/pretool-guardrails.sh`

作用：

- 拦危险命令
- 拦敏感文件读取

---

### 7.4 项目模板

文件：

- `~/.claude/templates/project-claude.template.md`
- `~/.claude/templates/project-settings.template.json`

作用：

- 给新仓库快速生成 Claude 项目级配置

---

## 8. 最后给你的建议

你现在最正确的用法，不是去把 `Claude Code` 硬拗成 `Codex tmux team`。

而是：

### 第一步

先把它当成：

- **一个带 OMX 风格工作流的 Claude 会话**

### 第二步

把这些命令用顺手：

- `/deep-interview`
- `/ralplan`
- `/ralph`
- `/team`
- `/analyze`
- `/ultraqa`

### 第三步

等你用顺手以后，再决定要不要做：

- **完全隔离的 Claude tmux team**

这样不会碰你的现有 Codex team，也不会把两套运行时搅在一起。

---

## 9. 相关文件位置

这套 Claude Code 多 agent 当前关键文件在：

- `~/.claude/CLAUDE.md`
- `~/.claude/settings.json`
- `~/.claude/skills/`
- `~/.claude/agents/`
- `~/bin/claudeguide`

这份指南保存在：

- `/home/tian/Recording_Datas/Code_Set/Claude_Code_多Agent与Subagent使用指南.md`
