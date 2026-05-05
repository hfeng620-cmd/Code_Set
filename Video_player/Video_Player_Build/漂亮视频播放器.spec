# -*- mode: python ; coding: utf-8 -*-


a = Analysis(
    ['D:\\VS_Code\\Code_Set\\Video_Player_Source\\Video_Player_Perfect.py'],
    pathex=[],
    binaries=[],
    datas=[('D:\\VS_Code\\Code_Set\\Video_player\\Video_Player_Resources\\favicon.ico', 'Video_Player_Resources'), ('D:\\VS_Code\\Code_Set\\Video_player\\Video_Player_Resources\\nap_cn.ico', 'Video_Player_Resources')],
    hiddenimports=['PyQt5', 'PyQt5.QtCore', 'PyQt5.QtWidgets', 'PyQt5.QtGui', 'PyQt5.QtMultimedia', 'PyQt5.QtMultimediaWidgets'],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='漂亮视频播放器',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=['D:\\VS_Code\\Code_Set\\Video_player\\Video_Player_Resources\\favicon.ico'],
)
