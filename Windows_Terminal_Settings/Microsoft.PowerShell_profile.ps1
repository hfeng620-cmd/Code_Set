$ErrorActionPreference = "SilentlyContinue"

Set-PSReadLineOption -PredictionSource History
Set-PSReadLineOption -PredictionViewStyle ListView
Set-PSReadLineOption -EditMode Windows
Set-PSReadLineOption -BellStyle None

Set-PSReadLineKeyHandler -Key Tab -Function MenuComplete
Set-PSReadLineKeyHandler -Key UpArrow -Function HistorySearchBackward
Set-PSReadLineKeyHandler -Key DownArrow -Function HistorySearchForward

function ll {
    Get-ChildItem -Force
}

function la {
    Get-ChildItem -Force -Hidden
}

function grep {
    param(
        [Parameter(Mandatory = $true, Position = 0)]
        [string]$Pattern,
        [Parameter(Position = 1)]
        [string]$Path = "."
    )

    Get-ChildItem -Path $Path -Recurse -File | Select-String -Pattern $Pattern
}

function Sync-ProxyEnvFromSystemProxy {
    if ($env:HTTP_PROXY -or $env:HTTPS_PROXY -or $env:ALL_PROXY) {
        return
    }

    $settings = Get-ItemProperty "HKCU:\Software\Microsoft\Windows\CurrentVersion\Internet Settings"
    if (-not $settings -or $settings.ProxyEnable -ne 1 -or [string]::IsNullOrWhiteSpace($settings.ProxyServer)) {
        return
    }

    $proxyAddress = $null
    $proxyServer = [string]$settings.ProxyServer

    if ($proxyServer -match "=") {
        $proxyMap = @{}
        foreach ($entry in ($proxyServer -split ";")) {
            if ($entry -match "^(?<scheme>[^=]+)=(?<address>.+)$") {
                $proxyMap[$matches.scheme.ToLowerInvariant()] = $matches.address
            }
        }

        if ($proxyMap.ContainsKey("https")) {
            $proxyAddress = $proxyMap["https"]
        } elseif ($proxyMap.ContainsKey("http")) {
            $proxyAddress = $proxyMap["http"]
        } elseif ($proxyMap.Count -gt 0) {
            $proxyAddress = ($proxyMap.GetEnumerator() | Select-Object -First 1).Value
        }
    } else {
        $proxyAddress = $proxyServer
    }

    if ([string]::IsNullOrWhiteSpace($proxyAddress)) {
        return
    }

    if ($proxyAddress -notmatch "^[a-z]+://") {
        $proxyAddress = "http://$proxyAddress"
    }

    $env:HTTP_PROXY = $proxyAddress
    $env:HTTPS_PROXY = $proxyAddress
    $env:ALL_PROXY = $proxyAddress

    if (-not [string]::IsNullOrWhiteSpace($settings.ProxyOverride)) {
        $noProxy = ($settings.ProxyOverride -split ";" | Where-Object { $_ -and $_ -ne "<local>" }) -join ","
        if ($noProxy) {
            $env:NO_PROXY = $noProxy
        }
    }
}

Sync-ProxyEnvFromSystemProxy

if (Get-Command oh-my-posh -ErrorAction SilentlyContinue) {
    oh-my-posh init pwsh --config "D:\OneDrive\文档\AutoHotkey\terminal\neon-synthwave.omp.json" | Invoke-Expression
} elseif (Test-Path "C:\Users\Administrator\AppData\Local\Programs\oh-my-posh\bin\oh-my-posh.exe") {
    & "C:\Users\Administrator\AppData\Local\Programs\oh-my-posh\bin\oh-my-posh.exe" init pwsh --config "D:\OneDrive\文档\AutoHotkey\terminal\neon-synthwave.omp.json" | Invoke-Expression
} else {
    function prompt {
        $path = $executionContext.SessionState.Path.CurrentLocation
        "PS $path$('>' * ($nestedPromptLevel + 1)) "
    }
}

$env:TERM_PROGRAM = "Windows Terminal"

