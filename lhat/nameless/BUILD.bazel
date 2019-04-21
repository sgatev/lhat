cc_library(
    name = "ast",
    srcs = [
        "ast.cc",
        "parser.cc",
        "printer.cc",
    ],
    hdrs = [
        "ast.h",
        "parser.h",
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
        "parser_test.cc",
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
    name = "beta",
    srcs = [
        "beta.cc",
    ],
    hdrs = [
        "beta.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":ast",
        ":sub",
    ],
)

cc_test(
    name = "beta_test",
    srcs = [
        "beta_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":ast",
        ":beta",
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
    visibility = ["//visibility:public"],
    deps = [
        ":ast",
    ],
)

cc_test(
    name = "sub_test",
    srcs = [
        "sub_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":ast",
        ":sub",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
