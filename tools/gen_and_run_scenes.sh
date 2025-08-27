#!/usr/bin/env zsh
trap 'echo "\n[INTERRUPTED] Cleaning up..."; pkill -P $$ 2>/dev/null; stty sane 2>/dev/null; exit 130' INT
set -eu

# Simple: generate N random scenes, run minirt for each, wait a fixed time, then kill.
# Usage: tools/gen_and_run_scenes.sh [--chaotic=LEVEL] [--valgrind] [N] [WAIT_SECS]
#   --chaotic=LEVEL: chaos level (0=minimal, 1=moderate, 2=full chaos)
#     0: Simple, easily visible scenes with minimal randomness
#     1: Moderate randomness within reasonable bounds
#     2: Full legal range stress testing (extreme values)
#   --valgrind: run with valgrind memory checking
#   N: number of scenes (default 10)
#   WAIT_SECS: seconds to wait before killing (default 2)
#
# Default mode (no --chaotic) uses level 1 for reasonable random scenes.

N=10
WAIT_SECS=2
VALGRIND_MODE=0
CHAOTIC_MODE=1  # Default to level 1 (moderate randomness)

# --- Robust, portable argument parsing ---
pos_args=()
while [[ $# -gt 0 ]]; do
  case "$1" in
    --valgrind)
      VALGRIND_MODE=1
      shift # past argument
      ;;
    --chaotic=*)
      CHAOTIC_MODE=${1#*=}
      shift # past argument=value
      ;;
    --chaotic)
      CHAOTIC_MODE=2
      shift # past argument
      ;;
    -*)
      echo "Error: Unknown option $1" >&2
      exit 1
      ;;
    *)
      pos_args+=("$1") # save positional arg
      shift # past argument
      ;;
  esac
done

# Restore positional arguments from the filtered list
set -- "${pos_args[@]}"

# Apply positional arguments
if [[ $# -ge 1 ]]; then
  N=$1
fi
if [[ $# -ge 2 ]]; then
  WAIT_SECS=$2
fi

# Function to create a readable valgrind summary
create_valgrind_summary() {
  local log_file="$1"
  local summary_file="${log_file%.log}_summary.txt"
  
  if [[ ! -f "$log_file" ]]; then
    echo "Valgrind log not found: $log_file" > "$summary_file"
    return
  fi
  
  {
    echo "=== VALGRIND SUMMARY FOR $(basename "$log_file") ==="
    echo "Generated: $(date)"
    echo ""
    
    # Process termination info
    if grep -q "Process terminating" "$log_file"; then
      echo "Process was terminated (expected when using timeout)"
      echo ""
    fi
    
    # Memory errors (the important stuff)
    echo "=== MEMORY ERRORS ==="
    local invalid_reads=$(grep -c "Invalid read" "$log_file" 2>/dev/null)
    local invalid_writes=$(grep -c "Invalid write" "$log_file" 2>/dev/null)
    local uninit_values=$(grep -c "Use of uninitialised value" "$log_file" 2>/dev/null)
    local conditional_jumps=$(grep -c "Conditional jump.*uninitialised" "$log_file" 2>/dev/null)
    
    # Ensure numeric values
    invalid_reads=${invalid_reads:-0}
    invalid_writes=${invalid_writes:-0}
    uninit_values=${uninit_values:-0}
    conditional_jumps=${conditional_jumps:-0}
    
    if [[ $invalid_reads -eq 0 && $invalid_writes -eq 0 && $uninit_values -eq 0 && $conditional_jumps -eq 0 ]]; then
      echo "✓ No memory access errors found"
    else
      echo "⚠ Memory access errors detected:"
      [[ $invalid_reads -gt 0 ]] && echo "  - Invalid reads: $invalid_reads"
      [[ $invalid_writes -gt 0 ]] && echo "  - Invalid writes: $invalid_writes"
      [[ $uninit_values -gt 0 ]] && echo "  - Uninitialized values used: $uninit_values"
      [[ $conditional_jumps -gt 0 ]] && echo "  - Conditional jumps on uninitialized data: $conditional_jumps"
    fi
    echo ""
    
    # Memory leaks summary
    echo "=== MEMORY LEAKS SUMMARY ==="
    if grep -q "HEAP SUMMARY" "$log_file"; then
      grep -A 1 "HEAP SUMMARY" "$log_file" | tail -1
      echo ""
    fi
    
    if grep -q "LEAK SUMMARY" "$log_file"; then
      echo "Leak breakdown:"
      grep -A 10 "LEAK SUMMARY" "$log_file" | grep -E "(definitely lost|indirectly lost|possibly lost|still reachable)"
    else
      echo "No leak summary found"
    fi
    echo ""
    
    # Key information only (filter out noise)
    echo "=== ACTUAL ISSUES (excluding 'still reachable') ==="
    local definitely_lost=$(grep "definitely lost:" "$log_file" | awk '{print $4}' | sed 's/,//g' | head -1)
    local indirectly_lost=$(grep "indirectly lost:" "$log_file" | awk '{print $4}' | sed 's/,//g' | head -1)
    local possibly_lost=$(grep "possibly lost:" "$log_file" | awk '{print $4}' | sed 's/,//g' | head -1)
    
    # Default to 0 if not found or empty
    definitely_lost=${definitely_lost:-0}
    indirectly_lost=${indirectly_lost:-0}
    possibly_lost=${possibly_lost:-0}
    
    # Check if any issues exist
    if [[ $definitely_lost -eq 0 && $indirectly_lost -eq 0 && $possibly_lost -eq 0 ]]; then
      echo "✓ No serious memory leaks detected"
    else
      echo "⚠ Memory leaks found:"
      [[ $definitely_lost -gt 0 ]] && echo "  - Definitely lost: $definitely_lost bytes"
      [[ $indirectly_lost -gt 0 ]] && echo "  - Indirectly lost: $indirectly_lost bytes"
      [[ $possibly_lost -gt 0 ]] && echo "  - Possibly lost: $possibly_lost bytes"
    fi
    echo ""
    
    echo "=== NOTES ==="
    echo "- 'Still reachable' memory is expected when programs are force-terminated"
    echo "- Graphics libraries (MLX, X11) commonly leave reachable memory"
    echo "- Focus on 'definitely lost', 'possibly lost', and memory access errors"
    echo ""
    echo "Full detailed log: $log_file"
    
  } > "$summary_file"
  
  echo "Readable summary created: $summary_file"
}


SCRIPT_DIR="${0:A:h}"
SCENES_DIR="$SCRIPT_DIR/scenes"
OUT_DIR="$SCENES_DIR/generated"
LOG_DIR="$OUT_DIR/logs"
BIN="$SCRIPT_DIR/../minirt"

if [[ ! -x "$BIN" ]]; then
  echo "Error: minirt binary not found at $BIN. Run 'make' first."
  exit 1
fi

if ! rm -rf -- "$OUT_DIR" 2>/dev/null; then
  echo "Error: Cannot remove existing generated directory. Check permissions."
  exit 1
fi

if ! mkdir -p "$OUT_DIR" 2>/dev/null; then
  echo "Error: Cannot create output directory $OUT_DIR. Check permissions."
  exit 1
fi

if ! mkdir -p "$LOG_DIR" 2>/dev/null; then
  echo "Error: Cannot create log directory $LOG_DIR. Check permissions."
  exit 1
fi


# --- Scene generation helpers ---
rand_intv() {
  local name=$1 min=$2 max=$3
  local v=$(( min + (RANDOM % (max - min + 1)) ))
  typeset -g "$name=$v"
}

rand_floatv() {
  local name=$1 min=$2 max=$3
  local v=$(( min + (max - min) * (RANDOM + 0.0) / 32767.0 ))
  # Round to 2 decimal places for readability
  printf -v v "%.2f" "$v"
  typeset -g "$name=$v"
}

# RGB color generation [0-255]
rgbv() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: any color
    local r=$(( RANDOM % 256 ))
    local g=$(( RANDOM % 256 ))
    local b=$(( RANDOM % 256 ))
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: avoid too dark colors
    local r=$(( 64 + (RANDOM % 192) ))
    local g=$(( 64 + (RANDOM % 192) ))
    local b=$(( 64 + (RANDOM % 192) ))
  else
    # Level 0: bright, pleasant colors
    local r=$(( 100 + (RANDOM % 156) ))
    local g=$(( 100 + (RANDOM % 156) ))
    local b=$(( 100 + (RANDOM % 156) ))
  fi
  typeset -g "$name=$r,$g,$b"
}

# Normalized vector for planes/cylinders [-1,1] per component
pick_norm_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: many random orientations
    local -a chaotic_choices=(
      "1,0,0" "-1,0,0" "0,1,0" "0,-1,0" "0,0,1" "0,0,-1"
      "0.707,0.707,0" "-0.707,0.707,0" "0.707,-0.707,0" "-0.707,-0.707,0"
      "0.707,0,0.707" "-0.707,0,0.707" "0.707,0,-0.707" "-0.707,0,-0.707"
      "0,0.707,0.707" "0,-0.707,0.707" "0,0.707,-0.707" "0,-0.707,-0.707"
      "0.577,0.577,0.577" "-0.577,0.577,0.577" "0.577,-0.577,0.577" "0.577,0.577,-0.577"
      "-0.577,-0.577,0.577" "-0.577,0.577,-0.577" "0.577,-0.577,-0.577" "-0.577,-0.577,-0.577"
    )
    local idx=$(( RANDOM % ${#chaotic_choices[@]} ))
    typeset -g "$name=${chaotic_choices[$((idx + 1))]}"
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: nice orientations with some variety
    local -a choices=(
      "0,0,-1" "0,-1,0" "0,1,0" "1,0,0" "-1,0,0" "0,0,1"
      "0.707,0.000,-0.707" "-0.707,0.000,-0.707" "0.577,0.577,-0.577"
      "0.577,-0.577,0.577" "-0.577,0.577,0.577" "0.000,0.707,0.707"
      "0.000,-0.707,0.707" "0.707,0.707,0.000" "-0.707,0.707,0.000"
      "0.408,0.816,0.408" "-0.408,0.816,-0.408" "0.333,0.667,0.667"
    )
    local idx=$(( RANDOM % ${#choices[@]} ))
    typeset -g "$name=${choices[$((idx + 1))]}"
  else
    # Level 0: simple cardinal and 45-degree orientations
    local -a simple_choices=(
      "0,0,-1" "0,-1,0" "0,1,0" "1,0,0" "-1,0,0" "0,0,1"
      "0.707,0.707,0" "0.707,-0.707,0" "0.707,0,0.707" "0.707,0,-0.707"
    )
    local idx=$(( RANDOM % ${#simple_choices[@]} ))
    typeset -g "$name=${simple_choices[$((idx + 1))]}"
  fi
}

# Camera position generation
camera_pos_v() {
  local name=$1
  local x y z
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: anywhere
    rand_floatv x -50.0 50.0
    rand_floatv y -50.0 50.0
    rand_floatv z -50.0 50.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: reasonable range
    rand_floatv x -3.0 3.0
    rand_floatv y 0.5 4.0
    rand_floatv z 2.0 10.0
  else
    # Level 0: close, simple positions
    rand_floatv x -1.0 1.0
    rand_floatv y 1.0 2.0
    rand_floatv z 3.0 6.0
  fi
  typeset -g "$name=$x,$y,$z"
}

# Camera direction generation (normalized)
camera_dir_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: any direction
    pick_norm_v "$name"
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: generally point toward negative Z
    local -a dirs=(
      "0,0,-1" "0,-0.2,-1" "0,0.2,-1" "0.2,0,-1" "-0.2,0,-1"
      "0.3,-0.1,-1" "-0.3,0.1,-1" "0.1,0.3,-1" "-0.1,-0.3,-1"
    )
    local idx=$(( RANDOM % ${#dirs[@]} ))
    typeset -g "$name=${dirs[$((idx + 1))]}"
  else
    # Level 0: straight forward or slight variations
    local -a simple_dirs=("0,0,-1" "0,-0.1,-1" "0,0.1,-1")
    local idx=$(( RANDOM % ${#simple_dirs[@]} ))
    typeset -g "$name=${simple_dirs[$((idx + 1))]}"
  fi
}

# Object positioning
vec3_front_v() {
  local name=$1
  local x y z
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: anywhere
    rand_floatv x -100.0 100.0
    rand_floatv y -100.0 100.0
    rand_floatv z -100.0 100.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: reasonable spread
    rand_floatv x -5.0 5.0
    rand_floatv y -2.0 4.0
    rand_floatv z -20.0 -3.0
  else
    # Level 0: close and visible
    rand_floatv x -2.0 2.0
    rand_floatv y -1.0 2.0
    rand_floatv z -8.0 -4.0
  fi
  typeset -g "$name=$x,$y,$z"
}

# Light positioning
light_pos_v() {
  local name=$1
  local x y z
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: anywhere
    rand_floatv x -100.0 100.0
    rand_floatv y -100.0 100.0
    rand_floatv z -100.0 100.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: good lighting positions
    rand_floatv x -6.0 6.0
    rand_floatv y 2.0 10.0
    rand_floatv z -10.0 -1.0
  else
    # Level 0: simple overhead lighting
    rand_floatv x -2.0 2.0
    rand_floatv y 4.0 8.0
    rand_floatv z -1.0 2.0
  fi
  typeset -g "$name=$x,$y,$z"
}

# Ground plane Y position
ground_y_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: anywhere
    rand_floatv "$name" -50.0 50.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: reasonable ground level
    rand_floatv "$name" -3.0 -0.5
  else
    # Level 0: simple ground level
    rand_floatv "$name" -2.0 -1.0
  fi
}

# Ambient lighting ratio [0.0,1.0]
ambient_ratio_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: any legal value
    rand_floatv "$name" 0.0 1.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: reasonable ambient
    rand_floatv "$name" 0.1 0.4
  else
    # Level 0: well-lit scenes
    rand_floatv "$name" 0.2 0.3
  fi
}

# Light brightness [0.0,1.0]
light_brightness_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: any legal value
    rand_floatv "$name" 0.0 1.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: good lighting
    rand_floatv "$name" 0.5 1.0
  else
    # Level 0: bright lighting
    rand_floatv "$name" 0.7 0.9
  fi
}

# FOV [0,180]
fov_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: full legal range
    rand_intv "$name" 1 179
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: practical range
    rand_intv "$name" 45 90
  else
    # Level 0: normal vision
    rand_intv "$name" 60 80
  fi
}

# Sphere/Cylinder diameter (>0)
diameter_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: huge range
    rand_floatv "$name" 0.1 20.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: reasonable sizes
    rand_floatv "$name" 0.5 3.0
  else
    # Level 0: small, visible objects
    rand_floatv "$name" 0.8 2.0
  fi
}

# Cylinder height (>0)
height_v() {
  local name=$1
  if [[ $CHAOTIC_MODE -eq 2 ]]; then
    # Full chaos: huge range
    rand_floatv "$name" 0.1 20.0
  elif [[ $CHAOTIC_MODE -eq 1 ]]; then
    # Moderate: reasonable heights
    rand_floatv "$name" 1.0 5.0
  else
    # Level 0: reasonable proportions
    rand_floatv "$name" 1.0 3.0
  fi
}

for i in $(seq 1 $N); do
  scene="$OUT_DIR/gen_$i.rt"
  
  # Generate scene parameters using mode-aware functions
  ambient=0; ambient_ratio_v ambient
  amb_rgb=""; rgbv amb_rgb
  fov=60; fov_v fov

  # Light positioning and properties
  lpos=""; light_pos_v lpos
  lratio=0; light_brightness_v lratio
  lrgb=""; rgbv lrgb

  # Camera positioning and direction
  cam_pos=""; camera_pos_v cam_pos
  cam_dir=""; camera_dir_v cam_dir

  # Ground plane
  gy=0; ground_rgb=""
  ground_y_v gy
  rgbv ground_rgb

  # Main central sphere
  cs_pos=""; vec3_front_v cs_pos
  cs_d=0; diameter_v cs_d
  cs_rgb=""; rgbv cs_rgb

  {
    if [[ $CHAOTIC_MODE -eq 2 ]]; then
      echo "# auto-generated scene $i (FULL CHAOS MODE)"
    elif [[ $CHAOTIC_MODE -eq 1 ]]; then
      echo "# auto-generated scene $i (MODERATE MODE)"
    else
      echo "# auto-generated scene $i (SIMPLE MODE)"
    fi
    echo "A $ambient $amb_rgb"
    echo "C $cam_pos $cam_dir $fov"
    echo "L $lpos $lratio $lrgb"
    echo "pl 0,$gy,0 0,1,0 $ground_rgb"
    echo "sp $cs_pos $cs_d $cs_rgb"

    # Generate additional objects
    if [[ $CHAOTIC_MODE -eq 2 ]]; then
      obj_count=0; rand_intv obj_count 0 10
    elif [[ $CHAOTIC_MODE -eq 1 ]]; then
      obj_count=0; rand_intv obj_count 2 5
    else
      obj_count=0; rand_intv obj_count 1 3  # Simple scenes
    fi
    
    j=1
    while (( j <= obj_count )); do
      choice=0; rand_intv choice 0 2
      if (( choice == 0 )); then
        # Sphere
        pos=""; vec3_front_v pos
        d=0; diameter_v d
        color=""; rgbv color
        echo "sp $pos $d $color"
      elif (( choice == 1 )); then
        # Cylinder with random rotation
        pos=""; vec3_front_v pos
        axis=""; pick_norm_v axis
        d=0; diameter_v d
        h=0; height_v h
        color=""; rgbv color
        echo "cy $pos $axis $d $h $color"
      else
        # Plane
        pos=""; vec3_front_v pos
        norm=""; pick_norm_v norm
        color=""; rgbv color
        echo "pl $pos $norm $color"
      fi
      (( j++ ))
    done
  } >| "$scene"
done

ok=0

MODE_TEXT=""
if [[ $CHAOTIC_MODE -eq 2 ]]; then
  MODE_TEXT=" in FULL CHAOS mode"
elif [[ $CHAOTIC_MODE -eq 1 ]]; then
  MODE_TEXT=" in MODERATE mode"
else
  MODE_TEXT=" in SIMPLE mode"
fi

echo "[INFO] Generated $N scenes$MODE_TEXT. Running minirt $N times, $WAIT_SECS seconds each."

if [[ $N -eq 0 ]]; then
  echo ""
  echo "Scenes run complete. OK: 0, Fail: 0"
  exit 0
fi

fail=0
ok=0

for i in $(seq 1 $N); do
  scene="$OUT_DIR/gen_$i.rt"
  echo "[Run $i/$N] $scene"
  if [[ $VALGRIND_MODE -eq 1 ]]; then
    VLOG="$LOG_DIR/valgrind_$i.log"
    RAW_LOG="$LOG_DIR/minirt_run_$i.log"
    valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --track-origins=yes --log-file="$VLOG" "$BIN" "$scene" > "$RAW_LOG" 2>&1 &
    pid=$!
    # Wait for 'Done rendering.' or process exit, Ctrl+C is always responsive
    (
      while ! grep -q 'Done rendering\.' "$RAW_LOG" 2>/dev/null; do
        sleep 0.2
        if ! kill -0 $pid 2>/dev/null; then exit; fi
      done
      # If we get here, rendering is done, so wait a bit then kill
      sleep "$WAIT_SECS"
      kill $pid 2>/dev/null || true
    ) &
    waiter=$!
    wait $pid 2>/dev/null || true
    kill $waiter 2>/dev/null || true
    # Check valgrind log for actual memory errors (ignore still reachable)
    if [[ -f "$VLOG" ]]; then
      # Create a readable summary
      create_valgrind_summary "$VLOG"
      
      # --- Robust Valgrind Log Analysis (v3) ---
      
      # Safely extracts a number from a Valgrind summary line (e.g., "definitely lost: 10 bytes")
      # awk is perfect for this and handles no-match cases gracefully.
      get_val() {
        local val
        # The `|| true` prevents the script from exiting if grep finds no matches.
        val=$(grep "$1" "$2" | awk -F'[: ]+' '{print $4}' | sed 's/,//g' | head -1 || true)
        if [[ "$val" =~ ^[0-9]+$ ]]; then
          echo "$val"
        else
          echo "0"
        fi
      }

      # For counting occurrences, piping grep to wc -l is the most robust method.
      # It correctly returns 0 if no lines match, without exiting the script.
      invalid_reads=$(grep "Invalid read" "$VLOG" | wc -l)
      invalid_writes=$(grep "Invalid write" "$VLOG" | wc -l)
      uninit_values=$(grep "Use of uninitialised value" "$VLOG" | wc -l)
      conditional_jumps=$(grep "Conditional jump.*uninitialised" "$VLOG" | wc -l)

      # Extract key metrics using the safe function.
      definitely_lost=$(get_val "definitely lost:" "$VLOG")
      indirectly_lost=$(get_val "indirectly lost:" "$VLOG")
      possibly_lost=$(get_val "possibly lost:" "$VLOG")
      
      # Total actual errors (not including "still reachable")
      actual_errors=$((invalid_reads + invalid_writes + uninit_values + conditional_jumps))
      
      # Check for serious memory leaks or errors
      if [[ "$definitely_lost" -gt 0 || "$indirectly_lost" -gt 0 || "$possibly_lost" -gt 0 || "$actual_errors" -gt 0 ]]; then
        fail=$((fail+1))
        echo "  [FAIL] Memory issues detected:"
        if [[ "$definitely_lost" -gt 0 ]]; then
          echo "    - Definitely lost: $definitely_lost bytes"
        fi
        if [[ "$indirectly_lost" -gt 0 ]]; then
          echo "    - Indirectly lost: $indirectly_lost bytes"
        fi
        if [[ "$possibly_lost" -gt 0 ]]; then
          echo "    - Possibly lost: $possibly_lost bytes"
        fi
        if [[ "$actual_errors" -gt 0 ]]; then
          echo "    - Memory errors: $actual_errors (reads:$invalid_reads, writes:$invalid_writes, uninit:$uninit_values, cond:$conditional_jumps)"
        fi
        echo "    Summary: ${VLOG%.log}_summary.txt"
      else
        # Check for excessive still reachable (might indicate a real issue)
        still_reachable=$(get_val "still reachable:" "$VLOG")
        if [[ "$still_reachable" -gt 500000 ]]; then  # > 500KB still reachable might be excessive
          echo "  [WARN] Large amount still reachable: $still_reachable bytes (expected from forced termination)"
        fi
        ok=$((ok+1))
        echo "  [OK] No serious memory issues (summary: ${VLOG%.log}_summary.txt)"
      fi
    else
      fail=$((fail+1))
      echo "  [FAIL] No valgrind log found."
    fi
  else
    RAW_LOG="$LOG_DIR/minirt_run_$i.log"
    "$BIN" "$scene" > "$RAW_LOG" 2>&1 &
    pid=$!
    (
      while ! grep -q 'Done rendering\.' "$RAW_LOG" 2>/dev/null; do
        sleep 0.2
        if ! kill -0 $pid 2>/dev/null; then exit; fi
      done
      sleep "$WAIT_SECS"
      kill $pid 2>/dev/null || true
    ) &
    waiter=$!
    wait $pid 2>/dev/null || true
    kill $waiter 2>/dev/null || true
    # Check exit status for warnings
    if kill -0 $pid 2>/dev/null; then
      fail=$((fail+1))
      echo "  [WARN] minirt did not exit cleanly, killed."
    else
      ok=$((ok+1))
      echo "  [OK] minirt exited."
    fi
  fi
done
if [[ $VALGRIND_MODE -eq 1 ]]; then
  echo "\nValgrind mode complete. OK: $ok, Fail: $fail"
  echo "Valgrind logs in $LOG_DIR"
else
  echo "\nScenes run complete. OK: $ok, Fail: $fail"
fi
exit 0
