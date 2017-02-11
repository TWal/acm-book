#include <precontest/template.cpp>
#include <math/egcd.cpp>
#include <math/fft.cpp>
#include <rapidcheck.h>

void testMath() {
    rc::check("egcd: gcd(a, b) divides a and b", [](lli a, lli b) {
        lli g = X(egcd(a, b));
        if(a != 0) RC_ASSERT(a%g == 0);
        if(b != 0) RC_ASSERT(b%g == 0);
    });

    rc::check("egcd: (g,u,v)=gcd(a,b) => a*u+b*v=g", [](lli a, lli b) {
        lli g, u, v;
        tie(g, u, v) = egcd(a, b);
        RC_ASSERT(a*u+b*v == g);
    });

    rc::check("fft: inverse fft", []() {
        auto doublegen = rc::gen::suchThat<double>([](double d) {
            return fabs(d) < 1e6;
        });
        auto pairgen = rc::gen::pair(doublegen, doublegen);
        auto cpxgen = rc::gen::map(pairgen, [](pair<double, double> p) {
                return cpx(p.first, p.second);
        });
        auto vigen = rc::gen::container<vector<cpx>>(cpxgen);
        vector<cpx> a = *vigen.as("a");
        FFT fft(8*sizeof(size_t) - __builtin_clzll(a.size()));
        while(a.size() != fft.N) a.push_back(cpx(0, 0));
        vector<cpx> b = a;
        fft.fft(b, false);
        fft.fft(b, true);
        FOR(i, a.size()) {
            RC_ASSERT(abs(a[i]-b[i]) < 1e-5);
        }
    });

    rc::check("fft: polynomial multiplication using fft", []() {
        const lli N = 1000*1000;
        auto vigen = rc::gen::container<vi>(rc::gen::inRange(-N, N));
        vi a = *vigen.as("a");
        vi b = *vigen.as("b");
        FFT fft(8*sizeof(size_t) - __builtin_clzll(a.size() + b.size() + 1));

        //multiplication using fft method
        fft.pad(a);
        fft.pad(b);
        vi c = fft.mult(a, b);
        FOR(n, c.size()) {
            lli cur = 0;
            for(lli k = 0; k <= n; ++k) {
                if(k < a.size() && n-k < b.size()) {
                    cur += a[k] * b[n-k];
                }
            }
            RC_ASSERT(cur == c[n]);
        }
    });
}
