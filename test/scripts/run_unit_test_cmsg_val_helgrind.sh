#!/bin/sh
BUILD_PATH="../build"
UNIT_TEST_BIN="$BUILD_PATH/unit_test"
UNIT_TEST_HELGRIND_LOG="$BUILD_PATH/unit_test_helgrind.log"

echo""
echo "valgrind --tool=helgrind --history-level=approx --log-file="$UNIT_TEST_HELGRIND_LOG" -s "$UNIT_TEST_BIN""
valgrind --tool=helgrind --history-level=approx --log-file="$UNIT_TEST_HELGRIND_LOG" -s "$UNIT_TEST_BIN"