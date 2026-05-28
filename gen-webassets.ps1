# =============================================================================
# Huyang Web-Assets Generator
# Liest alle data/*.html, *.css, *.js und schreibt sie als PROGMEM-Konstanten
# in WebAssets.h. Damit muss kein LittleFS mehr geflasht werden.
#
# Verwendung: aus dem Sketch-Ordner:
#   powershell -ExecutionPolicy Bypass -File gen-webassets.ps1
# =============================================================================

$root = $PSScriptRoot
$dataDir = Join-Path $root "data"
$outFile = Join-Path $root "WebAssets.h"

if (-not (Test-Path $dataDir)) {
    Write-Host "FEHLER: data/ Verzeichnis nicht gefunden in $root" -ForegroundColor Red
    exit 1
}

# Map: Dateiname -> C++ Konstantenname
$mapping = @{
    "base.html"           = "WEB_BASE_HTML"
    "index.html"          = "WEB_INDEX_HTML"
    "index.face.html"     = "WEB_FACE_HTML"
    "index.neck.html"     = "WEB_NECK_HTML"
    "index.body.html"     = "WEB_BODY_HTML"
    "index.audio.html"    = "WEB_AUDIO_HTML"
    "index.sequence.html" = "WEB_SEQUENCE_HTML"
    "settings.html"       = "WEB_SETTINGS_HTML"
    "calibration.html"    = "WEB_CALIBRATION_HTML"
    "api.html"            = "WEB_API_HTML"
    "styles.css"          = "WEB_STYLES_CSS"
    "javascript.js"       = "WEB_JAVASCRIPT_JS"
    "joystick.js"         = "WEB_JOYSTICK_JS"
}

$header = @"
// =============================================================================
// AUTO-GENERIERT von gen-webassets.ps1 - NICHT VON HAND BEARBEITEN
// Quelle: data/*.* Dateien
// Generiert: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
// =============================================================================
#ifndef WebAssets_h
#define WebAssets_h

#include <Arduino.h>

"@

$out = [System.Text.StringBuilder]::new()
[void]$out.Append($header)

$totalBytes = 0
foreach ($file in $mapping.Keys | Sort-Object) {
    $path = Join-Path $dataDir $file
    if (-not (Test-Path $path)) {
        Write-Host "  WARN: $file fehlt, skip" -ForegroundColor Yellow
        continue
    }
    $content = Get-Content -Path $path -Raw -Encoding UTF8
    if ($null -eq $content) { $content = "" }

    # Pruefe ob der Delimiter ")HUY(" im Content vorkommt - sollte nie
    if ($content -match '\)HUY"') {
        Write-Host "  FEHLER: $file enthaelt Delimiter-Sequenz )HUY"" - waehle anderen Delimiter!" -ForegroundColor Red
        exit 1
    }

    $const = $mapping[$file]
    $sizeBytes = [System.Text.Encoding]::UTF8.GetByteCount($content)
    $totalBytes += $sizeBytes
    [void]$out.AppendLine("// ----- $file ($sizeBytes bytes) -----")
    [void]$out.AppendLine("const char $const[] PROGMEM = R""HUY(")
    [void]$out.Append($content)
    [void]$out.AppendLine(")HUY"";")
    [void]$out.AppendLine("")
    Write-Host "  OK: $file -> $const ($sizeBytes bytes)" -ForegroundColor Green
}

[void]$out.AppendLine("#endif // WebAssets_h")

# Mit UTF-8 (ohne BOM) schreiben - wichtig fuer Arduino-Compiler
[System.IO.File]::WriteAllText($outFile, $out.ToString(), [System.Text.UTF8Encoding]::new($false))

Write-Host ""
Write-Host "=== Fertig ===" -ForegroundColor Cyan
Write-Host "Datei : $outFile"
Write-Host "Bytes : $totalBytes ($([math]::Round($totalBytes/1KB, 1)) KB)"
