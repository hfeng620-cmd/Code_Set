使用说明（Windows 10）

文件位置：
D:\VS_Code\Code_Set\AudioControlHotkeys

包含文件：
- AudioHotkeys.ahk    快捷键脚本（AutoHotkey v2）
- SetAppVolume.ps1   按进程调整音量（PowerShell + CoreAudio）
- music_up.cmd       音乐组音量 +2
- music_down.cmd     音乐组音量 -2
- music_mute.cmd     音乐组静音切换
- game_up.cmd        游戏组音量 +2
- game_down.cmd      游戏组音量 -2
- game_mute.cmd      游戏组静音切换

功能：
1) NumLock 关闭时，数字小键盘（Numpad0-9）会执行“播放/暂停”
2) Launch_Media 键（若键盘发送这个键）改为“播放/暂停”
3) 分别调节“音乐”和“游戏”程序的音量

快捷键：
- 音乐组：Ctrl + Alt + Numpad+ / Numpad-  (音量 +/-)
- 音乐组：Ctrl + Alt + Numpad*          (静音切换)
- 游戏组：Alt + Shift + Numpad+ / Numpad- (音量 +/-)
- 游戏组：Alt + Shift + Numpad*          (静音切换)

如果你不想用快捷键，也可以直接双击这些 cmd：
- music_up.cmd / music_down.cmd / music_mute.cmd
- game_up.cmd / game_down.cmd / game_mute.cmd

配置要点：
1) 打开 SetAppVolume.ps1，修改 $AppGroups 里的进程名（xxx.exe）
   - music 组：填你常用的音乐播放器进程名
   - game 组：填你正在玩的游戏进程名
2) 音量步进（默认 2%）可在 AudioHotkeys.ahk 里改

运行方法：
1) 安装 AutoHotkey v2（需要 v2.0）
2) 双击运行 AudioHotkeys.ahk
3) 如需开机自启，把 AudioHotkeys.ahk 放进：
   Win + R 输入：shell:startup

提示：
- 如果某个程序没响应，通常是进程名没写对
- 有些播放器会开多个进程，必要时把相关进程名都加入列表
