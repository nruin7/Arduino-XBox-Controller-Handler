# dev/ListPorts.ps1
param()

$ErrorActionPreference = "Stop"
if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
  Write-Host "arduino-cli not found on PATH." -ForegroundColor Red
  exit 1
}

arduino-cli board list
