#!/bin/bash

# 清理临时文件的Shell脚本
# 可以通过cron定期执行

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON_SCRIPT="$SCRIPT_DIR/clean_temp_files.py"

echo "开始清理临时文件: $(date)"

# 执行Python脚本
python3 "$PYTHON_SCRIPT" --dir "$SCRIPT_DIR/.." --verbose

echo "清理完成: $(date)"
