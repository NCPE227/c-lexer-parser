#!/bin/bash

# Initialize counters
PASSED=0
FAILED=0

echo "=== Running VALID tests (Expected to ACCEPT) ==="
for file in tests/valid/*; do
    # Run parser and hide standard output/errors
    ./parser "$file" > /dev/null 2>&1

    # Check the exit status ($? is the return value of your main function)
    if [ $? -eq 0 ]; then
        echo "OK:   $file"
        ((PASSED++))
    else
        echo "FAIL: $file (Was rejected, but should have been accepted)"
        ((FAILED++))
    fi
done

echo -e "\n=== Running INVALID tests (Expected to REJECT) ==="
for file in tests/invalid/*; do
    ./parser "$file" > /dev/null 2>&1

    if [ $? -ne 0 ]; then
        echo "OK:   $file rejected correctly"
        ((PASSED++))
    else
        echo "FAIL: $file (Was accepted, but should have been rejected)"
        ((FAILED++))
    fi
done

echo -e "\n=== SUMMARY ==="
echo "Passed: $PASSED"
echo "Failed: $FAILED"