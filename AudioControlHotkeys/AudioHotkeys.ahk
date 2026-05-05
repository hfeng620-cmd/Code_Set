#Requires AutoHotkey v2.0

; ========================
; Audio + Media Hotkeys
; ========================
; Behavior:
; - When NumLock is OFF, any numpad digit will Play/Pause media
; - Also remap Launch_Media key to Play/Pause (if your keyboard sends it)
; - Hotkeys to adjust MUSIC and GAME volumes separately (per-app sessions)

; ===== Paths =====
psPath := "D:\\VS_Code\\Code_Set\\AudioControlHotkeys\\SetAppVolume.ps1"

; ===== Helper =====
AdjustGroup(group, action, step := 2) {
    global psPath
    cmd := "powershell.exe -NoProfile -ExecutionPolicy Bypass -File `"" psPath "`" -Group " group " -Action " action " -Step " step
    RunWait(cmd, , "Hide")
}

; ===== Media Play/Pause =====
Launch_Media::Send "{Media_Play_Pause}"

#HotIf !GetKeyState("NumLock", "T")
Numpad0::Send "{Media_Play_Pause}"
Numpad1::Send "{Media_Play_Pause}"
Numpad2::Send "{Media_Play_Pause}"
Numpad3::Send "{Media_Play_Pause}"
Numpad4::Send "{Media_Play_Pause}"
Numpad5::Send "{Media_Play_Pause}"
Numpad6::Send "{Media_Play_Pause}"
Numpad7::Send "{Media_Play_Pause}"
Numpad8::Send "{Media_Play_Pause}"
Numpad9::Send "{Media_Play_Pause}"
#HotIf

; ===== Volume controls =====
; MUSIC group: Ctrl + Alt + NumpadAdd/Sub
^!NumpadAdd::AdjustGroup("music", "up", 2)
^!NumpadSub::AdjustGroup("music", "down", 2)
^!NumpadMult::AdjustGroup("music", "togglemute", 2)

; GAME group: Alt + Shift + NumpadAdd/Sub
!+NumpadAdd::AdjustGroup("game", "up", 2)
!+NumpadSub::AdjustGroup("game", "down", 2)
!+NumpadMult::AdjustGroup("game", "togglemute", 2)
