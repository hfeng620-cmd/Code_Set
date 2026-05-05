param(
    [Parameter(Mandatory=$true)]
    [ValidateSet(''music'',''game'')]
    [string]$Group,

    [ValidateSet(''up'',''down'',''set'',''mute'',''unmute'',''togglemute'')]
    [string]$Action = ''up'',

    [int]$Step = 2,

    [int]$SetVolume = 50
)

# Edit these process names to match your apps (lower/upper case doesn't matter)
$AppGroups = @{
    music = @(
        ''Spotify.exe'',
        ''MusicBee.exe'',
        ''AIMP.exe'',
        ''foobar2000.exe'',
        ''iTunes.exe'',
        ''QQMusic.exe'',
        ''CloudMusic.exe'',
        ''cloudmusic.exe'',
        ''cloudmusic_report.exe'',
        ''zen.exe'',
        ''chrome.exe''
    )
    game = @(
        ''Steam.exe'',
        ''steamwebhelper.exe'',
        ''Game.exe'',
        ''Unity.exe'',
        ''UE4Editor.exe'',
        ''UE5Editor.exe'',
        ''GenshinImpact.exe'',
        ''YuanShen.exe''
    )
}

$targets = $AppGroups[$Group]
if (-not $targets -or $targets.Count -eq 0) {
    Write-Error "No process names configured for group ''$Group''."
    exit 1
}

$code = @"
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Diagnostics;

namespace CoreAudioSession {
    public enum EDataFlow { eRender, eCapture, eAll }
    public enum ERole { eConsole, eMultimedia, eCommunications }

    [Guid("A95664D2-9614-4F35-A746-DE8DB63617E6"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IMMDeviceEnumerator {
        int NotImpl1();
        int GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role, out IMMDevice ppDevice);
    }

    [Guid("D666063F-1587-4E43-81F1-B948E807363F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IMMDevice {
        int Activate(ref Guid iid, int dwClsCtx, IntPtr pActivationParams, [MarshalAs(UnmanagedType.IUnknown)] out object ppInterface);
    }

    [Guid("77AA99A0-1BD6-484F-8BC7-2C654C9A9B6F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IAudioSessionManager2 {
        int NotImpl1();
        int NotImpl2();
        int GetSessionEnumerator(out IAudioSessionEnumerator SessionEnum);
    }

    [Guid("E2F5BB11-0570-40CA-ACDD-3AA01277DEE8"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IAudioSessionEnumerator {
        int GetCount(out int SessionCount);
        int GetSession(int SessionCount, out IAudioSessionControl Session);
    }

    [Guid("F4B1A599-7266-4319-A8CA-E70ACB11E8CD"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IAudioSessionControl {
        int NotImpl1();
        int NotImpl2();
        int GetState(out int pRetVal);
        int NotImpl3();
        int NotImpl4();
        int NotImpl5();
        int NotImpl6();
    }

    [Guid("BFB7FF88-7239-4FC9-8FA2-07C950BE9C6D"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IAudioSessionControl2 {
        int NotImpl1();
        int NotImpl2();
        int GetState(out int pRetVal);
        int NotImpl3();
        int NotImpl4();
        int NotImpl5();
        int NotImpl6();
        int GetSessionIdentifier([MarshalAs(UnmanagedType.LPWStr)] out string pRetVal);
        int GetSessionInstanceIdentifier([MarshalAs(UnmanagedType.LPWStr)] out string pRetVal);
        int GetProcessId(out int pRetVal);
        int IsSystemSoundsSession();
        int SetDuckingPreference(bool optOut);
    }

    [Guid("87CE5498-68D6-44E5-9215-6DA47EF883D8"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface ISimpleAudioVolume {
        int SetMasterVolume(float fLevel, ref Guid EventContext);
        int GetMasterVolume(out float pfLevel);
        int SetMute(bool bMute, ref Guid EventContext);
        int GetMute(out bool pbMute);
    }

    [ComImport, Guid("BCDE0395-E52F-467C-8E3D-C4579291692E")]
    class MMDeviceEnumeratorComObject { }

    public static class SessionVolume {
        private const int CLSCTX_ALL = 23;

        public static int Adjust(string[] processNames, float delta, bool useDelta, float setValue, bool useSet, int muteMode) {
            var targets = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            foreach (var p in processNames) targets.Add(p);

            var enumerator = new MMDeviceEnumeratorComObject() as IMMDeviceEnumerator;
            if (enumerator == null) return 0;

            IMMDevice device;
            enumerator.GetDefaultAudioEndpoint(EDataFlow.eRender, ERole.eMultimedia, out device);
            if (device == null) return 0;

            Guid iid = typeof(IAudioSessionManager2).GUID;
            object obj;
            device.Activate(ref iid, CLSCTX_ALL, IntPtr.Zero, out obj);
            var mgr = obj as IAudioSessionManager2;
            if (mgr == null) return 0;

            IAudioSessionEnumerator sessionEnum;
            mgr.GetSessionEnumerator(out sessionEnum);
            if (sessionEnum == null) return 0;

            int count;
            sessionEnum.GetCount(out count);
            int hits = 0;

            for (int i = 0; i < count; i++) {
                IAudioSessionControl ctl;
                sessionEnum.GetSession(i, out ctl);
                if (ctl == null) continue;

                var ctl2 = ctl as IAudioSessionControl2;
                if (ctl2 == null) continue;

                int pid;
                ctl2.GetProcessId(out pid);
                if (pid <= 0) continue;

                string pname;
                try {
                    pname = Process.GetProcessById(pid).ProcessName + ".exe";
                } catch {
                    continue;
                }

                if (!targets.Contains(pname)) continue;

                var vol = ctl as ISimpleAudioVolume;
                if (vol == null) continue;

                float current;
                vol.GetMasterVolume(out current);
                float next = current;

                if (useSet) {
                    next = Clamp(setValue);
                } else if (useDelta) {
                    next = Clamp(current + delta);
                }

                Guid ctx = Guid.Empty;

                if (useSet || useDelta) {
                    vol.SetMasterVolume(next, ref ctx);
                }

                if (muteMode != 0) {
                    bool mute;
                    if (muteMode > 0) mute = true;
                    else if (muteMode < 0) mute = false;
                    else mute = false;
                    if (muteMode == 2) {
                        bool curMute;
                        vol.GetMute(out curMute);
                        mute = !curMute;
                    }
                    vol.SetMute(mute, ref ctx);
                }

                hits++;
            }

            return hits;
        }

        private static float Clamp(float v) {
            if (v < 0f) return 0f;
            if (v > 1f) return 1f;
            return v;
        }
    }
}
"@

Add-Type -TypeDefinition $code -Language CSharp -ErrorAction Stop | Out-Null

$delta = [Math]::Abs($Step) / 100.0
$useDelta = $true
$useSet = $false
$setValue = [Math]::Max(0, [Math]::Min(100, $SetVolume)) / 100.0

$muteMode = 0
switch ($Action) {
    'up' { $delta = +$delta; $useDelta = $true }
    'down' { $delta = -$delta; $useDelta = $true }
    'set' { $useSet = $true; $useDelta = $false }
    'mute' { $muteMode = 1 }
    'unmute' { $muteMode = -1 }
    'togglemute' { $muteMode = 2 }
}

$hits = [CoreAudioSession.SessionVolume]::Adjust($targets, $delta, $useDelta, $setValue, $useSet, $muteMode)
if ($hits -eq 0) {
    Write-Output "No matching audio sessions found for group '$Group'."
}
