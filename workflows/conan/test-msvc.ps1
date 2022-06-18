#!/usr/bin/env pwsh

Set-StrictMode -Version latest
$ErrorActionPreference = "Stop"

$PROJECT_DIR="$PSScriptRoot\..\.."

conan install `
  --build=missing `
  --env CXXFLAGS="/bigobj /D_ITERATOR_DEBUG_LEVEL=1 /DNDEBUG /O2" `
  --env wss:CXXFLAGS="/bigobj /D_ITERATOR_DEBUG_LEVEL=1 /DNDEBUG /O2 /W2 /WX" `
  "${PROJECT_DIR}"

conan build `
  "${PROJECT_DIR}"
