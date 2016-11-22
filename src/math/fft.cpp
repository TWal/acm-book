const double PI = 3.141592653589793238460;

typedef complex<double> cpx;
typedef std::valarray<cpx> CArray;

void fft(CArray& x, bool inv=false, bool once=true) {
    const size_t N = x.size();
    assert((N & (N-1)) == 0); // Is N a power of two?
    if (N <= 1) return;

    CArray even = x[std::slice(0, N/2, 2)];
    CArray odd  = x[std::slice(1, N/2, 2)];

    fft(even, inv, false);
    fft(odd, inv, false);

    int dir = inv ? 1 : -1;

    for(size_t k = 0; k < N/2; ++k) {
        cpx t = std::polar(1.0, -2 * dir * PI * k / N) * odd[k];
        x[k]     = even[k] + t;
        x[k+N/2] = even[k] - t;
    }

    if(once && inv) x /= N;
}

int ilog2(int x) { return 31 - __builtin_clz(x); }

//Works well if the coefficients are <= 10^6
vi polymult(vi a, vi b) {
    size_t N = 1 << (ilog2(a.size() + b.size() - 1) + 1);
    CArray a2(N), b2(N); vi c(N);
    FOR(i, a.size()) a2[i] = a[i];
    FOR(i, b.size()) b2[i] = b[i];
    fft(a2); fft(b2); a2 *= b2; fft(a2, true);
    FOR(i, N) c[i] = (lli)floor(a2[i].real()+0.5);
    return c;
}

