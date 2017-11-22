// Returns the total cost of the directed MST of the directed graph G = (V, E)
// from the root node r (0 <= r < n), with n = |V|.
// Complexity: O(nm)
struct Edge { int u, v; lli w; };

lli dmst(int n, vector<Edge> E, int r) {
    const lli INF = 1ll << 52;
    vi cost, label, bio;
    vi back(n);
    lli ans = 0;
    while (true) {
        cost.assign(n, INF);
        for (auto e: E) if (e.u != e.v && e.w < cost[e.v]) {
            cost[e.v] = e.w;
            back[e.v] = e.u;
        }
        cost[r] = 0;
        FOR(u, n) if (cost[u] == INF) return -1;
        FOR(u, n) ans += cost[u];
        label.assign(n, -1); bio.assign(n, -1);
        int np = 0;
        FOR(u, n) {
            int v;
            for (v = u; v != r && bio[v] == -1; v = back[v]) bio[v] = u;
            if (v != r && bio[v] == u) {
                for (; label[v] == -1; v = back[v]) label[v] = np;
                np++;
            }
        }
        if (np == 0) break;
        for (auto& l: label) if (l == -1) l = np++;
        for (auto& e: E) { // don't forget & 
            int pu = label[e.u], pv = label[e.v];
            if (pu != pv) e.w -= cost[e.v];
            e.u = pu; e.v = pv;
        }
        r = label[r];
        n = np;
    }
    return ans;
}

// Example of use
int main() {
    int T; scanf("%d", &T);
    FORU(t, 1, T + 1) {
        int n, m; scanf("%d%d", &n, &m);
        vector<Edge> E(m);
        FOR(i, m) scanf("%d%d%lld", &E[i].u, &E[i].v, &E[i].w);
        lli ans = dmst(n, E, 0);
        printf("Case #%lld: ", t);
        if (ans == -1) puts("Possums!");
        else printf("%lld\n", ans);
    } 
}
