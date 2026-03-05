#!/usr/bin/env python3
"""
VMware Service Manager
自动管理VMware NAT Service和VMware DHCP Service
当检测到vmware.exe运行时启动服务，当vmware.exe关闭时停止服务
"""

import time
import subprocess
import sys
import os
from typing import List, Tuple, Optional
import psutil


class VMwareServiceManager:
    """VMware服务管理器"""
    
    def __init__(self, check_interval: int = 5):
        """
        初始化服务管理器
        
        Args:
            check_interval: 检查间隔（秒）
        """
        self.check_interval = check_interval
        self.vmware_process_name = "vmware.exe"
        self.services_to_manage = [
            "VMware NAT Service",
            "VMnetDHCP"  # VMware DHCP Service的内部名称
        ]
        self.last_vmware_state = False
        
    def is_vmware_running(self) -> bool:
        """
        检查VMware是否正在运行
        
        Returns:
            bool: True如果VMware正在运行
        """
        try:
            for proc in psutil.process_iter(['name']):
                if proc.info['name'] and proc.info['name'].lower() == self.vmware_process_name:
                    return True
            return False
        except Exception as e:
            print(f"检查进程时出错: {e}")
            return False
    
    def get_service_status(self, service_name: str) -> Tuple[bool, Optional[str]]:
        """
        获取服务状态
        
        Args:
            service_name: 服务名称
            
        Returns:
            Tuple[bool, Optional[str]]: (是否正在运行, 状态信息)
        """
        try:
            # 使用PowerShell获取服务状态
            cmd = f'powershell -Command "Get-Service -Name \'{service_name}\' | Select-Object Status"'
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            
            if result.returncode == 0:
                output = result.stdout.strip()
                if "Running" in output:
                    return True, "Running"
                elif "Stopped" in output:
                    return False, "Stopped"
                else:
                    return False, "Unknown"
            else:
                return False, f"Error: {result.stderr}"
                
        except Exception as e:
            return False, f"Exception: {str(e)}"
    
    def start_service(self, service_name: str) -> bool:
        """
        启动服务
        
        Args:
            service_name: 服务名称
            
        Returns:
            bool: True如果启动成功
        """
        try:
            print(f"正在启动服务: {service_name}")
            cmd = f'powershell -Command "Start-Service -Name \'{service_name}\'"'
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            
            if result.returncode == 0:
                print(f"服务 {service_name} 启动成功")
                return True
            else:
                print(f"服务 {service_name} 启动失败: {result.stderr}")
                return False
                
        except Exception as e:
            print(f"启动服务 {service_name} 时出错: {e}")
            return False
    
    def stop_service(self, service_name: str) -> bool:
        """
        停止服务
        
        Args:
            service_name: 服务名称
            
        Returns:
            bool: True如果停止成功
        """
        try:
            print(f"正在停止服务: {service_name}")
            cmd = f'powershell -Command "Stop-Service -Name \'{service_name}\'"'
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            
            if result.returncode == 0:
                print(f"服务 {service_name} 停止成功")
                return True
            else:
                print(f"服务 {service_name} 停止失败: {result.stderr}")
                return False
                
        except Exception as e:
            print(f"停止服务 {service_name} 时出错: {e}")
            return False
    
    def manage_services_based_on_vmware(self, vmware_running: bool) -> None:
        """
        根据VMware状态管理服务
        
        Args:
            vmware_running: VMware是否正在运行
        """
        if vmware_running:
            # VMware正在运行，确保服务启动
            for service in self.services_to_manage:
                is_running, status = self.get_service_status(service)
                if not is_running:
                    self.start_service(service)
                else:
                    print(f"服务 {service} 已在运行中")
        else:
            # VMware未运行，停止服务
            for service in self.services_to_manage:
                is_running, status = self.get_service_status(service)
                if is_running:
                    self.stop_service(service)
                else:
                    print(f"服务 {service} 已停止")
    
    def run_monitor(self) -> None:
        """运行监控循环"""
        print("VMware服务监控器已启动")
        print(f"监控的服务: {', '.join(self.services_to_manage)}")
        print(f"检查间隔: {self.check_interval}秒")
        print("按 Ctrl+C 停止监控\n")
        
        try:
            while True:
                vmware_running = self.is_vmware_running()
                
                # 只在状态变化时执行操作
                if vmware_running != self.last_vmware_state:
                    print(f"\nVMware状态变化: {'运行中' if vmware_running else '已停止'}")
                    self.manage_services_based_on_vmware(vmware_running)
                    self.last_vmware_state = vmware_running
                
                # 显示当前状态
                if vmware_running:
                    print(f"✓ VMware正在运行 - 服务已确保启动", end='\r')
                else:
                    print(f"✗ VMware未运行 - 服务已确保停止", end='\r')
                
                time.sleep(self.check_interval)
                
        except KeyboardInterrupt:
            print("\n\n监控已停止")
        except Exception as e:
            print(f"\n监控出错: {e}")
            sys.exit(1)


def create_desktop_shortcut() -> None:
    """在桌面创建快捷方式"""
    try:
        desktop_path = os.path.join(os.path.expanduser("~"), "Desktop")
        shortcut_path = os.path.join(desktop_path, "VMware服务管理器.lnk")
        script_path = os.path.abspath(__file__)
        working_dir = os.path.dirname(script_path)
        
        # 使用PowerShell命令直接创建快捷方式
        ps_command = f"""
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut('{shortcut_path}')
$Shortcut.TargetPath = 'pythonw.exe'
$Shortcut.Arguments = '{script_path}'
$Shortcut.WorkingDirectory = '{working_dir}'
$Shortcut.Description = 'VMware服务自动管理器'
$Shortcut.IconLocation = 'shell32.dll,21'
$Shortcut.Save()
"""
        
        # 直接执行PowerShell命令
        cmd = f'powershell -Command "{ps_command}"'
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        
        if result.returncode == 0:
            print(f"快捷方式已创建到: {shortcut_path}")
        else:
            print(f"创建快捷方式失败: {result.stderr}")
        
    except Exception as e:
        print(f"创建快捷方式时出错: {e}")


def main() -> None:
    """主函数"""
    import argparse
    
    parser = argparse.ArgumentParser(description="VMware服务管理器")
    parser.add_argument("--install", action="store_true", help="安装并创建桌面快捷方式")
    parser.add_argument("--interval", type=int, default=5, help="检查间隔（秒）")
    parser.add_argument("--once", action="store_true", help="只执行一次检查和管理")
    
    args = parser.parse_args()
    
    if args.install:
        create_desktop_shortcut()
        print("安装完成！")
        return
    
    manager = VMwareServiceManager(check_interval=args.interval)
    
    if args.once:
        vmware_running = manager.is_vmware_running()
        print(f"VMware状态: {'运行中' if vmware_running else '已停止'}")
        manager.manage_services_based_on_vmware(vmware_running)
    else:
        manager.run_monitor()


if __name__ == "__main__":
    main()