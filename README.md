# 4-hour CPU HPC mini-project: Memory access and blocked matrix multiplication

**Research connection:** Your existing finite-element/CFD work makes numerical performance a natural learning target. This project studies **dense** matrix multiplication as a manageable cache experiment; it is **not** an implementation or performance result of ANSYS/FEA, or a claim about sparse solver performance.

## Goal

Build, verify, profile and benchmark three mathematically equivalent algorithms for `C = A × B`, storing all matrices in C++ row-major `std::vector<double>`:

1. `ijk`: classic row-column dot products; inner loop walks down columns of B.
2. `ikj`: inner loop traverses adjacent elements of B and C, reusing one A element.
3. `blocked`: repeated multiplication of smaller A and B tiles accumulating into a C tile.

There is **no promise** that tiling is fastest on every CPU, especially compared with auto-vectorized `ikj`.

## First 10 minutes: build and run

On a Mac, open Terminal and change to the directory containing this file and `hpc_lab.cpp`.

```sh
c++ --version
c++ -O3 -std=c++17 -Wall -Wextra -o hpc_lab hpc_lab.cpp
./hpc_lab 256 32 5
```

On Linux, equivalent `g++` or `clang++` works. On Windows, WSL with `g++` is one practical option. If `c++` is missing on Mac, install Apple Command Line Tools using `xcode-select --install`.

Arguments: `n` (matrix dimension), `tile` (tile edge), `repeats` (odd number; each method is warmed up once and timed repeatedly); optional `--csv` produces one machine-readable line.

## Four-hour curriculum

- 00:00–00:40. Build the program. Explain row-major offsets `i*n+j` and predict the memory-access path of `B[k*n+j]` for fixed i,j vs fixed i,k.
- 00:40–01:30. Compare `ijk` and `ikj` for `n=256` and `n=512`. Explain data locality and speedup without promising a fixed ratio.
- 01:30–02:40. Change tile sizes 8/16/32/64. Run repeated tests and record results. Investigate why different sizes vary.
- 02:40–03:20. Vocabulary check: compiler flags, CPU, cache, row-major, SIMD, BLAS, FFT, MPI, OpenMP, CUDA, profiler, memory bandwidth, latency and throughput, etc. Explain concepts; do not claim mastery of technologies you have not used.
- 03:20–04:00. Write a short technical report from real measurements; publish the source, build command, raw CSV, analysis and limitations.

## Automated experiment

```sh
bash benchmark.sh
```

This builds the code and runs 8 configurations (2 matrix sizes × 4 tile sizes), with 5 repetitions of each of the 3 methods after a warm-up, producing `results.csv`.

### Fair measurement checklist

- Compare methods on the same computer, compiler and optimization flags (`-O3`) and identical generated input.
- Avoid other demanding tasks when benchmarking. Repeat if measurements fluctuate significantly.
- `benchmark.sh` runs the `ijk` baseline for each tile size; the repeated baseline measurements also reveal run-to-run variability.
- A compiler may auto-vectorize; this code does not create explicit threads. Time includes zeroing output matrix C.
- `PASS` uses a scale-aware tolerance. Floating-point reduction order may cause small differences; inspect both reported absolute errors and the failure counts.
- This small benchmark is not a substitute for a hardware profiler (`perf` on supported Linux systems or Instruments on macOS), and doesn't on its own prove cache misses are the sole cause.

## Experiment interpretation

For each fixed n, use the `ikj_ms` and `blocked_ms` in the CSV. `ijk_ms/ikj_ms` and `ijk_ms/blocked_ms` are speedups **relative to ijk**, while `ikj_ms/blocked_ms` measures whether tiling improves upon loop reordering. Choose the tile size based on repeatable measurements, not a guessed cache size.

## Future bridge to finite elements

After this project, explore sparse matrix-vector multiplication `y=K*x` in CSR form, then a conjugate gradient method for an appropriate symmetric positive definite test system. Verify assumptions; not every FE system is SPD. FFT would be a separate project built around sampled pulsatile signals, rather than a feature of current steady-state CFD.
