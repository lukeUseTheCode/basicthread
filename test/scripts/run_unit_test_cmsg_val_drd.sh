#!/bin/sh
BUILD_PATH="../build"
UNIT_TEST_CMSG_BIN="$BUILD_PATH/unit_test_cmsg"
UNIT_TEST_CMSG_DRD_LOG="$BUILD_PATH/unit_test_drd.log"

echo ""
echo "valgrind --tool=drd -s --log-file="$UNIT_TEST_CMSG_DRD_LOG" "$UNIT_TEST_CMSG_BIN""
valgrind --tool=drd -s --log-file="$UNIT_TEST_CMSG_DRD_LOG" "$UNIT_TEST_CMSG_BIN"