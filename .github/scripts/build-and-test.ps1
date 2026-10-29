param(
    [Parameter(Mandatory = $true)]
    [ValidateSet("ON", "OFF")]
    [string]$LegacyRenderer
)

$ErrorActionPreference = "Stop"

$repositoryDir = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot "../.."))
$buildDir = if ($env:COIN_CI_BUILD_DIR) {
    $env:COIN_CI_BUILD_DIR
} else {
    Join-Path $repositoryDir "cmake_build_dir"
}
$installDir = if ($env:COIN_CI_INSTALL_DIR) {
    $env:COIN_CI_INSTALL_DIR
} else {
    Join-Path $repositoryDir "cmake_install_dir"
}
$smokeBuildDir = if ($env:COIN_CI_SMOKE_BUILD_DIR) {
    $env:COIN_CI_SMOKE_BUILD_DIR
} else {
    Join-Path $repositoryDir "installed-package-smoke"
}

$configureArguments = @(
    "-S", $repositoryDir,
    "-B", $buildDir,
    "-G", "Visual Studio 17 2022",
    "-A", "x64",
    "-DCMAKE_INSTALL_PREFIX=$installDir",
    "-DCOIN_BUILD_LEGACY_GL_RENDERER=$LegacyRenderer",
    "-DCOIN_BUILD_TESTS=ON"
)
cmake @configureArguments
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build $buildDir --target INSTALL --config Release --parallel 2
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

ctest --test-dir $buildDir -C Release --output-on-failure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$smokeConfigureArguments = @(
    "-S", (Join-Path $repositoryDir "testsuite/installed-package-smoke"),
    "-B", $smokeBuildDir,
    "-G", "Visual Studio 17 2022",
    "-A", "x64",
    "-DCMAKE_PREFIX_PATH=$installDir",
    "-DCOIN_EXPECT_LEGACY_GL_RENDERER=$LegacyRenderer"
)
cmake @smokeConfigureArguments
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build $smokeBuildDir --config Release --parallel 2
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
