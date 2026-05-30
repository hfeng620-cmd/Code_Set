param(
    [int]$Port
)

$ErrorActionPreference = "Stop"
Set-Location -LiteralPath $PSScriptRoot

if (-not (Get-Command node -ErrorAction SilentlyContinue)) {
    Write-Host "Node.js was not found in PATH."
    Write-Host "Install Node.js first, then run this script again."
    exit 1
}

if (-not (Test-Path -LiteralPath ".\server.js")) {
    Write-Host "server.js was not found in:"
    Write-Host (Get-Location).Path
    exit 1
}

if ($PSBoundParameters.ContainsKey("Port")) {
    $env:PORT = [string]$Port
} elseif ([string]::IsNullOrWhiteSpace($env:PORT)) {
    $env:PORT = "8787"
}

Write-Host "Starting Talk with yourself on http://localhost:$env:PORT"
Write-Host "Press Ctrl+C to stop the server."
Write-Host ""

node server.js
exit $LASTEXITCODE
