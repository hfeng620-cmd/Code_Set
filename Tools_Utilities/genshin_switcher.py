import os
import sys
import subprocess

# 获取脚本当前所在的绝对路径
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

def get_path(filename):
    return os.path.join(BASE_DIR, filename)

def check_env():
    """检查是否放在了正确的游戏目录下"""
    # 检查是否存在国服或国际服的执行文件
    if not (os.path.exists(get_path('YuanShen.exe')) or os.path.exists(get_path('GenshinImpact.exe'))):
        print("❌ 错误：脚本位置放错啦！")
        print(f"当前位置: {BASE_DIR}")
        print("请将此脚本剪切到包含 'YuanShen.exe' 或 'config.ini' 的文件夹内再运行。")
        input("\n按回车键退出...")
        return False
    return True

def modify_config(target_type):
    config_path = get_path('config.ini')
    configs = {
        'global': {'channel': '1', 'cps': 'hoyoverse', 'sub_channel': '1'},
        'cn': {'channel': '1', 'cps': 'mihoyo', 'sub_channel': '1'}
    }
    
    target = configs[target_type]
    if not os.path.exists(config_path):
        print(f"❌ 找不到 config.ini")
        return False
    
    with open(config_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        
    with open(config_path, 'w', encoding='utf-8') as f:
        for line in lines:
            if line.startswith('channel='):
                f.write(f"channel={target['channel']}\n")
            elif line.startswith('cps='):
                f.write(f"cps={target['cps']}\n")
            elif line.startswith('sub_channel='):
                f.write(f"sub_channel={target['sub_channel']}\n")
            else:
                f.write(line)
    return True

def rename_files(to_server):
    if to_server == 'global':
        targets = [('YuanShen.exe', 'GenshinImpact.exe'), ('YuanShen_Data', 'GenshinImpact_Data')]
    else:
        targets = [('GenshinImpact.exe', 'YuanShen.exe'), ('GenshinImpact_Data', 'YuanShen_Data')]
    
    for old, new in targets:
        old_path, new_path = get_path(old), get_path(new)
        if os.path.exists(old_path):
            try:
                os.rename(old_path, new_path)
                print(f"✅ 已转换: {old} -> {new}")
            except Exception as e:
                print(f"❌ 转换失败 (请先关闭游戏和启动器): {e}")

def main():
    if not check_env():
        return

    os.system('cls' if os.name == 'nt' else 'clear')
    print("="*45)
    print("      原神区服一键转换工具 (自动路径版)")
    print(f"  当前位置: {BASE_DIR}")
    print("="*45)
    print(" 1. 切换到【国际服】 (Global)")
    print(" 2. 切换到【国服】   (CN)")
    print("="*45)
    
    choice = input("\n请输入数字 (1/2): ")
    
    if choice in ['1', '2']:
        mode = 'global' if choice == '1' else 'cn'
        print(f"\n[任务] 正在准备切换至 {'国际服' if mode == 'global' else '国服'}...")
        
        if modify_config(mode):
            rename_files(mode)
            print("\n✨ 操作成功！")
            confirm = input("是否立即启动游戏本体？(y/n): ")
            if confirm.lower() == 'y':
                exe = 'GenshinImpact.exe' if mode == 'global' else 'YuanShen.exe'
                subprocess.Popen(get_path(exe), cwd=BASE_DIR)
    else:
        print("无效选择，程序退出。")

if __name__ == "__main__":
    main()