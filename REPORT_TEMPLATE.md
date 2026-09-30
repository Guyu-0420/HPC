# Performance optimization report (fill with your own results)

**Hardware:** [CPU model; RAM; operating system]  
**Compiler:** [`c++ --version` output]  
**Build command:** `c++ -O3 -std=c++17 -Wall -Wextra -o hpc_lab hpc_lab.cpp`  
**Data type:** double; **Input:** deterministic; **Threads:** no explicit threading

## 1. Bottleneck hypothesis

`ijk` walks down B columns, separated by n double elements in row-major memory. This may reduce spatial locality compared with the contiguous traversal of B and C in `ikj`. Hypothesis, not proof of hardware-cache event counts.

## 2. Implementation changes

[Explain `ijk` -> `ikj`, then tiling with tested sizes. Explain what `A[i*n+k]` reuses.]

## 3. Measurements

Attach the unedited `results.csv`. Summarize the median timings for n=256 and n=512 and compare `ikj` vs baseline and `blocked` vs `ikj`.

**Fastest measured configuration:** [fill after running]  
**Speedup vs ijk baseline:** [fill after running]  
**Speedup vs ikj, if applicable:** [fill after running]

## 4. Correctness

[State max absolute error and failure count for each optimization. Explain the mixed absolute/relative tolerance used.]

## 5. Explanation and limitations

[Discuss whether data-locality hypothesis matches measurements, compiler vectorization, cache-size uncertainty, measurement noise, and the fact that this is a dense CPU single-threaded teaching project, not a GPU kernel or finite-element solver.]

## 6. Resume bullet draft — use only after you actually finish

**CPU numerical kernel optimization (C++17):** Implemented and benchmarked baseline, loop-reordered and tiled double-precision matrix-multiplication kernels. On [your actual CPU] for [n], obtained [measured speedup]× relative to baseline using [verified change], with numerical equivalence checked using scale-aware error tolerances; documented reproducible measurement conditions and raw results.
