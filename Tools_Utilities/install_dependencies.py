#!/usr/bin/env python
"""
机器学习评价指标脚本依赖安装程序
"""

import sys
import subprocess
import importlib

def check_and_install():
    """检查并安装必要的依赖库"""
    
    required_packages = {
        'pandas': 'pandas',
        'numpy': 'numpy',
        'sklearn': 'scikit-learn'
    }
    
    print("正在检查依赖库...")
    print("-" * 50)
    
    missing_packages = []
    
    # 检查已安装的库
    for module_name, package_name in required_packages.items():
        try:
            importlib.import_module(module_name if module_name != 'sklearn' else 'sklearn')
            print(f"✓ {package_name} 已安装")
        except ImportError:
            print(f"✗ {package_name} 未安装")
            missing_packages.append(package_name)
    
    # 如果有缺失的库，询问是否安装
    if missing_packages:
        print(f"\n发现 {len(missing_packages)} 个未安装的库:")
        for pkg in missing_packages:
            print(f"  - {pkg}")
        
        choice = input("\n是否要自动安装这些库？ (y/n): ").strip().lower()
        
        if choice in ['y', 'yes', '是']:
            print("\n开始安装依赖库...")
            for pkg in missing_packages:
                print(f"\n正在安装 {pkg}...")
                try:
                    # 使用 pip 安装
                    subprocess.check_call([sys.executable, '-m', 'pip', 'install', pkg])
                    print(f"✓ {pkg} 安装成功")
                except subprocess.CalledProcessError as e:
                    print(f"✗ {pkg} 安装失败: {e}")
                    print("请尝试手动安装:")
                    print(f"  pip install {pkg}")
        else:
            print("\n请手动安装缺失的库:")
            for pkg in missing_packages:
                print(f"  pip install {pkg}")
    else:
        print("\n✓ 所有依赖库已安装！")
    
    print("\n" + "=" * 50)
    print("安装完成！现在可以运行生成脚本了。")
    print("运行以下命令生成评价指标对照表：")
    print("  python generate_ml_metrics.py")

def manual_installation_guide():
    """显示手动安装指南"""
    
    print("=" * 60)
    print("手动安装指南")
    print("=" * 60)
    print("\n1. Windows 用户:")
    print("   打开命令提示符或 PowerShell，运行:")
    print("   pip install pandas numpy scikit-learn")
    
    print("\n2. macOS/Linux 用户:")
    print("   打开终端，运行:")
    print("   pip3 install pandas numpy scikit-learn")
    
    print("\n3. 如果遇到权限问题，尝试:")
    print("   pip install --user pandas numpy scikit-learn")
    
    print("\n4. 使用清华镜像加速（国内用户推荐）:")
    print("   pip install pandas numpy scikit-learn -i https://pypi.tuna.tsinghua.edu.cn/simple")
    
    print("\n5. 验证安装:")
    print("   运行 python，然后输入:")
    print("   import pandas as pd")
    print("   import numpy as np")
    print("   from sklearn import metrics")
    print("   如果没有报错，说明安装成功！")

def check_installation():
    """验证安装是否成功"""
    
    print("\n" + "=" * 50)
    print("验证安装结果")
    print("=" * 50)
    
    tests = [
        ("pandas", "pd.DataFrame([{'a': 1}])"),
        ("numpy", "np.array([1, 2, 3])"),
        ("sklearn.metrics", "accuracy_score([1,0,1], [1,0,1])")
    ]
    
    all_pass = True
    
    for lib, code in tests:
        try:
            if lib == "pandas":
                import pandas as pd
                exec(f"result = {code}")
                print(f"✓ {lib} 测试通过")
            elif lib == "numpy":
                import numpy as np
                exec(f"result = {code}")
                print(f"✓ {lib} 测试通过")
            elif lib == "sklearn.metrics":
                from sklearn.metrics import accuracy_score
                result = accuracy_score([1, 0, 1], [1, 0, 1])
                print(f"✓ {lib} 测试通过")
        except Exception as e:
            print(f"✗ {lib} 测试失败: {e}")
            all_pass = False
    
    return all_pass

if __name__ == "__main__":
    print("机器学习评价指标脚本依赖安装程序")
    print("=" * 50)
    
    # 显示菜单
    print("\n请选择操作:")
    print("1. 自动检查并安装依赖")
    print("2. 查看手动安装指南")
    print("3. 验证当前安装状态")
    print("4. 退出")
    
    choice = input("\n请输入选项 (1-4): ").strip()
    
    if choice == "1":
        check_and_install()
    elif choice == "2":
        manual_installation_guide()
    elif choice == "3":
        if check_installation():
            print("\n✓ 所有库安装正确！")
        else:
            print("\n✗ 有些库未正确安装，请重新安装。")
    elif choice == "4":
        print("退出程序")
    else:
        print("无效选项，请重新运行程序。")
