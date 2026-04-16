# Aider 部署记录

## 基本信息
- **部署日期**: 2026年 04月 15日 星期三 22:57:43 CST
- **安装工具**: uv (Python package manager)
- **包名称**: aider-chat
- **当前版本**: 0.86.2

## 文件位置
- **可执行文件路径**: /home/tian/.local/bin/aider
- **虚拟环境路径**: /home/tian/.local/share/uv/tools/aider-chat
- **安装日志**: 使用 `uv tool list` 可查看详情

## 配置文件 (Config Files)
- **默认搜索路径**:
  1. 当前工作目录下的 `.aider.conf.yml`
  2. 用户主目录下的 `~/.aider.conf.yml`
- **手动指定方式**: 使用 `aider --config <path_to_yml>`

## 使用建议
- 在项目根目录下运行 `aider` 即可开始。
- 如果需要配置 API Key 或其他设置，建议创建 `~/.aider.conf.yml` 或使用环境变量。
