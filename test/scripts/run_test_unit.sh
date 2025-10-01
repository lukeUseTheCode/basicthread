#!/bin/sh


BUILD_PATH="../build"
UNIT_TEST_BIN="$BUILD_PATH/unit_test"
UNIT_TEST_MEMLEAK_LOG="$BUILD_PATH/unit_test_memory_leak.log"
UNIT_TEST_HELGRIND_LOG="$BUILD_PATH/unit_test_helgrind.log"
UNIT_TEST_DRD_LOG="$BUILD_PATH/unit_test_drd.log"

echo ""
echo "Valgrind run Unit Test"
echo ""
echo "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="$UNIT_TEST_MEMLEAK_LOG" "$UNIT_TEST_BIN""
echo ""
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="$UNIT_TEST_LOG" "$UNIT_TEST_BIN"

echo""
echo "valgrind --tool=helgrind --history-level=approx --log-file="$UNIT_TEST_HELGRIND_LOG" -s "$UNIT_TEST_BIN""
valgrind --tool=helgrind --history-level=approx --log-file="$UNIT_TEST_HELGRIND_LOG" -s "$UNIT_TEST_BIN"

echo ""
echo "valgrind --tool=drd -s --log-file="$UNIT_TEST_DRD_LOG" "$UNIT_TEST_BIN""
valgrind --tool=drd -s --log-file="$UNIT_TEST_DRD_LOG" "$UNIT_TEST_BIN"