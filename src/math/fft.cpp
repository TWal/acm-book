const double PI = acos(-1);
typedef complex<double> cpx;

template<int base>
struct FFT {
    static const lli N = 1 << base;
    cpx rt[N];
    cpx rtconj[N];
    int rev[N];
    FFT() {
        FOR(i, N) rev[i] = 0;
        FOR(i, N) rev[i] = (rev[i >> 1] >> 1) + ((i&1) << (base-1));
        FOR(i, N/2) rt[i+N/2] = polar(1.0, 2 * PI * i / N);
        FORD(i, 0, N/2) rt[i] = rt[2*i];
        FOR(i, N) rtconj[i] = conj(rt[i]);
    }

    void fft(const vector<cpx>& a, vector<cpx>& f, bool inv = false) {
        assert(a.size() == N && f.size() == N);
        cpx* roots = inv ? rtconj : rt;
        FOR(i, N) f[i] = a[rev[i]];
        for(lli k = 1; k < N; k <<= 1) for(lli i = 0; i < N; i += 2*k) FOR(j, k) {
            cpx z = f[i+j+k] * roots[j+k];
            f[i+j+k] = f[i+j] - z;
            f[i+j]   = f[i+j] + z;
        }
        if(inv) FOR(i, N) f[i] /= N;
    }

    vi mult(const vi& a, const vi& b) {
        assert(a.size() == N && b.size() == N);
        vector<cpx> f(N), c(N);
        FOR(i, N) c[i] = cpx(a[i], b[i]);
        fft(c, f);
        FOR(i, N) {
            lli j = (N - i) & (N - 1);
            c[i] = (f[j] * f[j] - conj(f[i] * f[i])) * cpx(0, -0.25/N);
        }
        fft(c, f);
        vi res(N);
        FOR(i, N) res[i] = (lli)round(f[i].real());
        return res;
    }

    void pad(vi& a) {
        a.reserve(N);
        while(a.size() < N) a.pb(0);
    }
};

