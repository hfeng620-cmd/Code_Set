#!/bin/bash

# 设置systemd服务的脚本

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVICE_FILE="$SCRIPT_DIR/clean-temp-files.service"
TIMER_FILE="$SCRIPT_DIR/clean-temp-files.timer"

# 创建systemd用户目录
mkdir -p ~/.config/systemd/user

# 复制服务文件
cp "$SERVICE_FILE" ~/.config/systemd/user/
cp "$TIMER_FILE" ~/.config/systemd/user/

# 重新加载systemd
systemctl --user daemon-reload

# 启用并启动定时器
systemctl --user enable clean-temp-files.timer
systemctl --user start clean-temp-files.timer

echo "服务设置完成！"
echo "检查定时器状态："
systemctl --user list-timers --all
