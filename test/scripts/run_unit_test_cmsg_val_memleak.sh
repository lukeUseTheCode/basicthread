#!/bin/sh
BUILD_PATH="../build"
UNIT_TEST_BIN="$BUILD_PATH/unit_test"
UNIT_TEST_MEMLEAK_LOG="$BUILD_PATH/unit_test_memory_leak.log"

echo ""
echo "Valgrind run Unit Test"
echo ""
echo "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="$UNIT_TEST_MEMLEAK_LOG" "$UNIT_TEST_BIN""
echo ""
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="$UNIT_TEST_LOG" "$UNIT_TEST_BIN"