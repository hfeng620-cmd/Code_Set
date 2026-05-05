import winreg
import ctypes

def remove_startup():
    key_path = r"Software\Microsoft\Windows\CurrentVersion\Run"
    app_name = "MyScreenTracker" # 这是旧代码中使用的注册表名称

    try:
        # 打开注册表
        key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, key_path, 0, winreg.KEY_ALL_ACCESS)
        
        # 尝试删除键值
        winreg.DeleteValue(key, app_name)
        winreg.CloseKey(key)
        
        ctypes.windll.user32.MessageBoxW(0, "成功：已清理旧版本的开机自启设置！", "清理完成", 0)
        print("清理成功")
        
    except FileNotFoundError:
        ctypes.windll.user32.MessageBoxW(0, "未发现旧版本的开机自启设置，无需清理。", "提示", 0)
        print("未发现设置，无需清理")
    except Exception as e:
        ctypes.windll.user32.MessageBoxW(0, f"清理失败: {e}", "错误", 0)
        print(f"清理失败: {e}")

if __name__ == "__main__":
    remove_startup()