cc_library(
    name = "ast",
    srcs = [
        "ast.cc",
        "parser.cc",
    ],
    hdrs = [
        "ast.h",
        "parser.h",
    ],
)

cc_test(
    name = "ast_test",
    srcs = [
        "parser_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":ast",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
