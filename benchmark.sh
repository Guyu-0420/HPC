#!/usr/bin/env bash
set -euo pipefail
# Run from the project directory: bash benchmark.sh
c++ -O3 -std=c++17 -Wall -Wextra -o hpc_lab hpc_lab.cpp
printf 'n,tile,repeats,ijk_ms,ikj_ms,blocked_ms,ikj_speedup,blocked_speedup,ikj_max_abs,blocked_max_abs,ikj_failures,blocked_failures,status\n' > results.csv
for n in 256 512; do
  for tile in 8 16 32 64; do
    ./hpc_lab "$n" "$tile" 5 --csv >> results.csv
  done
done
printf '\nSaved results.csv\n'
