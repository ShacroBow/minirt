
#!/usr/bin/env zsh
trap 'echo "\n[INTERRUPTED] Cleaning up..."; pkill -P $$ 2>/dev/null; stty sane 2>/dev/null; exit 130' INT
set -eu

# Simple: generate N random scenes, run minirt for each, wait a fixed time, then kill.
# Usage: tools/gen_and_run_scenes.sh [N] [WAIT_SECS]
#   N: number of scenes (default 10)
#   WAIT_SECS: seconds to wait before killing (default 2)


N=10
WAIT_SECS=2
VALGRIND_MODE=0


# Parse arguments: [--valgrind] [N] [WAIT_SECS]
args=()
for arg in "$@"; do
  if [[ "$arg" == "--valgrind" ]]; then
    VALGRIND_MODE=1
  else
    args+=("$arg")
  fi
done

if [[ ${#args[@]} -ge 1 ]]; then
  N=${args[1]}
fi
if [[ ${#args[@]} -ge 2 ]]; then
  WAIT_SECS=${args[2]}
fi


SCRIPT_DIR="${0:A:h}"
SCENES_DIR="$SCRIPT_DIR/scenes"
OUT_DIR="$SCENES_DIR/generated"
BIN="$SCRIPT_DIR/../minirt"

if [[ ! -x "$BIN" ]]; then
  echo "Error: minirt binary not found at $BIN. Run 'make' first."
  exit 1
fi

rm -rf -- "$SCENES_DIR"
mkdir -p "$OUT_DIR"


# --- Scene generation helpers (from previous version) ---
rand_intv() {
  local name=$1 min=$2 max=$3
  local v=$(( min + (RANDOM % (max - min + 1)) ))
  typeset -g "$name=$v"
}
rand_floatv() {
  local name=$1 min=$2 max=$3
  local v=$(( min + (max - min) * (RANDOM + 0.0) / 32767.0 ))
  typeset -g "$name=$v"
}
rgbv() {
  local name=$1
  local r=$(( 64 + (RANDOM % 192) ))
  local g=$(( 64 + (RANDOM % 192) ))
  local b=$(( 64 + (RANDOM % 192) ))
  typeset -g "$name=$r,$g,$b"
}
pick_norm_v() {
  local name=$1
  local -a choices=(
    "0,0,-1" "0,-1,0" "0,1,0" "1,0,0" "-1,0,0" "0,0,1"
    "0.707,0.000,-0.707" "-0.707,0.000,-0.707" "0.577,0.577,-0.577"
  )
  local idx=$(( RANDOM % ${#choices[@]} ))
  typeset -g "$name=${choices[$((idx+1))]}"
}
vec3_front_v() {
  local name=$1
  local x y z
  rand_floatv x -3.0 3.0
  rand_floatv y -0.5 2.0
  rand_floatv z -12.0 -4.0
  typeset -g "$name=$x,$y,$z"
}
ground_y_v() {
  local name=$1
  rand_floatv "$name" -2.0 -0.2
}

for i in {1..$N}; do
  scene="$OUT_DIR/gen_$i.rt"
  ambient=0; rand_floatv ambient 0.05 0.2
  amb_rgb=""; rgbv amb_rgb
  fov=60; rand_intv fov 50 70

  lx=0 ly=0 lz=0 lratio=0 lrgb=""
  rand_floatv lx -2.0 2.0
  rand_floatv ly 3.0 6.0
  rand_floatv lz -6.0 -2.0
  rand_floatv lratio 0.6 1.0
  rgbv lrgb

  gy=0; ground_rgb=""
  ground_y_v gy
  rgbv ground_rgb

  cs_d=1.2; cs_rgb=""
  rand_floatv cs_d 0.9 1.6
  rgbv cs_rgb

  {
    echo "# auto-generated scene $i"
    echo "A $ambient $amb_rgb"
    echo "C 0,1.0,2.5 0,0,-1 $fov"
    echo "L $lx,$ly,$lz $lratio $lrgb"
    echo "pl 0,$gy,0 0,1,0 $ground_rgb"
    echo "sp 0,0.6,-6.0 $cs_d $cs_rgb"

    obj_count=0; rand_intv obj_count 2 4
    j=1
    while (( j <= obj_count )); do
      choice=0; rand_intv choice 0 2
      if (( choice == 0 )); then
        pos=""; vec3_front_v pos
        d=0; rand_floatv d 0.6 2.5
        color=""; rgbv color
        echo "sp $pos $d $color"
      elif (( choice == 1 )); then
        pos=""; vec3_front_v pos
        d=0; rand_floatv d 0.5 1.6
        h=0; rand_floatv h 1.0 4.0
        color=""; rgbv color
        echo "cy $pos 0,1,0 $d $h $color"
      else
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

echo "[INFO] Generated $N scenes. Running minirt $N times, $WAIT_SECS seconds each."


fail=0

for i in {1..$N}; do
  scene="$OUT_DIR/gen_$i.rt"
  echo "[Run $i/$N] $scene"
  if [[ $VALGRIND_MODE -eq 1 ]]; then
    VLOG="$OUT_DIR/valgrind_$i.log"
    RAW_LOG="$OUT_DIR/minirt_run_$i.log"
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
    wait $pid 2>/dev/null
    kill $waiter 2>/dev/null || true
    # Check valgrind log for leaks (ignore still reachable)
    if [[ -f "$VLOG" ]]; then
      definitely_lost=$(grep "definitely lost:" "$VLOG" | awk '{print $4+0}')
      indirectly_lost=$(grep "indirectly lost:" "$VLOG" | awk '{print $4+0}')
      possibly_lost=$(grep "possibly lost:" "$VLOG" | awk '{print $4+0}')
      errors=$(grep -E "ERROR SUMMARY: [1-9][0-9]* errors" "$VLOG" | wc -l)
      if [[ "$definitely_lost" -gt 0 || "$indirectly_lost" -gt 0 || "$possibly_lost" -gt 0 || "$errors" -gt 0 ]]; then
        fail=$((fail+1))
        echo "  [FAIL] Leaks or errors detected (see $VLOG)"
      else
        ok=$((ok+1))
        echo "  [OK] No leaks."
      fi
    else
      fail=$((fail+1))
      echo "  [FAIL] No valgrind log found."
    fi
  else
    RAW_LOG="$OUT_DIR/minirt_run_$i.log"
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
    wait $pid 2>/dev/null
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
  echo "Valgrind logs in $OUT_DIR"
else
  echo "\nScenes run complete. OK: $ok, Fail: $fail"
fi
exit 0
