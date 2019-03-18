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
    name = "vars",
    srcs = [
        "vars.cc",
    ],
    hdrs = [
        "vars.h",
    ],
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
