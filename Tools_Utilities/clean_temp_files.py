#!/usr/bin/env python3
"""
定期清除临时文件的脚本
可以手动运行或通过cron定期执行
"""

import os
import shutil
import glob
import json
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

# 默认配置
DEFAULT_CONFIG = {
    "protected_dirs": [
        "venv",
        ".venv", 
        "env",
        "node_modules"
    ],
    "max_file_size_mb": 100,
    "exclude_patterns": [
        "**/*.py",
        "**/*.cpp", 
        "**/*.h",
        "**/*.hpp",
        "**/*.c",
        "**/src/**",
        "**/include/**",
        "**/lib/**"
    ]
}

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
    
    # 其他临时文件
    '**/*.log',
    '**/*.tmp',
    '**/*.bak',
    '**/*.cache',
]

def load_config(config_file='clean_config.json'):
    """加载配置文件"""
    config_path = Path(config_file)
    if config_path.exists():
        try:
            with open(config_path, 'r', encoding='utf-8') as f:
                config = json.load(f)
                logger.info(f"已加载配置文件: {config_file}")
                return config
        except Exception as e:
            logger.warning(f"加载配置文件失败: {e}")
    
    logger.info("使用默认配置")
    return DEFAULT_CONFIG

def should_skip_path(path, config):
    """检查是否应该跳过某个路径"""
    # 检查是否在保护目录中
    for protected_dir in config['protected_dirs']:
        if protected_dir in str(path):
            logger.info(f"跳过受保护目录: {path}")
            return True
    
    # 检查是否在排除模式中
    for pattern in config['exclude_patterns']:
        if Path(path).match(pattern):
            logger.info(f"跳过排除模式: {path} ({pattern})")
            return True
    
    return False

def clean_temp_files(root_dir='.', dry_run=False, config_file='clean_config.json'):
    """
    清理临时文件和目录
    
    Args:
        root_dir: 要清理的根目录
        dry_run: 是否只显示将要删除的文件而不实际删除
        config_file: 配置文件路径
    """
    config = load_config(config_file)
    root_path = Path(root_dir).resolve()
    deleted_count = 0
    total_size = 0
    
    logger.info(f"开始清理临时文件，根目录: {root_path}")
    logger.info(f"{'[DRY RUN]' if dry_run else ''}模式")
    logger.info(f"保护目录: {config['protected_dirs']}")
    
    for pattern in TEMP_PATTERNS:
        # 使用glob递归查找匹配的文件和目录
        matches = glob.glob(str(root_path / pattern), recursive=True)
        
        for match in matches:
            path = Path(match)
            
            # 检查是否应该跳过
            if should_skip_path(path, config):
                continue
            
            try:
                if path.is_file():
                    # 检查文件大小
                    size = path.stat().st_size
                    if size > config['max_file_size_mb'] * 1024 * 1024:
                        logger.info(f"跳过大文件: {path} ({size/1024/1024:.2f} MB)")
                        continue
                    
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
    parser.add_argument('--config', '-c', default='clean_config.json', help='配置文件路径')
    
    args = parser.parse_args()
    
    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
    
    clean_temp_files(args.dir, args.dry_run, args.config)

if __name__ == '__main__':
    main()
