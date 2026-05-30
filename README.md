# Code_Set

个人代码、工具、配置与故障排障记录集合。

## 目录结构

### 工具与脚本

| 目录 | 说明 |
|------|------|
| `Screen_Tracker/` | 跨平台屏幕使用时间追踪器 |
| `Video_player/` | 视频播放器（PyInstaller 打包） |
| `Crawler_Scripts/` | 爬虫脚本（B站、豆瓣等） |
| `Data_Analysis/` | 数据分析与机器学习评估脚本 |
| `Tools_Utilities/` | 数据库检查、PyInstaller 提取、系统服务等工具 |
| `AudioControlHotkeys/` | Windows 音频控制热键（AutoHotkey） |

### 配置指南

| 文件/目录 | 说明 |
|-----------|------|
| `Clash_Verge_配置详解/` | Clash Verge 代理配置详解 |
| `Windows_Terminal_Settings/` | Windows Terminal 美化与配置 |
| `niri窗口关闭与打开配置/` | Niri 窗口管理器配置 |
| `代理自动切换配置指南.md` | 代理自动切换配置说明 |
| `Documentation/Editors/VSCode-shortcuts.md` | VSCode 快捷键速查 |
| `Documentation/Editors/VSCode-Vim-速查表.md` | VSCode Vim 模式速查 |
| `Documentation/Editors/VSCode-Vim-个人适配指南.md` | VSCode Vim 个人适配指南 |
| `Documentation/Git/GIT_GUIDE_FOR_YOUR_REPOS.md` | Git 使用指南 |
| `开机自定义/` | Plymouth 开机画面自定义 |

### AI 工具使用指南

| 文件 | 说明 |
|------|------|
| `Documentation/AI_Tools/Claude_Code_多Agent与Subagent使用指南.md` | Claude Code 多 Agent 与 Subagent |
| `Documentation/AI_Tools/Claude_Code_多Provider与模型切换指南.md` | Claude Code 多 Provider 与模型切换 |
| `Documentation/AI_Tools/DeepTutor-使用与高效使用指南.md` | DeepTutor 使用指南 |
| `Documentation/AI_Tools/Hermes_使用指南.md` | Hermes 使用指南 |
| `Documentation/AI_Tools/Kangentic_使用与部署指南.md` | Kangentic 使用与部署指南 |

### 学习与练习

| 目录 | 说明 |
|------|------|
| `C_Cpp_Code/` | C/C++ 练习代码（黑马程序员教程） |
| `Python课程练习/` | Python 课程练习 |
| `Learning_Exercises/` | 算法动画、粒子系统、ML 评估脚本等练习 |

### 排障记录

| 目录 | 说明 |
|------|------|
| `arch-usb-boot-repair-20260512/` | Arch USB SSD 引导修复记录 |
| `AAGL_Genshin_Arch_排障记录/` | Arch 上原神启动器排障 |

### 其他

| 目录/文件 | 说明 |
|-----------|------|
| `VMware_Setting/` | VMware 服务管理自动化 |
| `vscode-local-agent-bridge/` | VSCode 本地代理桥接扩展 |
| `Documentation/` | 技术文档与参考笔记 |

## 环境

- **OS**: Arch Linux + Windows 11 双系统
- **Python**: 3.12
- **Node.js**: LTS
- **C/C++**: MSYS2 UCRT64 GCC
- **Shell**: Zsh / PowerShell
- **Editor**: VSCode with Vim

## 本地 VSCode

- 工作区配置放在 `.vscode/`
- Python 解释器默认指向仓库内的 `.venv`
- 常用运行任务覆盖 Python、Python GUI 和 C/C++ 小练习

## 依赖

- Python 依赖清单见 `requirements-vscode.txt`
