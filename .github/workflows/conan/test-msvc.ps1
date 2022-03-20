#!/usr/bin/env pwsh

Set-StrictMode -Version latest
$ErrorActionPreference = "Stop"

$PROJECT_DIR="$PSScriptRoot\..\..\.."

conan install `
  --build=missing `
  --env CONAN_CMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}\.github\workflows\toolchains\windows-msvc.cmake" `
  --settings build_type=Test `
  "${PROJECT_DIR}" `
  "$@"

conan build `
  "${PROJECT_DIR}"
