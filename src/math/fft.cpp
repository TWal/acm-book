const double PI = acos(-1);
typedef complex<double> cpx;

//For NTT, use findNTTParams.py to find A and R
const lli A = 12;
const lli P = A*(1<<10) + 1; //prime number
const lli R = 10302; //primitive root

template<lli P>
struct Fp {
    lli n;
    Fp() : n(0) {}
    Fp(lli n_) : n(n_%P) {}
    inline Fp operator+(Fp other) const { return Fp(n + other.n); }
    inline Fp operator-(Fp other) const { return Fp(n - other.n + P); }
    inline Fp operator*(Fp other) const { return Fp(n * other.n); }
    inline Fp operator/(Fp other) const { return (*this) * other.pow(P-2); };
    Fp pow(lli b) const {
        if(b == 0) return Fp(1);
        Fp rec = pow(b/2);
        return b%2 == 0 ? rec*rec : (*this)*rec*rec;
    };
};

template<typename T> void fillPrimRoots(T* vec, lli N, bool conj) {}

template<> void fillPrimRoots<cpx>(cpx* vec, lli N, bool conj) {
    double sign = conj ? -1 : 1;
    FOR(i, N/2) vec[i] = polar(1.0, sign * 2 * PI * i / N);
}

template<> void fillPrimRoots<Fp<P>>(Fp<P>* vec, lli N, bool conj) {
    Fp<P> current(1), primRoot(R);
    if(conj) primRoot = Fp<P>(1)/primRoot;
    FOR(i, N/2) vec[i] = current, current = primRoot*current;
}

template<typename T>
struct FFT {
    lli base;
    lli N;
    vector<T> rt;
    vector<T> rtconj;
    vi rev;
    FFT(lli _base) : base(_base), N(1<<_base), rt(N, 0), rtconj(N), rev(N) {
        FOR(i, N) rev[i] = (rev[i >> 1] >> 1) + ((i&1) << (base-1));
        fillPrimRoots<T>(rt.data() + N/2, N, false);
        fillPrimRoots<T>(rtconj.data() + N/2, N, true);
        FORD(i, 0, N/2) {
            rt[i]     = rt[2*i];
            rtconj[i] = rtconj[2*i];
        }
    }

    void fft(vector<T>& a, bool inv = false) const {
        assert(a.size() == N);
        fft(a.data(), inv);
    }

    void fft(T* a,  bool inv = false) const {
        const T* roots = inv ? rtconj.data() : rt.data();
        FOR(i, N) if(i < rev[i]) swap(a[i], a[rev[i]]);
        for(lli k = 1; k < N; k <<= 1) for(lli i = 0; i < N; i += 2*k) FOR(j, k) {
            T z = a[i+j+k] * roots[j+k];
            a[i+j+k] = a[i+j] - z;
            a[i+j]   = a[i+j] + z;
        }
        T invN = T(1)/T(N);
        if(inv) FOR(i, N) a[i] = a[i] * invN;
    }

    template<typename U>
    void pad(vector<U>& a) const {
        a.reserve(N);
        while(a.size() < N) a.pb(0);
    }
};

//Fast FFT multiplication using complex numbers
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
