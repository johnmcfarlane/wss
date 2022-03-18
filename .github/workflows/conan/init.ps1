#!/usr/bin/env pwsh

Set-StrictMode -Version latest
$ErrorActionPreference = "Stop"

$PROJECT_DIR="$PSScriptRoot\..\..\.."

pip.exe install conan
conan config install "$PROJECT_DIR\.github\workflows\conan\settings.yml"
