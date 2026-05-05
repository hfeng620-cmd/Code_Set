使用说明（UI 音量混音器）

路径：
D:\VS_Code\Code_Set\AudioControlHotkeys\VolumeMixerUI

文件：
- volume_mixer_ui.py

安装依赖（首次运行需要）：
1) 打开 PowerShell
2) 执行：
   python -m pip install --upgrade pip
   python -m pip install pycaw comtypes

运行：
python volume_mixer_ui.py

说明：
- 每个正在发声的软件都会出现一个滑块
- 直接拖动滑块即可单独调整该软件音量
- Mute 可单独静音某个软件
- Refresh 可刷新列表

常见问题：
- 看不到某个软件：它没有在播放声音时不会出现
- 需要管理员权限：如果无法调节某些软件，尝试用“以管理员身份运行 PowerShell”再启动
