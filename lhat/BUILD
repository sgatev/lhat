cc_library(
    name = "names",
    srcs = [
        "names.cc",
    ],
    hdrs = [
        "names.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lhat/named:ast",
        "//lhat/named:vars",
        "//lhat/nameless:ast",
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
