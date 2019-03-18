cc_library(
    name = "names",
    srcs = [
        "names.cc",
    ],
    hdrs = [
        "names.h",
    ],
)

cc_test(
    name = "names_test",
    srcs = [
        "names_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":names",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
