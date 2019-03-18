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
