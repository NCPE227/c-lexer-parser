#!/bin/bash

# 1. Build the project if the executable doesn't exist
if [ ! -f "./parse" ]; then
    echo "Executable 'parse' not found. Running make..."
    make
    if [ $? -ne 0 ]; then
        echo "Error: 'make' failed."
        exit 1
    fi
fi

# Counters for reporting
total=0
passed=0

echo "---------------------------------------"
echo "Running Valid Tests (Expecting Success)"
echo "---------------------------------------"

for test_file in tests/valid/*; do
    [ -e "$test_file" ] || continue
    total=$((total + 1))

    # Run the parser and capture output
    output=$(./parse "$test_file" 2>&1)
    status=$?

    # Trim any potential whitespace from the output for comparison
    trimmed_output=$(echo "$output" | xargs)

    if [ $status -eq 0 ] && [[ "$trimmed_output" == "accepts" ]]; then
        echo "[PASS] $test_file"
        passed=$((passed + 1))
    else
        echo "[FAIL] $test_file"
        echo "       Expected: 'accepts' with exit 0"
        echo "       Actual:   '$trimmed_output' with exit $status"
    fi
done

echo ""
echo "---------------------------------------"
echo "Running Invalid Tests (Expecting Error)"
echo "---------------------------------------"

for test_file in tests/invalid/*; do
    [ -e "$test_file" ] || continue
    total=$((total + 1))

    # Run the parser
    output=$(./parse "$test_file" 2>&1)
    status=$?

    if [ $status -ne 0 ]; then
        echo "[PASS] $test_file (Corrected rejected)"
        passed=$((passed + 1))
    else
        echo "[FAIL] $test_file (Expected error, but parser accepted)"
    fi
done

echo ""
echo "---------------------------------------"
echo "Final Score: $passed / $total tests passed"
echo "---------------------------------------"

# Exit with failure if any test failed
if [ $passed -eq $total ]; then
    exit 0
else
    exit 1
fi