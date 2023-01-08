#!/usr/bin/env pwsh

Set-StrictMode -Version latest
$ErrorActionPreference = "Stop"

$PROJECT_DIR="$PSScriptRoot\..\.."

conan profile new `
  --detect `
  --force `
  ./detected

conan install `
  --build=missing `
  --profile=./detected `
  --profile="${PROJECT_DIR}/workflows/conan/profiles/test-msvc" `
  "${PROJECT_DIR}"

conan build `
  "${PROJECT_DIR}"
