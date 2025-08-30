# dev/UploadRecv.ps1
param(
  [Parameter(Mandatory = $true)] [string] $Port,
  [string] $Fqbn = "arduino:avr:mega",
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
function Wait-ForPort([string] $p, [int] $timeoutSec = 8) {
  $deadline = (Get-Date).AddSeconds($timeoutSec)
  while ((Get-Date) -lt $deadline) {
    $ports = (arduino-cli board list) -join "`n"
    if ($ports -match [regex]::Escape($p)) { return }
    Start-Sleep -Milliseconds 400
  }
}

Require-Cli

$RepoRoot = (Resolve-Path "$PSScriptRoot\..").Path
$SketchDir = Join-Path $RepoRoot "dev\Atmega_Recv"
$OutDir    = Join-Path $RepoRoot "build\Atmega_Recv"

if (-not (Test-Path (Join-Path $SketchDir "Atmega_Recv.ino"))) {
  Write-Host "Missing sketch: $SketchDir\Atmega_Recv.ino" -ForegroundColor Red
  exit 1
}
Ensure-Dir $OutDir

Write-Host "Compiling receiver ($Fqbn)..." -ForegroundColor Cyan
Exec "arduino-cli" @("compile","--fqbn",$Fqbn,"--libraries",$RepoRoot,"--output-dir",$OutDir,$SketchDir)

Write-Host "Uploading to $Port..." -ForegroundColor Cyan
Exec "arduino-cli" @("upload","-p",$Port,"--fqbn",$Fqbn,"--input-dir",$OutDir)

Write-Host "Waiting for $Port to reappear..." -ForegroundColor DarkCyan
Wait-ForPort $Port

Write-Host "Opening serial monitor on $Port @ $Baud. Press Ctrl+C to exit." -ForegroundColor Green
& arduino-cli monitor -p $Port --config "baudrate=$Baud"
