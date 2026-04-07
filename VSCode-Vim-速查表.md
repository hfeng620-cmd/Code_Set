# VS Code + Vim 速查表

适用环境：当前这台设备上的 VS Code + VSCodeVim 配置。

## 模式

- 普通模式：像 Vim，适合移动、跳转、查定义
- 插入模式：像 VS Code，适合输入、保存、复制粘贴

切换：

- 进入插入模式：`i`
- 退出插入模式：`Esc`
- 退出插入模式：`jj`

## 先背这几个

- `Space f f`：找文件
- `Space f g`：全局搜索
- `Space w`：保存
- `Space e`：切换侧边栏
- `Space t t`：切换终端
- `g d`：转到定义
- `g r`：查找引用
- `g i`：转到实现
- `K`：查看说明
- `Space r n`：重命名
- `Space c a`：快速修复
- `Space f m`：格式化

## 基础移动

- `h`：左
- `j`：下
- `k`：上
- `l`：右
- `w`：下一个单词
- `b`：上一个单词
- `0`：行首
- `$`：行尾
- `g g`：文件开头
- `Shift+g`：文件结尾

## 插入模式常用

把它当普通 VS Code 用：

- `Ctrl+S`：保存
- `Ctrl+Z`：撤销
- `Ctrl+X`：剪切
- `Ctrl+V`：粘贴
- `Ctrl+P`：快速打开文件
- `Ctrl+K Ctrl+S`：打开快捷键设置
- `Ctrl+Shift+P`：命令面板

## 普通模式常用

- `Ctrl+D`：向下翻半页
- `Ctrl+U`：向上翻半页
- `Ctrl+R`：重做
- `Ctrl+V`：块选择

## 最常用工作流

1. `Space f f` 找文件
2. `g d` 看定义
3. `i` 开始编辑
4. `Esc` 或 `jj` 退出编辑
5. `Space w` 或 `Ctrl+S` 保存
6. `Space c a` 修复问题
7. `Space f m` 格式化

## 简单判断法

- 想输入、保存、复制粘贴：先进入插入模式
- 想移动、跳转、查定义：先回普通模式
