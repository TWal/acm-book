#include <precontest/template.cpp>
#include <math/egcd.cpp>
#include <math/Fp.cpp>
#include <math/fft.cpp>
#include <math/gauss.cpp>
#include <math/gauss_z2z.cpp>
#include <math/walsh_hadamard.cpp>
#include <math/subset_sum.cpp>
#include <math/opmod.cpp>
#include <math/pollard_rho.cpp>
#include <math/miller_rabin.cpp>
#include <math/invmod.cpp>
#include <math/invmod_range.cpp>
#include <math/factorize_range.cpp>
#include <math/gen_primes.cpp>
#include <math/polynom_euclid.cpp>
#include <math/linear_reccurence.cpp>
#include <rapidcheck.h>

const lli PR = 5;

template<typename T>
static rc::Gen<vector<vector<T>>> getMatrix(int size) {
    return rc::gen::container<vector<vector<T>>>(size, rc::gen::container<vector<T>>(size, rc::gen::arbitrary<T>()));
}

template<typename T>
static rc::Gen<vector<T>> getVector(int size) {
    return rc::gen::container<vector<T>>(size, rc::gen::arbitrary<T>());
}

template<typename T>
static rc::Gen<T> getPrime() {
    return rc::gen::map(rc::gen::arbitrary<T>(), [](T n) {
        if(n < 0) n = -n;
        while(!isPrime(n)) ++n;
        return n;
    });
}

template<>
bool isZero<Fp<PR>>(Fp<PR> x) {
    return x.n == 0;
}

template<lli P>
ostream& operator<<(ostream& os, Fp<P> f) {
    os << f.n;
    return os;
}


namespace rc {

template<lli P>
struct Arbitrary<Fp<P>> {
    static Gen<Fp<P>> arbitrary() {
        return gen::build<Fp<P>>(
            gen::set(&Fp<P>::n, rc::gen::inRange<lli>(0, P))
        );
    }
};

} // namespace rc


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

    rc::check("fft: inverse fft (complex)", []() {
        auto doublegen = rc::gen::suchThat<double>([](double d) {
            return fabs(d) < 1e6;
        });
        auto pairgen = rc::gen::pair(doublegen, doublegen);
        auto cpxgen = rc::gen::map(pairgen, [](pair<double, double> p) {
                return cpx(p.first, p.second);
        });
        auto vigen = rc::gen::container<vector<cpx>>(cpxgen);
        vector<cpx> a = *vigen.as("a");
        FFT<cpx> fft(8*sizeof(size_t) - __builtin_clzll(a.size()));
        while(a.size() != fft.N) a.push_back(cpx(0, 0));
        vector<cpx> b = a;
        fft.fft(b.data(), false);
        fft.fft(b.data(), true);
        FOR(i, a.size()) {
            RC_ASSERT(abs(a[i]-b[i]) < 1e-5);
        }
    });

    rc::check("fft: polynomial multiplication using fft (complex)", []() {
        const lli N = 1000*1000;
        auto vigen = rc::gen::container<vi>(rc::gen::inRange(-N, N));
        vi a = *vigen.as("a");
        vi b = *vigen.as("b");
        FFT<cpx> fft(8*sizeof(size_t) - __builtin_clzll(a.size() + b.size() + 1));

        //multiplication using fft method
        fft.pad(a);
        fft.pad(b);
        vi c = multFFT(fft, a, b);
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

    rc::check("fft: inverse fft (number theoretic)", []() {
        auto vigen = rc::gen::container<vector<Fp<P>>>(rc::gen::map(rc::gen::inRange((lli)0, P), [](lli n) { return Fp<P>(n); }));
        vector<Fp<P>> a = *vigen.as("a");
        FFT<Fp<P>> fft(10);
        RC_PRE(a.size() <= fft.N);
        fft.pad(a);
        vector<Fp<P>> b = a;
        fft.fft(b.data(), false);
        fft.fft(b.data(), true);
        FOR(i, a.size()) {
            RC_ASSERT(a[i].n == b[i].n);
        }
    });

    rc::check("fft: polynomial multiplication using fft (number theoretic)", []() {
        auto vigen = rc::gen::container<vi>(rc::gen::inRange(0, 100));
        vi a = *vigen.as("a");
        vi b = *vigen.as("b");
        FFT<Fp<P>> fft(10);

        vector<Fp<P>> ap(a.size());
        vector<Fp<P>> bp(b.size());
        FOR(i, a.size()) {
            ap[i] = Fp<P>(a[i]);
        }
        FOR(i, b.size()) {
            bp[i] = Fp<P>(b[i]);
        }
        fft.pad(ap);
        fft.pad(bp);
        fft.fft(ap);
        fft.fft(bp);
        vector<Fp<P>> cp(fft.N);
        FOR(i, fft.N) {
            cp[i] = ap[i]*bp[i];
        }
        fft.fft(cp, true);

        FOR(n, cp.size()) {
            lli cur = 0;
            for(lli k = 0; k <= n; ++k) {
                if(k < a.size() && n-k < b.size()) {
                    cur += a[k] * b[n-k];
                }
            }
            RC_ASSERT(Fp<P>(cur).n == cp[n].n);
        }
    });

    rc::check("gauss_mod", []() {
        using vf = vector<Fp<PR>>;
        using vvf = vector<vector<Fp<PR>>>;
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        vvf matrix = *getMatrix<Fp<PR>>(size).as("matrix");
        vf vec = *getVector<Fp<PR>>(size).as("vector");
        vf coefs = *getVector<Fp<PR>>(size).as("coefs");

        Gauss<Fp<PR>> gauss(size);
        FOR(i, size) {
            if(!gauss.add(matrix[i])) {
                vf out;
                RC_ASSERT(gauss.solve(matrix[i], out));
                vf tmp(size, 0);
                FOR(j, gauss.V.size()) {
                    FOR(k, size) tmp[k] = tmp[k] + out[j] * matrix[gauss.V[j]][k];
                }

                FOR(j, size) {
                    RC_ASSERT(matrix[i][j].n == tmp[j].n);
                }
            }
        }

        //Check that a linear combination of the original vectors is solvable
        {
            vf comblin(size, Fp<PR>(0));
            FOR(i, size) {
                FOR(j, size) comblin[i] = comblin[i] + coefs[j] * matrix[j][i];
            }

            vf out;
            RC_ASSERT(gauss.solve(comblin, out));
            vf vec2(size, 0);
            FOR(i, size) {
                FOR(j, gauss.V.size()) {
                    vec2[i] = vec2[i] + out[j] * matrix[gauss.V[j]][i];
                }
            }

            FOR(i, size) {
                RC_ASSERT(comblin[i].n == vec2[i].n);
            }
        }

        //Try to solve a random vector
        {
            vf out;
            if(!gauss.solve(vec, out)) return;

            vf vec2(size, 0);

            FOR(i, size) {
                FOR(j, gauss.V.size()) {
                    vec2[i] = vec2[i] + out[j] * matrix[gauss.V[j]][i];
                }
            }

            FOR(i, size) {
                RC_ASSERT(vec[i].n == vec2[i].n);
            }
        }
    });

    rc::check("gauss_z2z", []() {
        const lli N = 200;
        vvb matrix = *getMatrix<bool>(N).as("matrix");
        vb vec = *getVector<bool>(N).as("vector");
        vb comblin = *getVector<bool>(N).as("comblin");

        vector<bitset<N>> bmatrix;
        bmatrix.resize(N);
        FOR(i, N) {
            FOR(j, N) {
                if(matrix[i][j]) bmatrix[i].set(j);
            }
        }
        bitset<N> bvec;
        FOR(i, N) {
            if(vec[i]) bvec.set(i);
        }

        GaussZ2Z<N> gauss;
        FOR(i, N) {
            if(!gauss.add(bmatrix[i])) {
                bitset<N> out;
                RC_ASSERT(gauss.solve(bmatrix[i], out));
                bitset<N> tmp;
                FOR(j, gauss.V.size()) {
                    if(out[j]) tmp ^= bmatrix[gauss.V[j]];
                }

                RC_ASSERT(bmatrix[i] == tmp);
            }
        }

        //Check that a linear combination of the original vectors is solvable
        {
            bitset<N> veccl, out, vec2;
            FOR(i, N) if(comblin[i]) veccl ^= bmatrix[i];
            RC_ASSERT(gauss.solve(veccl, out));
            FOR(i, gauss.V.size()) {
                if(out[i]) vec2 ^= bmatrix[gauss.V[i]];
            }

            RC_ASSERT(veccl == vec2);
        }

        //Try to solve a random vector
        {
            bitset<N> out;
            if(!gauss.solve(bvec, out)) return;

            bitset<N> vec2;
            FOR(i, gauss.V.size()) {
                if(out[i]) vec2 ^= bmatrix[gauss.V[i]];
            }

            RC_ASSERT(bvec == vec2);
        }
    });

    rc::check("wash_hadamard: inverse", []() {
        int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        while(size & (size-1)) ++size;
        lli log2Size = 0;
        while(size>>log2Size) ++log2Size;
        lli bound = (1ll<<(63-log2Size));
        vi v = *rc::gen::container<vi>((size_t)size, rc::gen::inRange<lli>(-bound, bound)).as("vector");
        vi vSave = v;
        hadamard(v, false);
        hadamard(v, true);
        RC_ASSERT(vSave == v);
    });

    rc::check("wash_hadamard: convolution", []() {
        int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        while(size & (size-1)) ++size;
        lli log2Size = 0;
        while(size>>log2Size) ++log2Size;
        lli bound = (1ll<<((63-3*log2Size)/2));
        auto vigen = rc::gen::container<vi>((size_t)size, rc::gen::inRange<lli>(-bound, bound));
        vi v1 = *vigen.as("v1");
        vi v2 = *vigen.as("v2");
        vi goodConv(size, 0);
        FOR(i, size) {
            FOR(j, size) {
                goodConv[i^j] += v1[i] * v2[j];
            }
        }
        hadamard(v1, false);
        hadamard(v2, false);
        FOR(i, size) {
            v1[i] *= v2[i];
        }
        hadamard(v1, true);
        RC_ASSERT(v1 == goodConv);
    });

    rc::check("wash_hadamard: bound", []() {
        int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        while(size & (size-1)) ++size;
        lli log2Size = 0;
        while(size>>log2Size) ++log2Size;
        lli bound = (1ll<<(63-log2Size));
        vi v = *rc::gen::container<vi>((size_t)size, rc::gen::inRange<lli>(-bound, bound)).as("vector");
        size_t realBound = 0;
        FOR(i, size) realBound = max(realBound, (size_t)abs(v[i]));
        hadamard(v, false);
        FOR(i, size) {
            RC_ASSERT((size_t)abs(v[i]) <= v.size()*realBound);
        }
    });

    rc::check("subset_sum : subset sum", []() {
        int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        while(size & (size-1)) ++size;
        vi v = *rc::gen::container<vi>((size_t)size, rc::gen::arbitrary<lli>()).as("vector");
        vi sum(size, 0);
        FOR(i, size) {
            lli j = i;
            //enumerate over subsets of i
            do {
                sum[i] += v[j];
                j = (j-1)&i;
            } while(j != i);
        }
        subsetSum(v, false);
        RC_ASSERT(sum == v);
    });

    rc::check("subset_sum: inverse", []() {
        int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        while(size & (size-1)) ++size;
        vi v = *rc::gen::container<vi>((size_t)size, rc::gen::arbitrary<lli>()).as("vector");
        vi vSave = v;
        subsetSum(v, false);
        subsetSum(v, true);
        RC_ASSERT(v == vSave);
    });

    rc::check("subset_sum: convolution", []() {
        int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        while(size & (size-1)) ++size;
        auto vigen = rc::gen::container<vi>((size_t)size, rc::gen::arbitrary<lli>()).as("vector");
        vi v1 = *vigen.as("v1");
        vi v2 = *vigen.as("v2");
        vi goodConv(size, 0);
        FOR(i, size) {
            FOR(j, size) {
                goodConv[i|j] += v1[i] * v2[j];
            }
        }
        subsetSum(v1, false);
        subsetSum(v2, false);
        FOR(i, size) {
            v1[i] *= v2[i];
        }
        subsetSum(v1, true);
        RC_ASSERT(v1 == goodConv);
    });

    rc::check("multmod: check correctness with 32 bit value", []() {
        llu a = *rc::gen::arbitrary<unsigned int>().as("a");
        llu b = *rc::gen::arbitrary<unsigned int>().as("b");
        llu m = *rc::gen::arbitrary<unsigned int>().as("m");
        RC_PRE(m > 0);
        RC_ASSERT((a*b)%m == multmod(a, b, m));
    });

    rc::check("multmod: uint128 version", []() {
        llu a = *rc::gen::arbitrary<llu>().as("a");
        llu b = *rc::gen::arbitrary<llu>().as("b");
        llu m = *rc::gen::arbitrary<llu>().as("m");
        RC_PRE(m > 0);
        RC_ASSERT(multmodInt128(a, b, m) == multmod(a, b, m));
    });

    rc::check("pollard_rho", []() {
        llu a = *rc::gen::arbitrary<unsigned int>().as("a");
        llu b = *rc::gen::arbitrary<unsigned int>().as("b");
        RC_PRE(a >= 2);
        RC_PRE(b >= 2);
        llu n = a*b;
        llu factor = pollardrho(n);
        RC_ASSERT(factor != 1 && factor != n && n%factor == 0);
    });

    rc::check("miller_rabin", []() {
        llu n = *rc::gen::arbitrary<unsigned int>().as("n");
        bool dumbIsPrime;
        if(n < 2) {
            dumbIsPrime = false;
        } else {
            dumbIsPrime = true;
            for(llu i = 2; i*i <= n; ++i) {
                if(n%i == 0) {
                    dumbIsPrime = false;
                    break;
                }
            }
        }
        RC_ASSERT(dumbIsPrime == isPrime(n));
    });

    rc::check("invmod", []() {
        lli a = *rc::gen::arbitrary<int>().as("a");
        lli m = *rc::gen::arbitrary<int>().as("m");
        RC_PRE(a > 0);
        RC_PRE(m > 0);
        lli g = __gcd(a, m);
        a /= g;
        m /= g;
        RC_PRE(m > 1);
        lli b = invmod(a, m);
        RC_ASSERT(((a*b)%m+m)%m == 1);
    });

    rc::check("invmodRange", []() {
        lli p = *getPrime<int>().as("p");
        lli size = min(10*1000ll, p-1);
        vi invs = invmodRange(size, p);
        FOR(i, size) {
            if(i != 0) {
                RC_ASSERT((i*invs[i])%p == 1);
            }
        }
    });

    rc::check("factorizeRange", []() {
        lli l = *rc::gen::arbitrary<int>().as("l");
        RC_PRE(l >= 2);
        lli r = l + 1000;
        vvi facts = factorizeRange(l, r);
        RC_ASSERT(facts.size() == r-l);
        FOR(i, facts.size()) {
            lli cur = 1;
            for(lli p : facts[i]) {
                RC_ASSERT(isPrime(p));
                cur *= p;
            }
            RC_ASSERT(cur == l+i);
        }
    });

    rc::check("gen_primes", []() {
        lli n = *rc::gen::inRange<int>(3, 10*1000).as("l");
        vi p = genPrimes(n);
        RC_ASSERT(p[0] == 2);
        FOR(i, p.size()) {
            RC_ASSERT(isPrime(p[i]));
        }
        FOR(i, p.size()-1) {
            FORU(j, p[i]+1, p[i+1]) {
                RC_ASSERT(!isPrime(j));
            }
        }
    });

    rc::check("polynom_euclid", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        const lli P = 1000*1000*1000+7;
        using F = Fp<P>;
        vector<F> a = *getVector<F>(size).as("a");
        lli bsize = *rc::gen::inRange<lli>(1, size+1);
        vector<F> b = *getVector<F>(bsize).as("b");
        RC_PRE(b.back().n != 0);
        pair<vector<F>, vector<F>> eucl = euclid(a, b);
        vector<F> q = eucl.first, r = eucl.second;
        RC_ASSERT((q.size()-1) + (b.size()-1) == (a.size()-1));
        RC_ASSERT(r.size() < b.size());
        vector<F> bq(b.size()+q.size()-1, 0);
        FOR(i, b.size()) FOR(j, q.size()) {
            bq[i+j] = bq[i+j] + b[i]*q[j];
        }
        vector<F> sub(a.size());
        FOR(i, a.size()) {
            sub[i] = a[i]-bq[i];
        }

        FOR(i, sub.size()) {
            if(i < r.size()) {
                RC_ASSERT(sub[i].n == r[i].n);
            } else {
                RC_ASSERT(sub[i].n == 0);
            }
        }
    });

    rc::check("linear_reccurence", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 2);
        const lli P = 1000*1000*1000+7;
        using F = Fp<P>;
        vector<F> rec = *getVector<F>(size).as("rec");
        vector<F> vals = *getVector<F>(size).as("vals");
        lli n = *rc::gen::inRange<lli>(0, 10*1000).as("n");
        vector<F> polyRec(rec.size()+1);
        FOR(i, rec.size()) {
            polyRec[i] = F()-rec[i];
        }
        polyRec.back() = 1;
        F fastVal = linearRec(n, polyRec, vals);

        while(n >= vals.size()) {
            F next = F();
            FOR(i, vals.size()) {
                next = next+vals[i]*rec[i];
            }
            FOR(i, vals.size()-1) {
                vals[i] = vals[i+1];
            }
            vals.back() = next;
            n -= 1;
        }

        F slowVal = vals[n];

        RC_ASSERT(fastVal.n == slowVal.n);
    });
}
