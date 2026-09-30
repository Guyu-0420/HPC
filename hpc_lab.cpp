// 4-hour HPC mini-project: cache-aware CPU matrix multiplication.
// macOS/Linux: c++ -O3 -std=c++17 -Wall -Wextra -o hpc_lab hpc_lab.cpp
// Usage: ./hpc_lab [n=256] [tile=32] [repeats=5] [--csv]
// Deliberately single-threaded C++; compiler auto-vectorization may occur.
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using Matrix = std::vector<double>;

// Row-major: element (row, col) resides at row*n+col.
void baseline_ijk(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    std::fill(C.begin(), C.end(), 0.0);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[static_cast<size_t>(i)*n+j] += A[static_cast<size_t>(i)*n+k]*B[static_cast<size_t>(k)*n+j];
}

// Inner loop traverses adjacent B and C elements; reuses scalar A[i,k].
void reordered_ikj(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    std::fill(C.begin(), C.end(), 0.0);
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            const double a = A[static_cast<size_t>(i)*n+k];
            for (int j = 0; j < n; ++j)
                C[static_cast<size_t>(i)*n+j] += a*B[static_cast<size_t>(k)*n+j];
        }
}

// Three-dimensional tiling; each innermost j loop still accesses contiguous rows.
// Loop nesting ii->jj->kk concentrates accumulation in a C submatrix.
void blocked(const Matrix& A, const Matrix& B, Matrix& C, int n, int tile) {
    std::fill(C.begin(), C.end(), 0.0);
    for (int ii = 0; ii < n; ii += tile)
        for (int jj = 0; jj < n; jj += tile)
            for (int kk = 0; kk < n; kk += tile)
                for (int i = ii; i < std::min(ii+tile, n); ++i)
                    for (int k = kk; k < std::min(kk+tile, n); ++k) {
                        const double a = A[static_cast<size_t>(i)*n+k];
                        for (int j = jj; j < std::min(jj+tile, n); ++j)
                            C[static_cast<size_t>(i)*n+j] += a*B[static_cast<size_t>(k)*n+j];
                    }
}

template<class F>
double median_ms(F&& f, int repeats) {
    std::vector<double> samples;
    samples.reserve(repeats);
    for (int r=0; r<repeats; ++r) {
        const auto start=std::chrono::steady_clock::now();
        f();
        const auto end=std::chrono::steady_clock::now();
        samples.push_back(std::chrono::duration<double,std::milli>(end-start).count());
    }
    std::sort(samples.begin(), samples.end());
    return samples[repeats/2]; // Odd repeats required.
}

struct Error {double max_abs=0, max_rel=0; size_t failures=0;};
Error compare(const Matrix& ref, const Matrix& test) {
    constexpr double ABS_TOL=1e-8, REL_TOL=1e-10;
    Error e;
    for (size_t p=0; p<ref.size(); ++p) {
        const double abs=std::abs(ref[p]-test[p]);
        const double rel=abs/std::max(std::abs(ref[p]),1e-12);
        e.max_abs=std::max(e.max_abs,abs);
        e.max_rel=std::max(e.max_rel,rel);
        if (!std::isfinite(test[p]) || abs>ABS_TOL+REL_TOL*std::abs(ref[p])) ++e.failures;
    }
    return e;
}

int parse_int(const char* text, const char* label, int low, int high) {
    const std::string s(text);
    size_t pos=0;
    int out;
    try { out=std::stoi(s,&pos); }
    catch (...) {throw std::invalid_argument(std::string(label)+" must be an integer");}
    if (pos!=s.size() || out<low || out>high)
        throw std::invalid_argument(std::string(label)+" must be between "+std::to_string(low)+" and "+std::to_string(high));
    return out;
}

int main(int argc, char** argv) {
    try {
        if (argc>5) throw std::invalid_argument("Usage: ./hpc_lab [n] [tile] [repeats: odd] [--csv]");
        const int n=argc>=2?parse_int(argv[1],"n",8,1536):256;
        const int tile=argc>=3?parse_int(argv[2],"tile",1,256):32;
        const int repeats=argc>=4?parse_int(argv[3],"repeats",1,15):5;
        if (repeats%2==0) throw std::invalid_argument("repeats must be an odd integer");
        const bool csv=argc==5 && std::string(argv[4])=="--csv";
        if (argc==5 && !csv) throw std::invalid_argument("the last argument must be --csv");
        const size_t nn=static_cast<size_t>(n)*n;
        Matrix A(nn),B(nn),ref(nn),other(nn),tiled(nn);
        for (size_t p=0; p<nn; ++p) {
            A[p]=static_cast<double>((p*17+11)%101)/101.0;
            B[p]=static_cast<double>((p*13+7)%103)/103.0;
        }
        // Warm all three kernels before timings. Time includes resetting C to zero.
        baseline_ijk(A,B,ref,n);
        reordered_ikj(A,B,other,n);
        blocked(A,B,tiled,n,tile);
        const double t0=median_ms([&]{baseline_ijk(A,B,ref,n);},repeats);
        const double t1=median_ms([&]{reordered_ikj(A,B,other,n);},repeats);
        const double t2=median_ms([&]{blocked(A,B,tiled,n,tile);},repeats);
        const Error e1=compare(ref,other),e2=compare(ref,tiled);
        const bool passed=e1.failures==0 && e2.failures==0;
        const double checksum=ref[0]+other[nn/2]+tiled[nn-1];
        std::cout<<std::fixed<<std::setprecision(6);
        if (csv) {
            // No header: benchmark.sh writes it once.
            std::cout<<n<<','<<tile<<','<<repeats<<','<<t0<<','<<t1<<','<<t2<<','
                     <<t0/t1<<','<<t0/t2<<','<<e1.max_abs<<','<<e2.max_abs<<','
                     <<e1.failures<<','<<e2.failures<<','<<(passed?"PASS":"FAIL")<<'\n';
        } else {
            std::cout<<"n="<<n<<", tile="<<tile<<", repeats="<<repeats<<" (median), C++ -O3\n";
            std::cout<<"ijk baseline : "<<t0<<" ms (1.00x)\n";
            std::cout<<"ikj reordered: "<<t1<<" ms ("<<t0/t1<<"x), max_abs="<<e1.max_abs<<"\n";
            std::cout<<"blocked      : "<<t2<<" ms ("<<t0/t2<<"x), max_abs="<<e2.max_abs<<"\n";
            std::cout<<(passed?"PASS":"FAIL")<<": correctness versus ijk, checksum="<<checksum<<"\n";
        }
        return passed?0:2;
    } catch (const std::exception& e) {
        std::cerr<<"Error: "<<e.what()<<'\n';
        return 1;
    }
}
