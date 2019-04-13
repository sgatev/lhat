cc_library(
    name = "ast",
    srcs = [
        "ast.cc",
        "parse.cc",
        "printer.cc",
    ],
    hdrs = [
        "ast.h",
        "parse.h",
        "printer.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lhat/core:overloaded",
        "//lhat/core:parse",
        "@absl//absl/strings",
    ],
)

cc_test(
    name = "ast_test",
    srcs = [
        "ast_test.cc",
        "parse_test.cc",
        "printer_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":ast",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "equiv",
    srcs = [
        "equiv.cc",
    ],
    hdrs = [
        "equiv.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":ast",
        ":sub",
    ],
)

cc_test(
    name = "equiv_test",
    srcs = [
        "equiv_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":equiv",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "sub",
    srcs = [
        "sub.cc",
    ],
    hdrs = [
        "sub.h",
    ],
    deps = [
        ":vars",
    ],
)

cc_test(
    name = "sub_test",
    srcs = [
        "sub_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":sub",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "vars",
    srcs = [
        "vars.cc",
    ],
    hdrs = [
        "vars.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":ast",
    ],
)

cc_test(
    name = "vars_test",
    srcs = [
        "vars_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":vars",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
