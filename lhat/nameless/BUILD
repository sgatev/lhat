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
    name = "shift",
    srcs = [
        "shift.cc",
    ],
    hdrs = [
        "shift.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":ast",
    ],
)

cc_test(
    name = "shift_test",
    srcs = [
        "shift_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":ast",
        ":shift",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
