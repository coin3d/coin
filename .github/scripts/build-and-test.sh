#!/usr/bin/env bash

set -euo pipefail

readonly legacy_renderer="${1:?usage: build-and-test.sh <ON|OFF>}"
case "${legacy_renderer}" in
  ON|OFF) ;;
  *)
    echo "Legacy renderer must be ON or OFF, got: ${legacy_renderer}" >&2
    exit 2
    ;;
esac

readonly repository_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
readonly build_dir="${COIN_CI_BUILD_DIR:-${repository_dir}/cmake_build_dir}"
readonly install_dir="${COIN_CI_INSTALL_DIR:-${repository_dir}/cmake_install_dir}"
readonly smoke_build_dir="${COIN_CI_SMOKE_BUILD_DIR:-${repository_dir}/installed-package-smoke}"

cmake_options=(
  -DCMAKE_BUILD_TYPE=Release
  -DCMAKE_INSTALL_PREFIX="${install_dir}"
  -DCOIN_BUILD_LEGACY_GL_RENDERER="${legacy_renderer}"
  -DCOIN_BUILD_TESTS=ON
  -DCOIN_STRICT_WARNINGS=ON
)

cmake -S "${repository_dir}" -B "${build_dir}" -G Ninja \
  "${cmake_options[@]}"
cmake --build "${build_dir}" --target install --config Release --parallel
ctest --test-dir "${build_dir}" -C Release --output-on-failure

cmake -S "${repository_dir}/testsuite/installed-package-smoke" \
  -B "${smoke_build_dir}" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="${install_dir}" \
  -DCOIN_EXPECT_LEGACY_GL_RENDERER="${legacy_renderer}"
cmake --build "${smoke_build_dir}" --config Release --parallel
