# dev/UploadSnd.ps1
param(
  [Parameter(Mandatory = $true)] [string] $Port,
  [string] $Fqbn = "esp32-bluepad32:esp32:esp32c3",
  [int]    $Baud = 115200
)

$ErrorActionPreference = "Stop"

function Require-Cli {
  if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
    Write-Host "arduino-cli not found on PATH." -ForegroundColor Red
    exit 1
  }
}
function Ensure-Dir([string] $p) { New-Item -ItemType Directory -Force -Path $p | Out-Null }
function Exec($file, [string[]] $ArgList) {
  & $file @ArgList
  if ($LASTEXITCODE -ne 0) {
    throw "Command failed: $file $($ArgList -join ' ')"
  }
}
function Wait-ForPort([string] $p, [int] $timeoutSec = 12) {
  $deadline = (Get-Date).AddSeconds($timeoutSec)
  while ((Get-Date) -lt $deadline) {
    $ports = (arduino-cli board list) -join "`n"
    if ($ports -match [regex]::Escape($p)) { return }
    Start-Sleep -Milliseconds 500
  }
}

Require-Cli

$RepoRoot = (Resolve-Path "$PSScriptRoot\..").Path
$SketchDir = Join-Path $RepoRoot "dev\ESP32_Snd"
$OutDir    = Join-Path $RepoRoot "build\ESP32_Snd"

if (-not (Test-Path (Join-Path $SketchDir "ESP32_Snd.ino"))) {
  Write-Host "Missing sketch: $SketchDir\ESP32_Snd.ino" -ForegroundColor Red
  exit 1
}
Ensure-Dir $OutDir

Write-Host "Compiling sender ($Fqbn)..." -ForegroundColor Cyan
Exec "arduino-cli" @("compile","--fqbn",$Fqbn,"--libraries",$RepoRoot,"--output-dir",$OutDir,$SketchDir)

Write-Host "Uploading to $Port..." -ForegroundColor Cyan
Exec "arduino-cli" @("upload","-p",$Port,"--fqbn",$Fqbn,"--input-dir",$OutDir)

Write-Host "Waiting for $Port to reappear..." -ForegroundColor DarkCyan
Wait-ForPort $Port

Write-Host "Opening serial monitor on $Port @ $Baud. Press Ctrl+C to exit." -ForegroundColor Green
& arduino-cli monitor -p $Port --config "baudrate=$Baud"
