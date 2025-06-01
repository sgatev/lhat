#!/bin/bash
# Refreshes compile_commands.json.

bazel run @hedron_compile_commands//:refresh_all
