#!/usr/bin/env pwsh

Set-StrictMode -Version latest
$ErrorActionPreference = "Stop"

$PROJECT_DIR="$PSScriptRoot\..\.."

conan install `
  --build=missing `
  --env wss:CXXFLAGS="/W2 /WX /O2" `
  --settings build_type=Debug `
  "${PROJECT_DIR}"

conan build `
  "${PROJECT_DIR}"
