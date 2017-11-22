using ld = long double;
// Static convex hull trick
// Solve recurrences of the form dp[i] = min(dp[j] + b[j] * a[i], j < i)
// Hypotheses:  a[i] <= a[i+1] (increasing queries)
//              b[i] >= b[i+1] (decreasing slopes)
struct StaticCHT {
    vi a, b;
    int hd, tl;

    // n: upper bound on the number of lines inserted
    StaticCHT(int n) : a(n), b(n) { hd = tl = 0; }

    // Add y  = a0*x + b0
    void add(lli a0, lli b0) {
        while (tl - hd >= 2) {
            lli a1 = a[tl - 1], b1 = b[tl - 1];
            lli a2 = a[tl - 2], b2 = b[tl - 2];
            // Beware of overflows: consider long double
            if (ld(b0 - b2) * (a1 - a0) < ld(b0 - b1) * (a2 - a0)) break;
            //if ((long double)1 * (b0 - b2) / (a2 - a0) < 
            //    (long double)1 * (b0 - b1) / (a1 - a0))
            //  break;
            tl--;
        }
        a[tl] = a0; b[tl] = b0;
        tl++;
    }

    // Lower hull value on line x = x0
    lli query(lli x0) {
        while (tl - hd >= 2) {
            if (x0 * a[hd] + b[hd] < x0 * a[hd + 1] + b[hd + 1]) break;
            hd++;
        }
        return x0 * a[hd] + b[hd];
    }
};

// Fully dynamic variant (from https://github.com/niklasb/contest-algos/)
// Should not appear in b[i]
const lli QUERY = LLONG_MIN;
struct Line {
    lli a, b;
    mutable function<const Line *()> succ;

    bool operator < (const Line& o) const {
        if (o.b != QUERY) return a > o.a;
        const Line *s = succ();
        if (s == NULL) return false;
        return b - s->b > (s->a - a) * o.a;
    }
};

struct DynamicCHT : public multiset<Line> {
    bool bad(iterator y) {
        auto z = next(y);
        if (y == begin()) {
            if (z == end()) return false;
            return y->a == z->a && y->b >= z->b;
        }
        auto x = prev(y);
        if (z == end()) return y->a == x->a && y->b >= x->b;
        return ld(x->b - y->b) * (z->a - y->a) >= ld(y->b - z->b) * (y->a - x->a);
    }

    void add(lli a, lli b) {
        auto y = insert({a, b, nullptr});
        y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
        if (bad(y)) { erase(y); return; }
        while (next(y) != end() && bad(next(y))) erase(next(y));
        while (y != begin() && bad(prev(y))) erase(prev(y));
    }

    lli query(lli x) {
        auto l = *lower_bound((Line){x, QUERY, nullptr});
        return l.a * x + l.b;
    }
};

// Example of use
int main() {
    int n; scanf("%d", &n);
    vi a(n), b(n);
    FOR(i, n) scanf("%lld", &a[i]);
    FOR(i, n) scanf("%lld", &b[i]);
    DynamicCHT cht;
    cht.add(b[0], 0);
    lli ans;
    FORU(i, 1, n) {
        ans = cht.query(a[i]);
        cht.add(b[i], ans);
    }
    printf("%lld\n", ans);
}
