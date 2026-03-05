#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
演示如何在VSCode中回溯文件的Python脚本
"""

import os
import sys
import subprocess

def print_git_history():
    """打印Video_Player.py的Git历史"""
    print("=" * 60)
    print("Video_Player.py 文件历史")
    print("=" * 60)
    
    # 获取提交历史
    cmd = ['git', 'log', '--oneline', '--', 'Code_Set/Video_Player.py']
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode == 0:
        commits = result.stdout.strip().split('\n')
        print(f"找到 {len(commits)} 个提交版本:")
        for i, commit in enumerate(commits):
            print(f"{i+1}. {commit}")
    else:
        print("无法获取Git历史")
    
    print()

def show_file_versions():
    """显示不同版本的文件内容"""
    print("=" * 60)
    print("不同版本的文件内容对比")
    print("=" * 60)
    
    # 获取提交哈希
    cmd = ['git', 'log', '--pretty=format:%H', '--', 'Code_Set/Video_Player.py']
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode == 0:
        hashes = result.stdout.strip().split('\n')
        
        print("版本1 (初始版本):")
        cmd = ['git', 'show', f'{hashes[-1]}:Code_Set/Video_Player.py']
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode == 0:
            lines = result.stdout.split('\n')[:10]  # 只显示前10行
            for line in lines[:5]:
                print(f"  {line}")
            print("  ...")
        
        print("\n版本2 (添加datetime):")
        if len(hashes) >= 2:
            cmd = ['git', 'show', f'{hashes[-2]}:Code_Set/Video_Player.py']
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode == 0:
                lines = result.stdout.split('\n')[:10]
                for line in lines[:5]:
                    print(f"  {line}")
                print("  ...")
        
        print("\n版本3 (当前版本，添加logging):")
        with open('Code_Set/Video_Player.py', 'r', encoding='utf-8') as f:
            lines = f.readlines()[:10]
            for line in lines[:5]:
                print(f"  {line.strip()}")
            print("  ...")
    
    print()

def vscode_commands_demo():
    """演示VSCode中的回溯命令"""
    print("=" * 60)
    print("VSCode 中回溯文件的命令")
    print("=" * 60)
    
    commands = [
        ("查看本地历史", "Ctrl+Shift+P → 'File: Open Local History'"),
        ("查看Git历史", "Ctrl+Shift+G → 右键文件 → '查看提交历史'"),
        ("比较版本", "Ctrl+Shift+P → 'Git: Compare with...'"),
        ("恢复文件", "git checkout <commit> -- <filename>"),
        ("时间线视图", "文件资源管理器 → 右键文件 → '时间线'"),
    ]
    
    for desc, cmd in commands:
        print(f"• {desc}:")
        print(f"  {cmd}")
    
    print()

def restore_instructions():
    """恢复文件的说明"""
    print("=" * 60)
    print("如何恢复文件到特定版本")
    print("=" * 60)
    
    print("方法1: 使用Git命令")
    print("  1. 查看提交历史: git log --oneline -- Code_Set/Video_Player.py")
    print("  2. 复制要恢复的提交哈希")
    print("  3. 恢复文件: git checkout <commit_hash> -- Code_Set/Video_Player.py")
    print("  4. 提交恢复: git commit -m '恢复文件到版本 <commit_hash>'")
    
    print("\n方法2: 在VSCode中操作")
    print("  1. 打开源代码管理视图 (Ctrl+Shift+G)")
    print("  2. 右键点击 Video_Player.py")
    print("  3. 选择 '查看提交历史'")
    print("  4. 右键点击要恢复的提交")
    print("  5. 选择 '恢复此提交' 或 '比较提交'")
    
    print("\n方法3: 使用本地历史")
    print("  1. 在编辑器中打开 Video_Player.py")
    print("  2. 右键点击编辑器区域")
    print("  3. 选择 '本地历史记录' → '显示本地历史记录'")
    print("  4. 选择要恢复的版本")
    print("  5. 点击 '恢复' 按钮")

def main():
    """主函数"""
    print("\n" + "=" * 60)
    print("VSCode 文件回溯演示")
    print("=" * 60 + "\n")
    
    # 检查是否在Git仓库中
    if not os.path.exists('.git'):
        print("⚠️  当前目录不是Git仓库")
        print("请先运行: git init")
        return
    
    # 演示各个功能
    print_git_history()
    show_file_versions()
    vscode_commands_demo()
    restore_instructions()
    
    print("\n" + "=" * 60)
    print("下一步操作建议:")
    print("1. 在VSCode中打开 Code_Set/Video_Player.py")
    print("2. 尝试使用本地历史记录功能")
    print("3. 使用源代码管理视图查看Git历史")
    print("4. 安装GitLens扩展获得更强大的功能")
    print("=" * 60)

if __name__ == "__main__":
    main()
