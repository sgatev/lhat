load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "bazel_skylib",
    remote = "https://github.com/bazelbuild/bazel-skylib",
    tag = "1.2.1",
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

git_repository(
    name = "absl",
    remote = "https://github.com/abseil/abseil-cpp",
    tag = "20220623.0",
)

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.12.1",
)
