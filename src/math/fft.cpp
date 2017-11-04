const double PI = acos(-1);
typedef complex<double> cpx;

template<typename T> T kthRoot(lli k, lli N) {}

template<> cpx kthRoot<cpx>(lli k, lli N) {
    return polar(1.0, 2*PI*k/N);
}

//-- Only for NTT. Include Fp.cpp --//
//For NTT, use findNTTParams.py to find A and R
const lli A = 12;
const lli P = A*(1<<10) + 1; //prime number
const lli R = 10302; //primitive root

template<> Fp<P> kthRoot<Fp<P>>(lli k, lli N) {
    return Fp<P>(R).pow(k);
}

//-- End of "Only for NTT" --//

template<typename T>
struct FFT {
    lli base, N;
    vector<T> rt;
    vi rev;
    FFT(lli _base) : base(_base), N(1<<_base), rt(N, 0), rev(N) {
        FOR(i, N) rev[i] = (rev[i >> 1] >> 1) + ((i&1) << (base-1));
        rt[1] = 1;
        FORU(k, 1, base) {
            T w = kthRoot<T>(N>>(k+1), N);
            FORU(i, 1<<(k-1), (1<<k)) {
                rt[2*i] = rt[i];
                rt[2*i+1] = rt[i]*w;
            }
        }
    }

    void fft(vector<T>& a, bool inv = false) const {
        assert(a.size() == N);
        fft(a.data(), inv);
    }

    void fft(T* a,  bool inv = false) const {
        if(inv) reverse(a+1, a+N);
        FOR(i, N) if(i < rev[i]) swap(a[i], a[rev[i]]);
        for(lli k = 1; k < N; k <<= 1) for(lli i = 0; i < N; i += 2*k) FOR(j, k) {
            T z = a[i+j+k] * rt[j+k];
            a[i+j+k] = a[i+j] - z;
            a[i+j]   = a[i+j] + z;
        }
        T invN = T(1)/T(N);
        if(inv) FOR(i, N) a[i] = a[i] * invN;
    }

    template<typename U>
    void pad(vector<U>& a) const {
        a.resize(N, 0);
    }
};

// "2 in 1" FFT polynomial multiplication
vi multFFT(const FFT<cpx>& fft, const vi& a, const vi& b) {
    lli N = fft.N;
    assert(a.size() == N && b.size() == N);
    vector<cpx> c(N);
    FOR(i, N) c[i] = cpx(a[i], b[i]);
    fft.fft(c);
    vector<cpx> f = c;
    FOR(i, N) {
        lli j = (N - i) & (N - 1);
        c[i] = (f[j] * f[j] - conj(f[i] * f[i])) * cpx(0, -0.25/N);
    }
    fft.fft(c);
    vi res(N);
    FOR(i, N) res[i] = (lli)round(c[i].real());
    return res;
}
