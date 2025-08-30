# dev/Setup.ps1
param(
  [switch] $Overwrite,  # force re-create arduino-cli config (uses --overwrite)
  [switch] $Backup      # backup existing arduino-cli.yaml to .bak before modifying
)

$ErrorActionPreference = "Stop"

function Require-Cli {
  if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
    Write-Host "arduino-cli not found on PATH. Install Arduino CLI and try again." -ForegroundColor Red
    exit 1
  }
}

function Get-ConfigPath {
  # Try common OS locations
  if ($env:LOCALAPPDATA) {  # Windows
    return Join-Path $env:LOCALAPPDATA "Arduino15\arduino-cli.yaml"
  } elseif ($env:HOME) {
    $mac = Join-Path $env:HOME "Library/Arduino15/arduino-cli.yaml"
    $linux = Join-Path $env:HOME ".arduino15/arduino-cli.yaml"
    if (Test-Path $mac)   { return $mac }
    else                  { return $linux }
  } else {
    return "arduino-cli.yaml" # fallback (rare)
  }
}

function Update-Indexes-With-Fallback([string] $urls) {
  Write-Host "Updating Boards Manager index (from config)..." -ForegroundColor Cyan
  $ok = $true
  $out = ""
  try {
    $out = arduino-cli core update-index 2>&1 | Out-String
  } catch {
    $ok = $false
  }
  if (-not $ok -or ($out -match "404 Not Found")) {
    Write-Host "Retrying with explicit --additional-urls (avoids quoting/env issues)..." -ForegroundColor Yellow
    arduino-cli core update-index --additional-urls $urls
  }
}

function Ensure-Core([string] $coreId, [string] $urls) {
  if (-not (arduino-cli core list | Select-String -SimpleMatch $coreId)) {
    Write-Host "Installing core: $coreId" -ForegroundColor Cyan
    arduino-cli core install $coreId --additional-urls $urls
  } else {
    Write-Host "Core already installed: $coreId" -ForegroundColor DarkGray
  }
}

Require-Cli

# Warn about a known conflicting library (NINA co-processor flavor)
$conflictLib = Join-Path $env:USERPROFILE "Documents\Arduino\libraries\Bluepad32_for_NINA-W10_boards"
if (Test-Path $conflictLib) {
  Write-Host "Warning: Found '$conflictLib' (NINA W10 Bluepad32). This conflicts with ESP32 Bluepad32 API." -ForegroundColor Yellow
  Write-Host "Consider renaming or removing it to avoid accidental picks." -ForegroundColor Yellow
}

# Target URLs for boards indexes
$urls = "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json,https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json"

# Handle config init safely
$configFile = Get-ConfigPath
if (Test-Path $configFile) {
  if ($Backup) {
    Copy-Item $configFile "$configFile.bak" -Force
    Write-Host "Backed up existing config to: $configFile.bak" -ForegroundColor DarkGray
  }
  if ($Overwrite) {
    Write-Host "Re-initializing arduino-cli config (overwrite)..." -ForegroundColor Yellow
    arduino-cli config init --overwrite | Out-Null
  } else {
    Write-Host "Config exists at: $configFile (keeping it; no --overwrite)" -ForegroundColor DarkGray
  }
} else {
  Write-Host "Initializing arduino-cli config..." -ForegroundColor Cyan
  arduino-cli config init | Out-Null
}

# Persist the two Boards Manager URLs in config (safe even if already set)
arduino-cli config set board_manager.additional_urls $urls | Out-Null

# Update indexes (with 404 fallback)
Update-Indexes-With-Fallback $urls

# Ensure required cores are installed
Ensure-Core "arduino:avr" $urls
Ensure-Core "esp32-bluepad32:esp32" $urls

Write-Host "`nSetup complete." -ForegroundColor Green
arduino-cli core list
