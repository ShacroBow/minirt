#!/usr/bin/env zsh
set -euo pipefail

HEADER=minirt.h
ITERATIONS=${1:-30}

# Added runtime options
RUN_MODE=0
QUICK_MODE=0
SCENE_FILE="scene.rt"
RUN_WAIT_SECS=2
MAX_RENDER_WAIT=30  # safety cap per run
SCRIPT_DIR="${0:A:h}"  # directory of this script

# Seed random number generator
RANDOM=$$$(date +%s)

# Simple argument parsing
for arg in "$@"; do
  case "$arg" in
    --run) RUN_MODE=1 ;;
    --quick) QUICK_MODE=1 ;;
    --scene=*) SCENE_FILE="${arg#*=}" ;;
    --wait=*) RUN_WAIT_SECS="${arg#*=}" ;;
    --iters=*) ITERATIONS="${arg#*=}" ;;
    *) 
      if [[ "$arg" =~ ^[0-9]+$ ]]; then
        ITERATIONS="$arg"
      fi
      ;;
  esac
done

# Canonical "natural" defaults (restore target)
NAT_WIDTH=255
NAT_HEIGHT=255
NAT_ENABLE_AA=0
NAT_AA_SAMPLES=4
NAT_ENABLE_GAMMA=1
NAT_DISPLAY_GAMMA=2.2
NAT_ENABLE_POST_AA=0
NAT_POST_AA_SIZE=8
NAT_EDGE_AA_STRENGTH=0.85
NAT_EDGE_AA_THRESHOLD=25.0

# --- Helpers ---
rebuild() { make -s re >/dev/null; }

# Launch and auto-close minirt like gen script: wait for "Done rendering." then delay and kill
run_minirt_once() {
  local scene="$1"
  local log_file="$SCRIPT_DIR/scenes/stress_run.log"  # log now stored in scenes/
  rm -f "$log_file"  # Remove old log
  if [[ ! -x ./minirt ]]; then
    echo "[RUN] Binary ./minirt missing; skipping run." >&2
    return 1
  fi
  if [[ ! -f "$scene" ]]; then
    echo "[RUN] Scene '$scene' not found; skipping run." >&2
    return 1
  fi
  : > "$log_file"
  ./minirt "$scene" > "$log_file" 2>&1 &
  local pid=$!
  local start_ts=$(date +%s)
  (
    while ! grep -q 'Done rendering\.' "$log_file" 2>/dev/null; do
      sleep 0.2
      if ! kill -0 $pid 2>/dev/null; then exit; fi
      # timeout safety
      if (( $(date +%s) - start_ts > MAX_RENDER_WAIT )); then
        echo "[RUN] Timeout reached; terminating pid $pid" >&2
        break
      fi
    done
    sleep "$RUN_WAIT_SECS"
    kill $pid 2>/dev/null || true
  ) &
  local waiter=$!
  wait $pid 2>/dev/null || true
  kill $waiter 2>/dev/null || true
  if grep -q 'Done rendering\.' "$log_file" 2>/dev/null; then
    echo "[RUN] Completed render (log: $log_file)"
  else
    echo "[RUN] Render ended without completion message (log: $log_file)" >&2
  fi
}

set_define() {
  local name="$1" value="$2"
  awk -v n="$name" -v v="$value" '
    BEGIN{done=0}
    /^[[:space:]]*#define[[:space:]]+/ {
      if(!done && $2==n){ printf "# define %s %s\n", n, v; done=1; next }
    }
    /^[[:space:]]*#?[[:space:]]*define[[:space:]]+/ {
      if(!done && $3==n){ printf "# define %s %s\n", n, v; done=1; next }
    }
    {print}
  ' "$HEADER" > "$HEADER.tmp" && mv "$HEADER.tmp" "$HEADER"
}

set_defaults() {
  set_define WIDTH $NAT_WIDTH
  set_define HEIGHT $NAT_HEIGHT
  set_define ENABLE_AA $NAT_ENABLE_AA
  set_define AA_SAMPLES $NAT_AA_SAMPLES
  set_define ENABLE_GAMMA $NAT_ENABLE_GAMMA
  set_define DISPLAY_GAMMA $NAT_DISPLAY_GAMMA
  set_define ENABLE_POST_AA $NAT_ENABLE_POST_AA
  set_define POST_AA_SIZE $NAT_POST_AA_SIZE
  set_define EDGE_AA_STRENGTH $NAT_EDGE_AA_STRENGTH
  set_define EDGE_AA_THRESHOLD $NAT_EDGE_AA_THRESHOLD
}

# Random generators (range-based)
rand_int_range() { 
  local min=$1 max=$2
  RANDOM=$((RANDOM + $$))  # Add more entropy
  echo $(( min + RANDOM % (max - min + 1) ))
}
rand_width() { rand_int_range 1 1024; }
rand_height() { rand_int_range 1 1024; }
rand_bool() { 
  RANDOM=$((RANDOM + $$))
  echo $(( RANDOM % 2 ))
}
# Gamma in [1.6,2.4] step 0.1 without bc
rand_gamma() { 
  RANDOM=$((RANDOM + $$))
  local d=$((16 + RANDOM % 9))
  printf '%d.%d' $((d/10)) $((d%10))
}
# AA samples 0..64 (guard will clamp 0 to 1 in header). Bias toward lower values slightly.
rand_aa_samples() { 
  RANDOM=$((RANDOM + $$))
  local r=$(( RANDOM % 100 ))
  if (( r < 60 )); then 
    rand_int_range 0 16
  else 
    rand_int_range 17 64
  fi 
}

run_case() {
  printf '\n[CASE] %s\n' "$*"
  set_defaults
  for kv in "$@"; do
    local name=${kv%%=*}; local value=${kv#*=}; set_define "$name" "$value"
  done
  rebuild && printf '[OK] Build succeeded.\n'
  if (( RUN_MODE )); then
    run_minirt_once "$SCENE_FILE" || true
  fi
}

# Deterministic baseline tests (skip if quick mode)
if [[ $QUICK_MODE -eq 0 ]]; then
  run_case WIDTH=1 HEIGHT=1 ENABLE_AA=0 AA_SAMPLES=0
  run_case WIDTH=7 HEIGHT=13 ENABLE_AA=1 AA_SAMPLES=1
  run_case WIDTH=319 HEIGHT=53 ENABLE_AA=1 AA_SAMPLES=3
  run_case WIDTH=1024 HEIGHT=1024 ENABLE_AA=1 AA_SAMPLES=64 ENABLE_GAMMA=1 DISPLAY_GAMMA=2.2
  run_case WIDTH=640 HEIGHT=480 ENABLE_AA=1 AA_SAMPLES=0
  run_case ENABLE_GAMMA=0 DISPLAY_GAMMA=2.2
  run_case ENABLE_GAMMA=1 DISPLAY_GAMMA=1.8
fi

# Random fuzz
printf '\n[INFO] Starting %d randomized config iterations...\n' "$ITERATIONS"
idx=1
while (( idx <= ITERATIONS )); do
  # Force fresh random values each iteration
  RANDOM=$((RANDOM + idx + $$))
  W=$(rand_width)
  RANDOM=$((RANDOM + 1))
  H=$(rand_height) 
  RANDOM=$((RANDOM + 1))
  AA_EN=$(rand_bool)
  RANDOM=$((RANDOM + 1))
  GM_EN=$(rand_bool)
  RANDOM=$((RANDOM + 1))
  AAS=$(rand_aa_samples)
  RANDOM=$((RANDOM + 1))
  GMA=$(rand_gamma)
  run_case WIDTH=$W HEIGHT=$H ENABLE_AA=$AA_EN AA_SAMPLES=$AAS ENABLE_GAMMA=$GM_EN DISPLAY_GAMMA=$GMA
  printf '[PROGRESS] %d/%d\r' $idx $ITERATIONS
  (( idx++ ))
  sleep 0.02
done
printf '\n[INFO] Randomized stress complete.\n'

# Final restore to natural state
set_defaults
rebuild || true

# Verify restoration
chk_width=$(grep -E '^# *define +WIDTH' "$HEADER" | awk '{print $4}')
chk_height=$(grep -E '^# *define +HEIGHT' "$HEADER" | awk '{print $4}')
if [[ $chk_width == $NAT_WIDTH && $chk_height == $NAT_HEIGHT ]]; then
  echo '[SUCCESS] Header restored to natural defaults.'
else
  echo '[ERROR] Failed to restore natural defaults.' >&2
  exit 1
fi

exit 0
