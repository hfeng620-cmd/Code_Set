# Git 完整使用指南（结合你当前的 GitHub 仓库）

本指南按你现在机器上的真实结构来写，不按假设来写。

## 1. 你当前的仓库结构

你现在主要有两套 Git 结构：

### 仓库 A：`Code_Set`

- 本地路径：`/home/tian/Recording_Datas/Code_Set`
- Git 仓库根目录：`/home/tian/Recording_Datas/Code_Set`
- 当前分支：`main`
- 远程仓库：
  - `origin = https://github.com/hfeng620-cmd/Code_Set.git`

这说明 `Code_Set` 是一个独立仓库。

### 仓库 B：`Recording_Datas`

- 本地路径：`/home/tian/Recording_Datas`
- Git 仓库根目录：`/home/tian/Recording_Datas`
- 当前分支：`main`
- 远程仓库：
  - `origin = git@github.com:hfeng620-cmd/Recording_Datas.git`

注意：

- `/home/tian/Recording_Datas/ChatGPT_Recording` 不是独立仓库。
- `ChatGPT_Recording` 属于上级仓库 `Recording_Datas`。
- `/home/tian/Recording_Datas/Code_Set` 因为自己内部有 `.git`，所以它是嵌套的独立仓库。

这意味着：

- 你要提交 `Code_Set` 的内容时，要进入 `Code_Set` 仓库操作。
- 你要提交 `ChatGPT_Recording` 的内容时，要在 `Recording_Datas` 仓库操作。
- 不能把这两个目录当成同一个 Git 仓库来提交。

## 2. 最重要的理解

### `Code_Set` 怎么提交

在这里操作：

```bash
cd /home/tian/Recording_Datas/Code_Set
```

### `ChatGPT_Recording` 怎么提交

在这里操作：

```bash
cd /home/tian/Recording_Datas
```

因为 `ChatGPT_Recording` 的 Git 归属是 `Recording_Datas`，不是它自己。

## 3. 先学会看状态

无论在哪个仓库，第一步都建议先看状态：

### 看 `Code_Set` 状态

```bash
cd /home/tian/Recording_Datas/Code_Set
git status
```

### 看 `Recording_Datas` 状态

```bash
cd /home/tian/Recording_Datas
git status
```

你当前实际情况里：

- `Code_Set` 里已经有一些已修改文件和一个已删除文件。
- `Recording_Datas` 里当前有一些未跟踪内容，其中包括 `Code_Set/`、`Codex_Resume/`、若干 `.md`、脚本、以及 `便签/`。

## 4. 最常用的提交流程

提交的标准流程只有 4 步：

1. `git status`
2. `git add`
3. `git commit -m "说明"`
4. `git push`

下面分别按你的两个仓库来写。

## 5. 提交 `Code_Set` 到 GitHub

### 5.1 进入仓库

```bash
cd /home/tian/Recording_Datas/Code_Set
```

### 5.2 查看改动

```bash
git status
git diff
```

如果只想看某个文件：

```bash
git diff VSCode-shortcuts.md
```

### 5.3 添加改动到暂存区

添加单个文件：

```bash
git add VSCode-shortcuts.md
```

添加多个指定文件：

```bash
git add C_Cpp_Code/黑马练习/1.3变量.cpp
git add C_Cpp_Code/黑马练习/1.4常量
```

添加当前仓库全部改动：

```bash
git add .
```

如果有删除文件，也想一起记录：

```bash
git add -A
```

建议：

- 想精确提交时，用 `git add 文件名`
- 想一次性提交全部时，用 `git add -A`

### 5.4 提交

```bash
git commit -m "更新 C++ 练习和 VSCode 快捷键文档"
```

提交说明建议写清楚你做了什么，不要只写 `update`。

### 5.5 推送到 GitHub

```bash
git push origin main
```

如果你当前就在 `main`，也可以直接：

```bash
git push
```

## 6. 提交 `ChatGPT_Recording` 到 GitHub

因为 `ChatGPT_Recording` 属于 `Recording_Datas` 仓库，所以要在上级目录提交。

### 6.1 进入正确仓库

```bash
cd /home/tian/Recording_Datas
```

### 6.2 查看 `ChatGPT_Recording` 改动

```bash
git status
git status -- ChatGPT_Recording
```

查看具体差异：

```bash
git diff -- ChatGPT_Recording
```

### 6.3 只添加 `ChatGPT_Recording`

```bash
git add ChatGPT_Recording
```

如果只想加其中某个文件：

```bash
git add ChatGPT_Recording/某个文件.html
```

### 6.4 提交

```bash
git commit -m "更新 ChatGPT_Recording 对话导出文件"
```

### 6.5 推送

```bash
git push origin main
```

## 7. 怎么从 GitHub 更新到本地

“更新到本地”通常就是把远程最新提交拉到你电脑里。

核心命令：

```bash
git pull
```

但拉取前一定先看你本地有没有改动。

## 8. 更新本地 `Code_Set`

### 8.1 进入仓库

```bash
cd /home/tian/Recording_Datas/Code_Set
```

### 8.2 先检查状态

```bash
git status
```

如果工作区是干净的，直接拉：

```bash
git pull origin main
```

### 8.3 如果本地有改动怎么办

你当前的 `Code_Set` 就有本地改动，所以不能无脑 `git pull`。

你有 3 种处理方式：

#### 方式 1：先提交再拉取

最稳妥：

```bash
git add -A
git commit -m "本地修改先保存"
git pull --rebase origin main
```

#### 方式 2：先临时保存，再拉取，再恢复

适合改到一半还不想提交：

```bash
git stash
git pull origin main
git stash pop
```

#### 方式 3：确认不要本地修改，再丢弃

只有你确定这些改动完全不要了，才这样做：

```bash
git restore .
git pull origin main
```

如果包含删除、新增、暂存等复杂情况，丢弃前一定先确认。

## 9. 更新本地 `ChatGPT_Recording`

因为它属于 `Recording_Datas` 仓库，所以要这样做：

```bash
cd /home/tian/Recording_Datas
git status
git pull origin main
```

如果 `Recording_Datas` 里有本地改动，同样先：

```bash
git add .
git commit -m "保存本地改动"
git pull --rebase origin main
```

或者：

```bash
git stash
git pull origin main
git stash pop
```

## 10. 推荐你实际使用的日常命令

### `Code_Set` 日常提交流程

```bash
cd /home/tian/Recording_Datas/Code_Set
git status
git add -A
git commit -m "写清楚这次修改内容"
git push origin main
```

### `Code_Set` 日常更新流程

```bash
cd /home/tian/Recording_Datas/Code_Set
git status
git pull --rebase origin main
```

如果有本地改动：

```bash
cd /home/tian/Recording_Datas/Code_Set
git add -A
git commit -m "保存本地改动"
git pull --rebase origin main
```

### `ChatGPT_Recording` 日常提交流程

```bash
cd /home/tian/Recording_Datas
git status -- ChatGPT_Recording
git add ChatGPT_Recording
git commit -m "更新 ChatGPT_Recording 内容"
git push origin main
```

### `ChatGPT_Recording` 日常更新流程

```bash
cd /home/tian/Recording_Datas
git pull --rebase origin main
```

## 11. 怎么查看提交历史

### 当前仓库最近提交

```bash
git log --oneline -10
```

### 查看图形化分支关系

```bash
git log --oneline --graph --decorate --all -20
```

### 看某个文件是谁改的

```bash
git blame VSCode-shortcuts.md
```

## 12. 怎么查看远程仓库信息

```bash
git remote -v
```

你当前对应关系是：

### `Code_Set`

```text
origin  https://github.com/hfeng620-cmd/Code_Set.git
```

### `Recording_Datas`

```text
origin  git@github.com:hfeng620-cmd/Recording_Datas.git
```

说明：

- `Code_Set` 现在走 HTTPS
- `Recording_Datas` 现在走 SSH

两种都能用，只要认证正常即可。

## 12.1 `Code_Set` 推送失败时为什么会提示 GitHub 密码错误

如果你看到类似报错：

```text
remote: Invalid username or token. Password authentication is not supported for Git operations.
致命错误：'https://github.com/hfeng620-cmd/Code_Set.git/' 鉴权失败
```

这表示：

- 你的仓库远程地址还是 HTTPS
- GitHub 不支持再用账号密码执行 Git 推送
- 所以输入 GitHub 登录密码不会成功

这时应把远程地址改成 SSH。

## 12.2 把 `Code_Set` 从 HTTPS 改成 SSH

```bash
cd /home/tian/Recording_Datas/Code_Set
git remote set-url origin git.com:hfeng620-cmd/Code_Set.git
git remote -v
git push
```

这几句的作用分别是：

- `cd /home/tian/Recording_Datas/Code_Set`
  进入 `Code_Set` 仓库目录
- `git remote set-url origin git.com:hfeng620-cmd/Code_Set.git`
  把远程仓库 `origin` 从 HTTPS 改成 SSH
- `git remote -v`
  查看远程地址是否修改成功
- `git push`
  把本地提交推送到 GitHub

如果修改成功，`git remote -v` 应该显示：

```text
origin  git.com:hfeng620-cmd/Code_Set.git (fetch)
origin  git.com:hfeng620-cmd/Code_Set.git (push)
```

## 12.3 SSH 口令和 GitHub 密码不是一回事

如果你看到：

```text
Enter passphrase for key /home/tian/.ssh/id_ed25519:
```

这表示：

- Git 正在使用你的 SSH 私钥认证
- 这里要求输入的是 SSH 私钥的口令
- 这不是 GitHub 网站登录密码

如果后面看到类似：

```text
To github.com:hfeng620-cmd/Code_Set.git
   b051d6c..68ef0e1  main -> main
```

就表示推送已经成功完成。

## 12.4 怎么让 SSH 口令只输一次

最实用的做法是让 `ssh-agent` 临时记住你的私钥口令。

先在当前终端执行：

```bash
eval "1000 65534 1000 65534ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
```

然后系统会要求你输入一次 SSH 私钥口令。输完之后，在当前终端会话里再执行 `git push`，通常就不用重复输入了。

### 这两句分别做什么

- `eval "1000 65534 1000 65534ssh-agent -s)"`
  启动 `ssh-agent`，并把它的环境变量加载到当前 shell
- `ssh-add ~/.ssh/id_ed25519`
  把你的 SSH 私钥加入 agent，让 agent 帮你暂时保管解锁后的密钥

### 验证是否生效

可以直接测试：

```bash
git -C /home/tian/Recording_Datas/Code_Set push
```

如果 agent 已经记住口令，这次通常不会再要求你输入。

### 注意

- 这通常只对当前终端会话生效
- 你重新打开终端后，可能还需要再执行一次
- 如果你想开终端后基本自动可用，可以后续再把 `ssh-agent` 配进 `~/.zshrc`

## 12.4.1 如果想更自动化，可以放进 `~/.zshrc`

如果你希望每次打开终端时更省事，可以把下面这段加入 `~/.zshrc`：

```bash
if [ -z "$SSH_AUTH_SOCK" ]; then
  eval "$(ssh-agent -s)" >/dev/null
fi

ssh-add -l >/dev/null 2>&1
if [ $? -ne 0 ]; then
  ssh-add ~/.ssh/id_ed25519 </dev/tty
fi
```

这段配置的作用是：

- 如果当前终端还没有连接到 `ssh-agent`，就启动一个
- 如果你的私钥还没加入 agent，就自动执行 `ssh-add ~/.ssh/id_ed25519`
- 这样你通常只需要在新终端里输入一次 SSH 口令

### 注意事项

- 第一次打开新终端时，它仍然可能要求你输入一次口令
- 如果系统里已经有桌面环境自带的 keyring 或 agent，这段配置有时会显得重复
- 如果你以后发现每开一个终端都会启动新的 agent，可以再调整成更严格的写法

### 修改完后怎么生效

执行：

```bash
source ~/.zshrc
```

或者直接关闭当前终端，再打开一个新终端。

## 12.5 怎么测试 SSH 是否可用

```bash
ssh -T git.com
```

这不是推送代码，它只是测试你本机能否用 SSH 身份连接 GitHub。

如果配置正常，常见返回类似：

```text
Hi hfeng620-cmd! You've successfully authenticated, but GitHub does not provide shell access.
```

这说明 SSH 认证已经成功。

## 13. 什么时候用 `git pull --rebase`

建议你优先用：

```bash
git pull --rebase origin main
```

原因：

- 提交历史更直
- 少一些无意义 merge commit
- 适合个人仓库日常同步

如果你不想用 `rebase`，也可以：

```bash
git pull origin main
```

## 14. 遇到冲突怎么办

如果执行 `git pull --rebase origin main` 或 `git merge` 后冲突了：

### 第一步：看冲突文件

```bash
git status
```

### 第二步：打开文件手动改

冲突文件里会出现：

```text
<<<<<<< HEAD
你的内容
=======
远程内容
>>>>>>> 分支名或提交号
```

你需要手动保留正确内容，并删掉这些标记。

### 第三步：标记已解决

```bash
git add 冲突文件名
```

### 第四步：继续

如果是 rebase：

```bash
git rebase --continue
```

如果是 merge：

```bash
git commit
```

如果想放弃本次 rebase：

```bash
git rebase --abort
```

## 15. 你这个目录结构的特殊注意事项

这是最重要的一节。

### 注意 1：`Code_Set` 是嵌套独立仓库

`/home/tian/Recording_Datas/Code_Set` 自己内部有 `.git`，所以它不是普通文件夹。

这会导致：

- 在 `/home/tian/Recording_Datas` 里执行 `git status` 时，`Code_Set/` 会显示成未跟踪目录。
- 但你在 `Recording_Datas` 仓库里并不能正常直接管理 `Code_Set` 内部的每个文件历史。

简单理解：

- `Recording_Datas` 管不了 `Code_Set` 的内部提交历史
- `Code_Set` 必须在它自己的仓库里单独提交

### 注意 2：不要在错误目录提交

错误示例：

```bash
cd /home/tian/Recording_Datas/ChatGPT_Recording
git commit -m "update"
```

这个命令虽然可能还能沿着父目录找到 Git 仓库，但你容易误判自己是在 `ChatGPT_Recording` 独立仓库里操作。

更清晰的正确方式：

```bash
cd /home/tian/Recording_Datas
git add ChatGPT_Recording
git commit -m "更新 ChatGPT_Recording"
```

### 注意 3：不要混着提交两个仓库

你最好养成这个习惯：

- 改 `Code_Set`，就在 `Code_Set` 目录提交
- 改 `ChatGPT_Recording`，就在 `Recording_Datas` 目录提交

不要想一次 `git add .` 同时把两边都管了。

## 16. 你现在可以直接用的命令

### 提交 `Code_Set`

```bash
cd /home/tian/Recording_Datas/Code_Set
git status
git add -A
git commit -m "更新代码与文档"
git push origin main
```

### 更新 `Code_Set` 到本地

```bash
cd /home/tian/Recording_Datas/Code_Set
git status
git pull --rebase origin main
```

### 提交 `ChatGPT_Recording`

```bash
cd /home/tian/Recording_Datas
git status -- ChatGPT_Recording
git add ChatGPT_Recording
git commit -m "更新 ChatGPT 对话记录"
git push origin main
```

### 更新 `Recording_Datas` 到本地

```bash
cd /home/tian/Recording_Datas
git status
git pull --rebase origin main
```

## 17. 建议你长期固定使用的习惯

建议固定遵守下面这套顺序：

1. 先 `git status`
2. 再 `git diff`
3. 再 `git add`
4. 再 `git commit -m "明确说明"`
5. 最后 `git push`

更新代码前固定先做：

1. `git status`
2. 确认有没有未提交改动
3. 没改动就 `git pull --rebase`
4. 有改动就先提交或 `git stash`

## 18. 一句话总结

你现在不是一个仓库，而是两个层级不同的 Git 使用场景：

- `Code_Set` 是独立仓库，单独提交、单独拉取
- `ChatGPT_Recording` 属于 `Recording_Datas` 仓库，要在上级目录提交和更新

只要你永远先确认“我现在站在哪个 Git 仓库根目录”，Git 基本就不会乱。
