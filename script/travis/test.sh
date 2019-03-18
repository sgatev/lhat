#!/usr/bin/env bash

set -e

wget https://github.com/bazelbuild/bazel/releases/download/0.20.0/bazel-0.20.0-installer-${BAZEL_OS}-x86_64.sh
chmod +x bazel-0.20.0-installer-${BAZEL_OS}-x86_64.sh
./bazel-0.20.0-installer-${BAZEL_OS}-x86_64.sh --user

bazel build ...
bazel test ...
