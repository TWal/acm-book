#include <precontest/template.cpp>
#include <math/egcd.cpp>
#include <math/fft.cpp>
#include <rapidcheck.h>

void testMath() {
    rc::check("gcd(a, b) divides a and b", [](lli a, lli b) {
        lli g = X(egcd(a, b));
        if(a != 0) RC_ASSERT(a%g == 0);
        if(b != 0) RC_ASSERT(b%g == 0);
    });

    rc::check("(g,u,v)=gcd(a,b) => a*u+b*v=g", [](lli a, lli b) {
        lli g, u, v;
        tie(g, u, v) = egcd(a, b);
        RC_ASSERT(a*u+b*v == g);
    });

    rc::check("polynomial multiplication using fft", []() {
        const lli N = 1000*1000;
        auto vigen = rc::gen::container<vi>(rc::gen::inRange(-N, N));
        vi a = *vigen.as("a");
        vi b = *vigen.as("c");
        vi c = polymult(a, b);
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