struct WaveletTree {
    // If you don't want activation, assume that active[...][x] == x
    // t[u][i] = Card{0 <= k < i, v[k] in left part}
    // active[u].get(i) = Card{0 <= k < i, v[k] active}
    vi a; vvi t; vector<BIT> active;
    int n;

    void build(auto b, auto e, int L, int R, int u) {
        int s = e-b+1;
        active[u] = BIT(s);
        FORU(i, 1, s) active[u].add(i, 1);
        if(L == R) return;
        int M = (L + R) / 2;
        t[u].reserve(s); t[u].pb(0);
        auto f = [&](int i) { return i <= M; };
        for(auto it = b; it != e; ++it) t[u].pb(t[u].back() + f(*it));
        auto mv = stable_partition(b, e, f);
        build(b, mv, L, M, 2 * u); build(mv, e, M + 1, R, 2 * u + 1);
    }

    // a: input array
    // n: size of the alphabet (i.e. Sigma = {0, ..., n-1})
    WaveletTree(int n, const vi& a) : a(a), t(2*n), active(4*n), n(n) {
        vi v = a;
        build(v.begin(), v.end(), 0, n-1, 1);
    }

    pii _rank(int v, int r, bool calcSum = true) const {
        int L = 0, R = n - 1, u = 1, sum = 0;
        while(L != R) {
            int M = (L + R) / 2;
            if(v <= M) {
                r = t[u][r];
                R = M;
                u = 2 * u;
            } else {
                if(calcSum) sum += active[2*u].get(t[u][r]);
                r -= t[u][r];
                L = M + 1;
                u = 2 * u + 1;
            }
        }
        return mt(sum, active[u].get(r));
    }

    // Swap the elements at positions x and x+1 /!\ do not swap activations
    void wswap(int x) {
        int xsave = x;
        bool nact[2];
        FOR(i, 2) nact[i] = !isActivated(x+i);
        FOR(i, 2) if(nact[i]) activate(x+i, true, false);
        int L = 0, R = n - 1, u = 1;
        int v1 = a[x], v2 = a[x + 1];
        swap(a[x], a[x + 1]);
        while(L != R) {
            int M = (L + R) / 2;
            if((v1 <= M) != (v2 <= M)) {
                t[u][x + 1] += v1 <= M ? -1 : 1;
                break;
            } else if(v1 <= M) {
                x = t[u][x];
                R = M;
                u = 2 * u;
            } else {
                x -= t[u][x];
                L = M + 1;
                u = 2 * u + 1;
            }
        }
        FOR(i, 2) if(nact[i]) activate(xsave+i, false, false);
    }

    // Toggle the state of the element of value v at index x
    void activate(int x, bool activation, bool check = true) {
        if(check && isActivated(x) == activation) return;
        int L = 0, R = n - 1, u = 1;
        int v = a[x];
        while(L != R) {
            int M = (L + R) / 2;
            active[u].add(x + 1, activation ? 1 : -1);
            if(v <= M) {
                x = t[u][x];
                R = M;
                u = 2 * u;
            } else {
                x -= t[u][x];
                L = M + 1;
                u = 2 * u + 1;
            }
        }
        active[u].add(x + 1, activation ? 1 : -1);
    }

    bool isActivated(int x) { return active[1].get(x+1) == 1+active[1].get(x); }

    // Card{l <= k < r | a[k] == v}
    int rank(int v, int l, int r) const {
        return Y(_rank(v, r, false)) - Y(_rank(v, l, false));
    }

    // Card{l <= k < r | x <= a[k] < y}
    int rankrange(int x, int y, int l, int r) const {
        return X(_rank(y, r)) - X(_rank(x, r)) - X(_rank(y, l)) + X(_rank(x, l));
    }

    // k-th smallest element in [l, ..., r-1]. Returns -1 if it doesn't exists
    // /!\ k starts at 1, meaning that min(a[l:r]) == kth(1, l, r)
    int kth(int k, int l, int r) const {
        if(active[1].get(r) - active[1].get(l) < k) return -1;
        int L = 0, R = n - 1, u = 1;
        while(L != R) {
            int M = (L + R) / 2;
            int c = active[2*u].get(t[u][r]) - active[2*u].get(t[u][l]);
            if(k <= c) {
                l = t[u][l]; r = t[u][r];
                R = M;
                u = 2 * u;
            } else {
                k -= c; l -= t[u][l]; r -= t[u][r];
                L = M + 1;
                u = 2 * u + 1;
            }
        }
        return L;
    }
};
