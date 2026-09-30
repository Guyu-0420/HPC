# 20 core terms from HPC-oriented graduate job descriptions

**Your goal today:** Explain at least 12/20 in your own words and connect at least 6 to the mini-project. **Understanding the term ≠ experience developing that technology.** The five job families discussed in this conversation emphasize different subsets; this sheet is a learning map, not a claim that each employer requires all 20.

| Term | Plain-language meaning | Link to today's project |
|---|---|---|
| 1. C++ | Compiled programming language often used for performance-sensitive kernels. | `hpc_lab.cpp` |
| 2. Linux | Operating system widely used on HPC servers; terminal, toolchain and performance tools matter. | Compile/run same project on Linux or macOS. |
| 3. Compiler | Translates source code into executable machine instructions. | `c++` |
| 4. `-O3` | Compiler optimization level, which may vectorize loops. | Compare fixed flags across algorithms. |
| 5. CPU | General-purpose processor used here to execute code. | All three kernels. |
| 6. Cache (L1/L2/L3) | Small, relatively fast memory close to CPU, with different sizes/speeds. | Motivates data locality and tiling. |
| 7. RAM | Main memory, typically slower than cache to access. | Matrices live in system memory. |
| 8. Row-major | Consecutive columns in one row are adjacent in memory. | `B[k*n+j]` for increasing `j`. |
| 9. Spatial locality | Nearby addresses are accessed close together in time. | `ikj` contiguous B and C traversal. |
| 10. Temporal locality | Recently used data is used again soon. | Reuse `A[i*n+k]` across j; reuse C tile. |
| 11. Blocking/Tiling | Organize computation into smaller chunks to improve reuse. | `blocked()` and `tile`. |
| 12. Profiling | Measure where program spends time / uses hardware resources. | Timings now; hardware profiler later. |
| 13. Latency | Time for one operation or request to complete. | Milliseconds per whole matrix multiplication. |
| 14. Throughput | Work completed per unit time. | Floating-point operations per second (with a caveat for exact operation counts). |
| 15. Memory bandwidth | Amount of data moved per second across a memory interface. | Potential bottleneck; not directly measured by stopwatch. |
| 16. SIMD | One instruction performs similar arithmetic on multiple data lanes. | Compiler may auto-vectorize inner loops. |
| 17. BLAS | Standard interface families for basic linear algebra subroutines, e.g. GEMM. | Production libraries for dense matrix multiplication. |
| 18. FFT | Fast Fourier transform computes a discrete Fourier transform efficiently. | Possible *separate* pulsatile-signal project; not this dense matrix kernel. |
| 19. OpenMP | API/compiler directives for shared-memory parallel programming. | Extension, not used in today's single-thread teaching program. |
| 20. MPI / CUDA | MPI coordinates programs across processes/machines; CUDA is NVIDIA's GPU programming platform. | Extensions, not used in today's CPU program. |

**Extra terms** mentioned in specialized roles: `GEMM` = general dense matrix-matrix multiplication; `SpMV` = sparse matrix-vector multiplication; `CSR` = compressed sparse row storage; `NPU` = neural-network accelerator; `VTune/perf/Instruments` = examples of performance analysis tools, subject to supported hardware/OS.

## 20-question, closed-book self-test

For questions 1–20, close the table above and write one sentence for each term, plus an example when you can. Award one point only for a correct explanation, not for having heard the word. Score 12/20 meets today's vocabulary goal, not a job qualification.

1. What is C++ doing in our project? 
2. Why is Linux often mentioned in HPC JDs, even if you own a Mac?
3. What does a compiler do?
4. Why must we hold `-O3` constant for fair comparisons?
5. Does running C++ automatically mean the work runs on a GPU?
6. What is cache, compared with RAM?
7. What role does RAM play for matrices too large for cache?
8. Why does `i*n+j` represent row-major indexing?
9. Which `ikj` accesses improve spatial locality?
10. What does repeatedly using A[i,k] demonstrate?
11. Why do we test multiple tile sizes rather than declaring 32 best?
12. What does a hardware profiler provide that a stopwatch cannot?
13. What is latency in this benchmark?
14. Define throughput and give a unit.
15. What is memory bandwidth, and why is it not the same as measured wall time?
16. What does SIMD stand for in practical terms?
17. What sort of routines does BLAS standardize?
18. When would FFT connect to a *pulsatile* blood-flow problem?
19. How is OpenMP different from today’s ordinary single-thread code?
20. What is the difference between MPI and CUDA?
