#!/usr/bin/env zsh
set -euo pipefail

HEADER=minirt.h

backup() {
  cp -f -- "$HEADER" "$HEADER.bak"
}

restore() {
  mv -f -- "$HEADER.bak" "$HEADER"
}

rebuild() {
  echo "==> make re"
  make re -j8 >/dev/null
}

set_define() {
  local name="$1" value="$2"
  awk -v n="$name" -v v="$value" '
    BEGIN{done=0}
    {
      if(done==0){
        if($1=="#define" && $2==n){
          printf "#define %s %s\n", n, v; done=1; next
        }
        if($1=="#" && $2=="define" && $3==n){
          printf "# define %s %s\n", n, v; done=1; next
        }
      }
      print
    }
  ' "$HEADER" > "$HEADER.tmp" && mv "$HEADER.tmp" "$HEADER"
}

run_case() {
  echo "\n--- Case: $* ---"
  backup
  for kv in "$@"; do
    name=${kv%%=*}
    value=${kv#*=}
    set_define "$name" "$value"
  done
  rebuild
  restore
}

# Matrix of tests
run_case WIDTH=1 HEIGHT=1 ENABLE_AA=0
run_case WIDTH=7 HEIGHT=13 ENABLE_AA=1 AA_SAMPLES=1
run_case WIDTH=319 HEIGHT=53 ENABLE_AA=1 AA_SAMPLES=3
run_case WIDTH=1024 HEIGHT=1024 ENABLE_AA=1 AA_SAMPLES=64
run_case WIDTH=123 HEIGHT=77 ENABLE_AA=1 AA_SAMPLES=11
run_case WIDTH=640 HEIGHT=480 ENABLE_AA=1 AA_SAMPLES=0
run_case ENABLE_GAMMA=0 DISPLAY_GAMMA=2.2
run_case ENABLE_GAMMA=1 DISPLAY_GAMMA=1.8

# Edge AA grid irregular primes
run_case ENABLE_AA=1 AA_SAMPLES=2
run_case ENABLE_AA=1 AA_SAMPLES=5
run_case ENABLE_AA=1 AA_SAMPLES=7
run_case ENABLE_AA=1 AA_SAMPLES=17

# Odd resolutions
run_case WIDTH=17 HEIGHT=301
run_case WIDTH=319 HEIGHT=53
run_case WIDTH=53 HEIGHT=319

echo "\nAll stress cases built successfully."
