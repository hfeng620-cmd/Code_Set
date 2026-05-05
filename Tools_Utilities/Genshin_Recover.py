import os
import shutil
import subprocess

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

def get_path(filename):
    return os.path.join(BASE_DIR, filename)

def safe_switch(target_mode):
    config_path = get_path('config.ini')
    backup_path = get_path('config.ini.bak')

    # 1. 自动备份原始配置
    if not os.path.exists(backup_path) and os.path.exists(config_path):
        shutil.copy(config_path, backup_path)
        print("💡 已为您自动备份原始国服配置。")

    # 2. 修改配置
    if target_mode == 'global':
        # 国际服参数
        new_config = {
            'channel': '1',
            'cps': 'hoyoverse',
            'sub_channel': '1'
        }
        rename_map = [('YuanShen.exe', 'GenshinImpact.exe'), ('YuanShen_Data', 'GenshinImpact_Data')]
    else:
        # 国服参数（如果存在备份，优先从备份恢复）
        if os.path.exists(backup_path):
            shutil.copy(backup_path, config_path)
            print("🔄 已从备份恢复原始国服配置。")
        new_config = {
            'channel': '1',
            'cps': 'mihoyo',
            'sub_channel': '1'
        }
        rename_map = [('GenshinImpact.exe', 'YuanShen.exe'), ('GenshinImpact_Data', 'YuanShen_Data')]

    # 3. 执行文件重命名
    for old, new in rename_map:
        if os.path.exists(get_path(old)):
            try:
                os.rename(get_path(old), get_path(new))
                print(f"✅ 执行: {old} -> {new}")
            except Exception as e:
                print(f"❌ 失败: {e} (请关闭游戏后再试)")

    # 4. 更新 config.ini
    if target_mode == 'global':
        with open(config_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        with open(config_path, 'w', encoding='utf-8') as f:
            for line in lines:
                if any(line.startswith(k + '=') for k in new_config):
                    key = line.split('=')[0]
                    f.write(f"{key}={new_config[key]}\n")
                else:
                    f.write(line)
    
    print(f"\n✨ 已成功切换到 {'国际服' if target_mode == 'global' else '国服'}！")

def main():
    print("="*40)
    print("   原神双服一键切换器 (带备份安全版)")
    print(f"   路径: {BASE_DIR}")
    print("="*40)
    print(" 1. 切换到【国际服】")
    print(" 2. 还原回【国服】 (含配置还原)")
    
    choice = input("\n请输入数字: ")
    if choice == '1':
        safe_switch('global')
    elif choice == '2':
        safe_switch('cn')
    
    input("\n按回车键完成...")

if __name__ == "__main__":
    main()