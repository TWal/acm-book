const double PI = acos(-1);
typedef complex<double> cpx;

struct FFT {
    lli base;
    lli N;
    vector<cpx> rt;
    vector<cpx> rtconj;
    vi rev;
    FFT(lli _base) : base(_base), N(1<<_base), rt(N), rtconj(N), rev(N) {
        FOR(i, N) rev[i] = 0;
        FOR(i, N) rev[i] = (rev[i >> 1] >> 1) + ((i&1) << (base-1));
        FOR(i, N/2) rt[i+N/2] = polar(1.0, 2 * PI * i / N);
        FORD(i, 0, N/2) rt[i] = rt[2*i];
        FOR(i, N) rtconj[i] = conj(rt[i]);
    }

    void fft(vector<cpx>& a,  bool inv = false) {
        assert(a.size() == N);
        cpx* roots = inv ? rtconj.data() : rt.data();
        FOR(i, N) if(i < rev[i]) swap(a[i], a[rev[i]]);
        for(lli k = 1; k < N; k <<= 1) for(lli i = 0; i < N; i += 2*k) FOR(j, k) {
            cpx z = a[i+j+k] * roots[j+k];
            a[i+j+k] = a[i+j] - z;
            a[i+j]   = a[i+j] + z;
        }
        if(inv) FOR(i, N) a[i] /= N;
    }

    vi mult(const vi& a, const vi& b) {
        assert(a.size() == N && b.size() == N);
        vector<cpx> c(N);
        FOR(i, N) c[i] = cpx(a[i], b[i]);
        fft(c);
        vector<cpx> f = c;
        FOR(i, N) {
            lli j = (N - i) & (N - 1);
            c[i] = (f[j] * f[j] - conj(f[i] * f[i])) * cpx(0, -0.25/N);
        }
        fft(c);
        vi res(N);
        FOR(i, N) res[i] = (lli)round(c[i].real());
        return res;
    }

    void pad(vi& a) {
        a.reserve(N);
        while(a.size() < N) a.pb(0);
    }
};
