#!/usr/bin/env python3
"""
定期清除临时文件的脚本
可以手动运行或通过cron定期执行
"""

import os
import shutil
import glob
import logging
from pathlib import Path
from datetime import datetime

# 配置日志
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('clean_temp_files.log'),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

# 要清理的临时文件和目录模式
TEMP_PATTERNS = [
    # Python临时文件
    '**/__pycache__',
    '**/*.pyc',
    '**/*.pyo',
    '**/*.pyd',
    '**/*.egg-info',
    
    # aider相关文件
    '**/.aider*',
    '**/aider*',
    
    # IDE和编辑器临时文件
    '**/.vscode',
    '**/.idea',
    '**/*.swp',
    '**/*.swo',
    '**/.DS_Store',
    '**/Thumbs.db',
    
    # 构建和缓存文件
    '**/build',
    '**/dist',
    '**/out',
    '**/output',
    '**/.pytest_cache',
    '**/.coverage',
    '**/htmlcov',
    
    # 虚拟环境
    '**/venv',
    '**/.venv',
    '**/env',
    '**/node_modules',
    
    # 其他临时文件
    '**/*.log',
    '**/*.tmp',
    '**/*.bak',
    '**/*.cache',
]

def clean_temp_files(root_dir='.', dry_run=False):
    """
    清理临时文件和目录
    
    Args:
        root_dir: 要清理的根目录
        dry_run: 是否只显示将要删除的文件而不实际删除
    """
    root_path = Path(root_dir).resolve()
    deleted_count = 0
    total_size = 0
    
    logger.info(f"开始清理临时文件，根目录: {root_path}")
    logger.info(f"{'[DRY RUN]' if dry_run else ''}模式")
    
    for pattern in TEMP_PATTERNS:
        # 使用glob递归查找匹配的文件和目录
        matches = glob.glob(str(root_path / pattern), recursive=True)
        
        for match in matches:
            path = Path(match)
            try:
                if path.is_file():
                    size = path.stat().st_size
                    if dry_run:
                        logger.info(f"[将删除文件] {path} ({size} bytes)")
                    else:
                        path.unlink()
                        logger.info(f"[删除文件] {path} ({size} bytes)")
                    deleted_count += 1
                    total_size += size
                    
                elif path.is_dir():
                    # 检查目录是否为空
                    if any(path.iterdir()):
                        if dry_run:
                            logger.info(f"[将删除目录] {path} (非空)")
                        else:
                            shutil.rmtree(path)
                            logger.info(f"[删除目录] {path} (非空)")
                    else:
                        if dry_run:
                            logger.info(f"[将删除空目录] {path}")
                        else:
                            path.rmdir()
                            logger.info(f"[删除空目录] {path}")
                    deleted_count += 1
                    
            except Exception as e:
                logger.error(f"删除 {path} 时出错: {e}")
    
    logger.info(f"清理完成: 删除了 {deleted_count} 个项目，释放了 {total_size} 字节")
    return deleted_count, total_size

def main():
    """主函数"""
    import argparse
    
    parser = argparse.ArgumentParser(description='清理临时文件')
    parser.add_argument('--dir', '-d', default='.', help='要清理的目录 (默认: 当前目录)')
    parser.add_argument('--dry-run', action='store_true', help='只显示将要删除的文件而不实际删除')
    parser.add_argument('--verbose', '-v', action='store_true', help='详细输出')
    
    args = parser.parse_args()
    
    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
    
    clean_temp_files(args.dir, args.dry_run)

if __name__ == '__main__':
    main()
