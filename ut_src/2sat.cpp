// 2-SAT
// O(n + m)
struct SAT2 {
    vb vals;
    bool issat;

    // Formula:
    // (X(v[0]) or Y(v[0])) and
    // (X(v[1]) or Y(v[1])) and
    // ...
    // (X(v[p]) or Y(v[p]))
    // litteral ::= 2*x for x
    //              2*x+1 for not(x)
    // 0 <= minterms[0].fst/snd < 2*m
    SAT2(vector<pii> v, int m) {
	vvi adj(2 * m);
        for (auto it: v) {
            adj[X(it) ^ 1].pb(Y(it));
            adj[Y(it) ^ 1].pb(X(it));
        }
	SCC scc(adj);
        issat = true;
        for (int u = 0; u < 2 * m; u += 2) issat &= (scc.scc[u] != scc.scc[u + 1]);
        if (issat) {
            vals = vb(m);
            FOR(u, m) vals[u] = scc.scc[2 * u] > scc.scc[2 * u + 1];
        }
    }
};

// Example of use
int main() {
    int n, m; scanf("%d%d", &n, &m);
    vector<pii> a;
    FOR(i, m) {
        int u, v, c; scanf("%d%d%d", &u, &v, &c); u--; v--;
        if (c) {
            a.pb(mt(2 * u + 1, 2 * v));
            a.pb(mt(2 * v + 1, 2 * u));
        } else {
            a.pb(mt(2 * u, 2 * v));
            a.pb(mt(2 * u + 1, 2 * v + 1));
        }
    }
    SAT2 sat(a, n);
    if (sat.issat) {
        vi ans;
        FOR(i, n) if (sat.vals[i]) ans.pb(i);
        printf("%lld\n", SZ(ans));
        for (int x: ans) printf("%d ", 1 + x); puts("");
    } else
        puts("Impossible");
}
