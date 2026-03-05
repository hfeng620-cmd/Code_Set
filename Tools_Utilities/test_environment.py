#!/usr/bin/env python3
"""
环境测试脚本
用于验证Python环境是否按照.clinerules文件的要求正确配置
"""

import sys
import platform
import os
from pathlib import Path


def check_python_environment() -> dict:
    """检查Python环境配置"""
    results = {
        "python_version": platform.python_version(),
        "python_implementation": platform.python_implementation(),
        "python_executable": sys.executable,
        "python_path": sys.path,
        "current_working_directory": os.getcwd(),
        "environment_variables": {
            "PATH": os.environ.get("PATH", ""),
            "PYTHONPATH": os.environ.get("PYTHONPATH", ""),
            "VIRTUAL_ENV": os.environ.get("VIRTUAL_ENV", ""),
        }
    }
    return results


def check_required_packages() -> dict:
    """检查必要的Python包是否已安装"""
    packages = {
        "requests": "网络请求库",
        "beautifulsoup4": "HTML解析库",
        "pandas": "数据处理库",
        "openpyxl": "Excel文件处理",
        "sqlite3": "数据库（Python内置）",
    }
    
    # 包名到导入名的映射（有些包导入名不同）
    import_names = {
        "beautifulsoup4": "bs4",
        "openpyxl": "openpyxl",
        "pandas": "pandas",
        "requests": "requests",
        "sqlite3": "sqlite3"
    }
    
    installed = {}
    for package, description in packages.items():
        try:
            if package == "sqlite3":
                # sqlite3是Python内置模块
                import sqlite3
                version = sqlite3.sqlite_version
                installed[package] = {
                    "status": "已安装（内置）",
                    "version": version,
                    "description": description
                }
            else:
                import_name = import_names.get(package, package)
                module = __import__(import_name)
                version = getattr(module, "__version__", "未知版本")
                installed[package] = {
                    "status": "已安装",
                    "version": version,
                    "description": description
                }
        except ImportError:
            installed[package] = {
                "status": "未安装",
                "version": None,
                "description": description
            }
    
    return installed


def verify_clinerules_compliance() -> dict:
    """验证是否符合.clinerules文件的要求"""
    expected_python_path = r"D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\python.exe"
    expected_pip_path = r"D:\AAA_Learning_Resource\ScreenTracker\venv\Scripts\pip.exe"
    
    compliance = {
        "python_path_match": sys.executable.lower() == expected_python_path.lower(),
        "expected_python_path": expected_python_path,
        "actual_python_path": sys.executable,
        "virtual_env_present": "VIRTUAL_ENV" in os.environ,
        "virtual_env_path": os.environ.get("VIRTUAL_ENV", ""),
        "in_venv": hasattr(sys, 'real_prefix') or (hasattr(sys, 'base_prefix') and sys.base_prefix != sys.prefix),
    }
    
    # 检查pip路径是否存在
    pip_path = Path(expected_pip_path)
    compliance["pip_path_exists"] = pip_path.exists()
    compliance["expected_pip_path"] = str(pip_path)
    
    return compliance


def print_results(env_results: dict, package_results: dict, compliance_results: dict) -> None:
    """打印测试结果"""
    print("=" * 60)
    print("Python环境测试报告")
    print("=" * 60)
    
    print("\n1. Python环境信息:")
    print(f"   Python版本: {env_results['python_version']}")
    print(f"   Python实现: {env_results['python_implementation']}")
    print(f"   Python可执行文件: {env_results['python_executable']}")
    print(f"   当前工作目录: {env_results['current_working_directory']}")
    
    print("\n2. 环境变量:")
    for key, value in env_results['environment_variables'].items():
        if value:
            print(f"   {key}: {value[:100]}..." if len(str(value)) > 100 else f"   {key}: {value}")
    
    print("\n3. 包安装状态:")
    for package, info in package_results.items():
        status_icon = "✅" if info['status'] in ["已安装", "已安装（内置）"] else "❌"
        print(f"   {status_icon} {package}: {info['status']} ({info['description']})")
        if info['version']:
            print(f"       版本: {info['version']}")
    
    print("\n4. .clinerules合规性检查:")
    if compliance_results['python_path_match']:
        print("   ✅ Python路径匹配: 符合要求")
        print(f"      预期路径: {compliance_results['expected_python_path']}")
        print(f"      实际路径: {compliance_results['actual_python_path']}")
    else:
        print("   ❌ Python路径不匹配: 不符合要求")
        print(f"      预期路径: {compliance_results['expected_python_path']}")
        print(f"      实际路径: {compliance_results['actual_python_path']}")
    
    if compliance_results['pip_path_exists']:
        print("   ✅ Pip路径存在: 符合要求")
        print(f"      预期路径: {compliance_results['expected_pip_path']}")
    else:
        print("   ❌ Pip路径不存在: 不符合要求")
        print(f"      预期路径: {compliance_results['expected_pip_path']}")
    
    if compliance_results['in_venv']:
        print("   ✅ 在虚拟环境中运行")
        if compliance_results['virtual_env_path']:
            print(f"      虚拟环境路径: {compliance_results['virtual_env_path']}")
    else:
        print("   ⚠️  不在虚拟环境中运行")
    
    print("\n5. 总体评估:")
    all_packages_installed = all(
        info['status'] in ["已安装", "已安装（内置）"] 
        for info in package_results.values()
    )
    
    if (compliance_results['python_path_match'] and 
        compliance_results['pip_path_exists'] and 
        all_packages_installed):
        print("   ✅ 所有检查通过: 环境配置正确")
        print("   ✅ 符合.clinerules文件的所有要求")
    else:
        print("   ⚠️  部分检查未通过:")
        if not compliance_results['python_path_match']:
            print("      - Python路径不符合要求")
        if not compliance_results['pip_path_exists']:
            print("      - Pip路径不存在")
        if not all_packages_installed:
            missing = [p for p, info in package_results.items() 
                      if info['status'] not in ["已安装", "已安装（内置）"]]
            print(f"      - 缺少包: {', '.join(missing)}")
    
    print("\n" + "=" * 60)


def main() -> None:
    """主函数"""
    print("开始测试Python环境配置...")
    print(f"测试时间: {platform.python_version()} @ {platform.system()} {platform.release()}")
    
    # 执行检查
    env_results = check_python_environment()
    package_results = check_required_packages()
    compliance_results = verify_clinerules_compliance()
    
    # 打印结果
    print_results(env_results, package_results, compliance_results)
    
    # 返回退出代码
    if (compliance_results['python_path_match'] and 
        compliance_results['pip_path_exists']):
        sys.exit(0)  # 成功
    else:
        sys.exit(1)  # 失败


if __name__ == "__main__":
    main()
