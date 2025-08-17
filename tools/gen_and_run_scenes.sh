#!/usr/bin/env zsh
set -euo pipefail

# Generate N random scenes with visible geometry and (optionally) run each
# briefly using a timeout so the window closes automatically, or wait for
# the program to print "Done rendering." then close it gracefully.
# Usage:
#   tools/gen_and_run_scenes.sh [N] [--valgrind] [--wait5] [--time=<dur>|time=<dur>] [--maxwait=<dur>]
# Flags:
#   --valgrind          Run each scene under valgrind and capture its log.
#   --wait5             Wait 5s after "Done rendering." before closing.
#   --time=<dur>        Wait <dur> after "Done rendering." before closing (e.g. 5, 5s, 0.2).
#   --maxwait=<dur>     Max time to wait for the "Done rendering." line (default 120s).
# Env:
#   TIMEOUT=5s            (legacy fallback when not detecting completion)
#   RENDER_WAIT_MAX=120s  (max time to wait for "Done rendering." line)

# Defaults
N=10
VALGRIND_MODE=0
WAIT_AFTER_DONE="0.5"  # seconds; applies to both modes
CLOSE_GRACE="0.2"      # seconds; how long to wait after TERM before KILL (fixed)
VG_EXIT_WAIT="8"       # seconds to wait for valgrind to flush and exit
# Allow overriding via env; provide longer default to handle heavy renders
: ${RENDER_WAIT_MAX:=120s}
for arg in "$@"; do
  if [[ "$arg" == "--valgrind" ]]; then
    VALGRIND_MODE=1
  elif [[ "$arg" == "--wait5" ]]; then
    WAIT_AFTER_DONE="5"
  elif [[ "$arg" == --time=* ]] || [[ "$arg" == time=* ]]; then
    WAIT_AFTER_DONE="${arg#*=}"
  elif [[ "$arg" == --maxwait=* ]]; then
    RENDER_WAIT_MAX="${arg#--maxwait=}"
  elif [[ "$arg" == <-> ]]; then
    N=$arg
  fi

done

# Resolve paths relative to this script's directory
SCRIPT_DIR="${0:A:h}"
SCENES_DIR="$SCRIPT_DIR/scenes"
OUT_DIR="$SCENES_DIR/generated"
RUN_LOG_DIR="$SCRIPT_DIR/logs/run"
VALGRIND_LOG_DIR="$SCRIPT_DIR/logs/valgrind"
BIN="./minirt"
BIN_NAME="${BIN##*/}"

# Recreate scenes directory cleanly, then the output subdir
rm -rf -- "$SCENES_DIR"
mkdir -p "$OUT_DIR"

# Prepare logs (in script directory)
rm -rf -- "$RUN_LOG_DIR"
mkdir -p "$RUN_LOG_DIR"
if [[ $VALGRIND_MODE -eq 1 ]]; then
  if ! command -v valgrind >/dev/null 2>&1; then
    echo "Valgrind not found in PATH. Install it or run without --valgrind."
    exit 1
  fi
  rm -rf -- "$VALGRIND_LOG_DIR"
  mkdir -p "$VALGRIND_LOG_DIR"
fi

# Seed RNG with time and pid (zsh RANDOM is per-shell)
RANDOM=$(( ($(date +%s) ^ $$ ^ (RANDOM<<16)) & 0x7fffffff ))

# RNG helpers using zsh global assignment; avoid subshells
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

# Convert durations like "8s" or "8" to integer seconds
_to_seconds() {
  local v="$1"
  v="${v%s}"
  if [[ -z "$v" ]]; then echo 0; else echo "$v"; fi
}

# Poll the raw log file for the completion marker within a max wait
_wait_for_done() {
  local raw_log="$1" max_wait="$2"  # seconds
  local end_ts=$(( $(date +%s) + ${max_wait%.*} ))
  while (( $(date +%s) <= end_ts )); do
    if [[ -f "$raw_log" ]] && grep -q 'Done rendering\.' "$raw_log" 2>/dev/null; then
      return 0
    fi
    sleep 0.1
  done
  return 1
}

# Graceful close helper: try ESC via xdotool, then TERM, then KILL
# Usage: _graceful_close <pid> [max_wait_after_term_seconds]
_graceful_close() {
  local pid=$1
  local max_wait="${2:-2}"
  # Try ESC via xdotool
  if command -v xdotool >/dev/null 2>&1; then
    local wids
    wids=$(xdotool search --onlyvisible --name "miniRT" 2>/dev/null || true)
    if [[ -n "${wids:-}" ]]; then
      for w in $wids; do xdotool key --window "$w" Escape || true; done
    else
      wids=$(xdotool search --name "miniRT" 2>/dev/null || true)
      for w in $wids; do xdotool key --window "$w" Escape || true; done
    fi
  fi
  # Small pre-TERM pause proportional to grace
  {
    typeset -F 3 _cg="$max_wait"
    if (( _cg < 0.1 )); then sleep 0.02; elif (( _cg < 0.3 )); then sleep 0.05; else sleep 0.1; fi
  }
  if [[ -n "${pid:-}" ]] && kill -0 "$pid" 2>/dev/null; then
    kill -TERM "$pid" 2>/dev/null || true
  fi
  # Wait up to max_wait for clean exit before KILL
  {
    typeset -F 3 _cg="$max_wait"
    local -i loops
    local interval=0.05
    loops=$(( _cg <= 0 ? 0 : (_cg + 0.049) / 0.05 ))
    local j=0
    while (( j < loops )); do
      if [[ -z "${pid:-}" ]] || ! kill -0 "$pid" 2>/dev/null; then return 0; fi
      sleep "$interval"
      (( j++ ))
    done
  }
  if [[ -n "${pid:-}" ]]; then
    kill -KILL "$pid" 2>/dev/null || true
  fi
}

# Get PIDs for windows matching name via xdotool (if available)
_get_window_pids_by_name() {
  local name="$1"
  command -v xdotool >/dev/null 2>&1 || return 1
  local wids pids pid
  wids=$(xdotool search --name "$name" 2>/dev/null || true)
  for w in $wids; do
    pid=$(xdotool getwindowpid "$w" 2>/dev/null || true)
    if [[ -n "${pid:-}" ]]; then echo "$pid"; fi
  done | sort -u
}

# Force close any window by name by killing its owning PID(s)
_force_close_windows_by_name() {
  local name="$1" grace="$2"
  local pids pid
  pids=$(_get_window_pids_by_name "$name" 2>/dev/null || true)
  for pid in $pids; do
    # Skip if PID is the valgrind supervisor
    if [[ -n "${VAL_PID:-}" && "$pid" == "$VAL_PID" ]]; then continue; fi
    _graceful_close "$pid" "$grace"
  done
}

# Find first-level children PIDs for a given PPID
_children_of() {
  ps --no-headers -o pid= --ppid "$1" 2>/dev/null | tr -s ' ' | tr -d ' '
}

# Find descendant by command name under a PPID (depth-first)
_find_descendant_by_name() {
  local root="$1" name="$2"
  local kids
  kids=$(_children_of "$root")
  if [[ -z "${kids:-}" ]]; then return 1; fi
  local pid
  for pid in $kids; do
    local comm args
    comm=$(ps -o comm= -p "$pid" 2>/dev/null | tr -d '\n') || comm=""
    args=$(ps -o args= -p "$pid" 2>/dev/null | tr -d '\n') || args=""
    if [[ "$comm" == *"$name"* ]] || [[ "$args" == *"$name"* ]]; then
      echo "$pid"; return 0
    fi
    local found
    found=$(_find_descendant_by_name "$pid" "$name" 2>/dev/null || true)
    if [[ -n "${found:-}" ]]; then echo "$found"; return 0; fi
  done
  return 1
}

# Return the first immediate child PID of a PPID, if any
_first_child() {
  local root="$1"
  local kids
  kids=$(_children_of "$root")
  for k in $kids; do echo "$k"; return 0; done
  return 1
}

# Find the deepest child (leaf) under a PPID; returns the last descendant if any
_leaf_descendant() {
  local root="$1"
  local pid="$root"
  local next
  while true; do
    next=$(_children_of "$pid" | head -n1)
    if [[ -z "${next:-}" ]]; then
      echo "$pid"
      return 0
    fi
    pid="$next"
  done
}

# Wait for a PID to exit within a timeout (seconds). Return 0 if exited.
_wait_pid_exit() {
  local pid="$1" timeout_s="$2"
  local end=$(( $(date +%s) + ${timeout_s%.*} ))
  while kill -0 "$pid" 2>/dev/null && (( $(date +%s) < end )); do
    sleep 0.05
  done
  if kill -0 "$pid" 2>/dev/null; then return 1; else return 0; fi
}

# Decide terminal sink (preserve CR to show single-line progress); if no TTY, discard
_term_sink() {
  if [[ -t 1 ]]; then
    echo "/dev/tty"
  else
    echo "/dev/null"
  fi
}

# Start a program writing to a FIFO; fan out to terminal and raw log via tee.
# Exports WRITER_PID (program pid), READER_PID (tee pipeline pid), FIFO_PATH
_start_fanned_run() {
  local raw_log="$1"; shift 1
  local fifo="$RUN_LOG_DIR/.minirt_fifo_$$.$RANDOM"
  mkfifo "$fifo"
  # Writer (exec replaces subshell => PID == program's PID)
  ( exec "$@" ) >"$fifo" 2>&1 &
  local wpid=$!
  # Reader pipeline: raw log and terminal (preserve CR)
  local sink; sink=$(_term_sink)
  cat "$fifo" | tee "$raw_log" > "$sink" &
  local rpid=$!
  # Cleanup fifo when script exits
  trap "rm -f '$fifo'" EXIT
  typeset -g WRITER_PID="$wpid"
  typeset -g READER_PID="$rpid"
  typeset -g FIFO_PATH="$fifo"
}

# Start miniRT normally with fanned IO
_start_minirt() {
  _start_fanned_run "$BIN" "$scene"
}

# Kill valgrind and its child (miniRT); avoid ghost windows
_kill_valgrind_tree() {
  local vpid=$1
  local kids=$(_children_of "$vpid")
  for c in $kids; do
    kill -TERM "$c" 2>/dev/null || true
  done
  kill -TERM "$vpid" 2>/dev/null || true
  for _ in {1..30}; do
    local alive=0
    if kill -0 "$vpid" 2>/dev/null; then alive=1; fi
    for c in $kids; do
      if kill -0 "$c" 2>/dev/null; then alive=1; fi
    done
    if (( alive == 0 )); then break; fi
    sleep 0.1
  done
  for c in $kids; do kill -KILL "$c" 2>/dev/null || true; done
  kill -KILL "$vpid" 2>/dev/null || true
}

# Ensure binary is built
make -j8 >/dev/null

ok=0
fail=0
valgrind_err=0
valgrind_leaks=0

i=1
while (( i <= N )); do
  scene="$OUT_DIR/gen_$i.rt"

  # Precompute random elements without subshells
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

  echo "[Run $i/$N] $scene"

  RAW_LOG="$RUN_LOG_DIR/gen_$i.raw"
  : >"$RAW_LOG"
  CLEAN_RUN_LOG="$RUN_LOG_DIR/gen_$i.out"

  if [[ $VALGRIND_MODE -eq 1 ]]; then
    t=${RENDER_WAIT_MAX}
    VLOG="$VALGRIND_LOG_DIR/gen_$i.log"
    # Create a FIFO and run valgrind -> fifo; then fan out fifo to terminal/raw
    FIFO_TMP_REDIRECT="$RUN_LOG_DIR/.val_fifo_$$.$RANDOM"
    mkfifo "$FIFO_TMP_REDIRECT"
    # Start valgrind (no stdbuf) to simplify PID tree
    ( exec valgrind --quiet \
        --leak-check=full --show-leak-kinds=all --track-origins=yes \
        --errors-for-leak-kinds=all --log-file="$VLOG" \
        "$BIN" "$scene" ) >"$FIFO_TMP_REDIRECT" 2>&1 &
    VAL_PID=$!
    # Reader: tee to raw and to terminal (preserve CR)
    sink=$(_term_sink)
    cat "$FIFO_TMP_REDIRECT" | tee "$RAW_LOG" > "$sink" &
    READER_PID=$!
    # Give valgrind a moment to spawn the child, then resolve child PID by name
    sleep 0.2
    MINIRT_PID=$(_find_descendant_by_name "$VAL_PID" "$BIN_NAME" 2>/dev/null || true)
    if [[ -z "${MINIRT_PID:-}" ]]; then
      MINIRT_PID=$(_first_child "$VAL_PID" 2>/dev/null || true)
    fi
    if [[ -z "${MINIRT_PID:-}" ]]; then
      MINIRT_PID=$(_leaf_descendant "$VAL_PID" 2>/dev/null || true)
      # Avoid mistaking the valgrind PID itself as the child
      if [[ "${MINIRT_PID:-}" == "$VAL_PID" ]]; then
        MINIRT_PID=""
      fi
    fi
    found=0
    if _wait_for_done "$RAW_LOG" "$(_to_seconds "$t")"; then
      found=1
    fi
    if (( found )); then
      # Give the app a moment, then request a graceful shutdown of the real child
      sleep "$WAIT_AFTER_DONE"
      _graceful_close "${MINIRT_PID:-}" "$CLOSE_GRACE"
      # As a hardening step, kill any remaining window owning PIDs named miniRT
      _force_close_windows_by_name "miniRT" "$CLOSE_GRACE" || true
      # Wait for valgrind to exit on its own to flush logs
      if ! _wait_pid_exit "$VAL_PID" "$VG_EXIT_WAIT"; then
        # still alive: try TERM then KILL as last resort
        kill -TERM "$VAL_PID" 2>/dev/null || true
        if ! _wait_pid_exit "$VAL_PID" 2; then
          _kill_valgrind_tree "$VAL_PID"
        fi
      fi
      # Ensure reader finished consuming FIFO
      if [[ -n "${READER_PID:-}" ]]; then wait "$READER_PID" || true; fi
      rm -f "$FIFO_TMP_REDIRECT" || true
      # Clean run log: only keep Done rendering lines
      grep -F "Done rendering." "$RAW_LOG" > "$CLEAN_RUN_LOG" || :
      # Evaluate valgrind: errors or leaks => fail
      if [[ -s "$VLOG" ]]; then
        if grep -E "ERROR SUMMARY: [1-9][0-9]* errors" -q "$VLOG"; then
          echo "  Valgrind reported errors for $scene -> $VLOG"
          valgrind_err=$((valgrind_err+1))
          fail=$((fail+1))
        elif grep -Eq "(definitely|indirectly|possibly) lost: +([1-9][0-9,]*) bytes" "$VLOG" \
          || grep -Eq "[1-9][0-9,]* bytes in [1-9][0-9,]* blocks are (definitely|indirectly|possibly) lost" "$VLOG"; then
          echo "  Valgrind reported leaks for $scene -> $VLOG"
          valgrind_leaks=$((valgrind_leaks+1))
          fail=$((fail+1))
        else
          ok=$((ok+1))
        fi
      else
        echo "  Warning: Valgrind log empty for $scene (VAL_PID may have been killed too early)"
        fail=$((fail+1))
      fi
    else
      echo "  Timeout waiting for completion; stopping valgrind for $scene"
      if [[ -n "${VAL_PID:-}" ]]; then _kill_valgrind_tree "$VAL_PID"; fi
      if [[ -n "${READER_PID:-}" ]]; then wait "$READER_PID" || true; fi
      rm -f "$FIFO_TMP_REDIRECT" || true
      fail=$((fail+1))
    fi
  else
    t=${RENDER_WAIT_MAX}
    # Normal run with fanned IO
    _start_fanned_run "$RAW_LOG" "$BIN" "$scene"
    found=0
    if _wait_for_done "$RAW_LOG" "$(_to_seconds "$t")"; then
      found=1
    fi
    if (( found )); then
      sleep "$WAIT_AFTER_DONE"
      _graceful_close "${WRITER_PID:-}" "$CLOSE_GRACE"
      _force_close_windows_by_name "miniRT" "$CLOSE_GRACE" || true
      if [[ -n "${READER_PID:-}" ]]; then wait "$READER_PID" || true; fi
      grep -F "Done rendering." "$RAW_LOG" > "$CLEAN_RUN_LOG" || :
      ok=$((ok+1))
    else
      echo "  Timeout waiting for completion; stopping run for $scene"
      if [[ -n "${WRITER_PID:-}" ]]; then kill -TERM "$WRITER_PID" 2>/dev/null || true; fi
      sleep 0.2
      if [[ -n "${WRITER_PID:-}" ]]; then kill -KILL "$WRITER_PID" 2>/dev/null || true; fi
      if [[ -n "${READER_PID:-}" ]]; then wait "$READER_PID" || true; fi
      fail=$((fail+1))
    fi
  fi

  echo "  Raw log: $RAW_LOG"
  sleep 0.05
  (( i++ ))
done

if [[ $VALGRIND_MODE -eq 1 ]]; then
  echo "\nScenes run complete. OK: $ok, Fail: $fail, Valgrind errors: $valgrind_err, Valgrind leaks: $valgrind_leaks"
  echo "Valgrind logs: $VALGRIND_LOG_DIR"
else
  echo "\nScenes run complete. OK: $ok, Fail: $fail"
fi

exit 0
