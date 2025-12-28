#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

MINIRT=./minirt
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=42"
LOG_FILE="error_test.log"

# Clean up previous log
rm -f $LOG_FILE

echo -e "${YELLOW}Starting Error Tests...${NC}"

run_test() {
    local file=$1
    local desc=$2
    local expected_msg=$3

    echo -n "Testing: $desc ... "

    # Run minirt with valgrind
    # We expect it to fail (exit code != 0)
    # We capture output to check for specific error message
    output=$($VALGRIND $MINIRT "$file" 2>&1)
    exit_code=$?

    # Check if it crashed or exited with error
    if [ $exit_code -eq 0 ]; then
        echo -e "${RED}FAILED${NC} (Program did not exit with error)"
        echo "Output:" >> $LOG_FILE
        echo "$output" >> $LOG_FILE
		pwd
        return 1
    fi

    # Check for memory leaks (valgrind error exit code is 42)
    if [ $exit_code -eq 42 ]; then
        echo -e "${RED}MEMORY LEAK DETECTED${NC}"
        echo "Valgrind Output for $desc:" >> $LOG_FILE
        echo "$output" >> $LOG_FILE
        return 1
    fi

    # Check for expected error message
    if echo "$output" | grep -q "$expected_msg"; then
        echo -e "${GREEN}PASSED${NC}"
    else
        echo -e "${RED}FAILED${NC} (Expected error message not found)"
        echo "Expected: $expected_msg" >> $LOG_FILE
        echo "Got:" >> $LOG_FILE
        echo "$output" >> $LOG_FILE
        return 1
    fi
}

# --- 1. File Errors ---

# Non-existent file
run_test "non_existent_file.rt" "Non-existent file" "Error: Cannot open file"

# Empty file
touch empty.rt
run_test "empty.rt" "Empty file" "Error: empty file"
rm empty.rt

# Directory
mkdir -p dummy_dir.rt
run_test "dummy_dir.rt" "Directory as file" "Error: reading file"
rmdir dummy_dir.rt

# File too big
perl -e 'print "A 0.2 255,255,255\n" x 300' > big_file.rt
run_test "big_file.rt" "File too big" "Error: file too big"
rm big_file.rt

# Invalid extension
touch invalid.txt
run_test "invalid.txt" "Invalid extension" "Error: Scene file must have a .rt extension"
rm invalid.txt


# --- 2. Parsing/Syntax Errors ---

# Invalid Identifier
echo "X 0 0 0" > test.rt
run_test "test.rt" "Invalid identifier" "Error: Invalid identifier"

# --- Ambient Light Errors ---
echo "A 0.2" > test.rt
run_test "test.rt" "Ambient: Missing args" "Error: Ambient light args count."

echo "A 0.2 255,255,255 0.2" > test.rt
run_test "test.rt" "Ambient: Extra args" "Error: Ambient light args count."

echo "A 1.5 255,255,255" > test.rt
run_test "test.rt" "Ambient: Invalid ratio > 1" "Error: Ambient light ratio invalid."

echo "A -0.1 255,255,255" > test.rt
run_test "test.rt" "Ambient: Invalid ratio < 0" "Error: Ambient light ratio invalid."

echo "A 0.2 256,255,255" > test.rt
run_test "test.rt" "Ambient: Invalid color > 255" "Error: Ambient light color invalid."

echo "A 0.2 255,255,-1" > test.rt
run_test "test.rt" "Ambient: Invalid color < 0" "Error: Ambient light color invalid."

echo "A 0.2 255,255,255" > test.rt
echo "A 0.2 255,255,255" >> test.rt
run_test "test.rt" "Ambient: Duplicate" "Error: Duplicate Ambient."

# --- Camera Errors ---
echo "C 0,0,0" > test.rt
run_test "test.rt" "Camera: Missing args" "Error: Camera args count."

echo "C 0,0,0 0,0,1 70 1" > test.rt
run_test "test.rt" "Camera: Extra args" "Error: Camera args count."

echo "C 0,0,0 0,0,0 70" > test.rt
run_test "test.rt" "Camera: Zero normal" "Error: Camera normal invalid."

echo "C 0,0,0 0,2,0 70" > test.rt
run_test "test.rt" "Camera: Invalid normal > 1" "Error: Camera normal invalid."

echo "C 0,0,0 0,0,1 181" > test.rt
run_test "test.rt" "Camera: FOV > 180" "Error: Camera FOV invalid."

echo "C 0,0,0 0,0,1 -1" > test.rt
run_test "test.rt" "Camera: FOV < 0" "Error: Camera FOV invalid."

echo "C 0,0,0 0,0,1 70" > test.rt
echo "C 0,0,0 0,0,1 70" >> test.rt
run_test "test.rt" "Camera: Duplicate" "Error: Duplicate Camera."

# --- Light Errors ---
echo "L 0,0,0" > test.rt
run_test "test.rt" "Light: Missing args" "Error: Light args count."

echo "L 0,0,0 0.5 255,255,255 1" > test.rt
run_test "test.rt" "Light: Extra args" "Error: Light args count."

echo "L 0,0,0 1.1 255,255,255" > test.rt
run_test "test.rt" "Light: Invalid ratio" "Error: Light ratio invalid."

echo "L 0,0,0 0.5 300,255,255" > test.rt
run_test "test.rt" "Light: Invalid color" "Error: Light color invalid."

# --- Sphere Errors ---
echo "sp 0,0,0" > test.rt
run_test "test.rt" "Sphere: Missing args" "Error: Sphere args count."

echo "sp 0,0,0 10 255,0,0 0.5 255,255,255 1" > test.rt
run_test "test.rt" "Sphere: Extra args" "Error: Sphere args count."

echo "sp 0,0,0 -10 255,0,0" > test.rt
run_test "test.rt" "Sphere: Negative diameter" "Error: Sphere diameter invalid."

echo "sp 0,0,0 10 255,0,300" > test.rt
run_test "test.rt" "Sphere: Invalid color" "Error: Sphere color invalid."

# --- Plane Errors ---
echo "pl 0,0,0" > test.rt
run_test "test.rt" "Plane: Missing args" "Error: Plane args count."

echo "pl 0,0,0 0,1,0 255,0,0 0.5 255,255,255 1" > test.rt
run_test "test.rt" "Plane: Extra args" "Error: Plane args count."

echo "pl 0,0,0 0,2,0 255,0,0" > test.rt
run_test "test.rt" "Plane: Invalid normal" "Error: Plane normal invalid."

echo "pl 0,0,0 0,1,0 255,0,300" > test.rt
run_test "test.rt" "Plane: Invalid color" "Error: Plane color invalid."

# --- Cylinder Errors ---
echo "cy 0,0,0" > test.rt
run_test "test.rt" "Cylinder: Missing args" "Error: Cylinder args count."

echo "cy 0,0,0 0,1,0 10 20 255,0,0 0.5 255,255,255 1" > test.rt
run_test "test.rt" "Cylinder: Extra args" "Error: Cylinder args count."

echo "cy 0,0,0 0,2,0 10 20 255,0,0" > test.rt
run_test "test.rt" "Cylinder: Invalid normal" "Error: Cylinder normal invalid."

echo "cy 0,0,0 0,1,0 -10 20 255,0,0" > test.rt
run_test "test.rt" "Cylinder: Negative diameter" "Error: Cylinder diameter invalid."

echo "cy 0,0,0 0,1,0 10 -20 255,0,0" > test.rt
run_test "test.rt" "Cylinder: Negative height" "Error: Cylinder height invalid."

echo "cy 0,0,0 0,1,0 10 20 255,0,300" > test.rt
run_test "test.rt" "Cylinder: Invalid color" "Error: Cylinder color invalid."

# --- Scene Validation Errors ---
# Missing Ambient
echo "C 0,0,0 0,0,1 70" > test.rt
run_test "test.rt" "Missing Ambient" "Error: Missing A or C."

# Missing Camera
echo "A 0.2 255,255,255" > test.rt
run_test "test.rt" "Missing Camera" "Error: Missing A or C."

# Cleanup
rm -f test.rt

echo -e "${YELLOW}Tests Completed. Check $LOG_FILE for details.${NC}"
