cc_library(
    name = "const_env",
    srcs = [
        "const_env.cc",
    ],
    hdrs = [
        "const_env.h",
    ],
    deps = [
        "@absl//absl/strings",
    ],
)

cc_test(
    name = "const_env_test",
    srcs = [
        "const_env_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":const_env",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "names",
    srcs = [
        "names.cc",
    ],
    hdrs = [
        "names.h",
    ],
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

cc_binary(
    name = "repl",
    srcs = [
        "repl.cc",
    ],
    deps = [
        ":const_env",
        ":names",
        "//lhat/named:ast",
        "//lhat/nameless:ast",
        "//lhat/nameless:reduce",
        "@absl//absl/strings",
    ],
)
