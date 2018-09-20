#!/bin/bash

BIN="$(dirname "$(readlink -f "$0")")/bin/find"

TESTBED="testbed"

CMDS=("basic"
      "basic -type f"
      "basic -type d"
      "basic -follow"
      "basic -type f -follow"
      "basic -type d -follow"
      "basic -name bb1"
      "basic -name bbb2"
      "basic -name bb*"
      "basic -name bbb*"
      "basic -name '*'"
      "basic -name '????'"
      "basic -name bbb2 -type f"
      "basic -name bbb2 -type d"
      "basic -name bbb* -type f"
      "basic -name bbb* -type d"
      "basic -name '*' -type f"
      "basic -name '*' -type d"
      "basic -name bbb2 -follow"
      "basic -name bbb* -follow"
      "basic -name '*' -follow"
      "basic -name 'LINK_TO_*'"
      "basic -name 'LINK_TO_*' -type f"
      "basic -name 'LINK_TO_*' -type d"
      "basic -name 'LINK_TO_*' -follow"
      "basic -xdev"
      "basic -xdev -type f"
      "basic -xdev -type d"
      "basic -xdev -follow"
      "basic -xdev -type f -follow"
      "basic -xdev -type d -follow"
      "basic -xdev -name bbb2"
      "basic -xdev -name bbb*"
      "basic -xdev -name '*'"
      "traps/loop_sub_directory"
      "traps/loop_sub_directory -follow"
      "traps/sub_directory_1")

REF_OUT=$(mktemp)
TEST_OUT=$(mktemp)

cd "$TESTBED"

for cmd in "${CMDS[@]}"; do
    echo -e "\e[1m=== Testing: find $cmd\e[0m"

	find $cmd > "$REF_OUT" 2> /dev/null
    "$BIN" $cmd > "$TEST_OUT" 2> /dev/null

    cmp --silent "$REF_OUT" "$TEST_OUT"

    if [ $? -eq 0 ]; then
      echo -e "\e[32m==> Success\e[39m\n"
    else
      echo -e "\e[31m==> Failure, diff is:\e[39m" >&2
      diff --color "$REF_OUT" "$TEST_OUT" >&2
      echo ""
      exit 1
    fi
done

rm "$REF_OUT" "$TEST_OUT"
