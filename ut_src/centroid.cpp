// Example of centroid decomposition to solve "number of paths of length K"
// in a tree
const int MAXK = 500;
const int MAXN = 5e5;
vi adj[MAXN];
bool center[MAXN];
int cnt[MAXK + 1];
int sub[MAXN];
int n, k;
lli ans;

void mark(int u, int delta, int d = 1, int p = -1) {
    if (center[u] || d > k) return;
    cnt[d] += delta;
    for (int v: adj[u]) if (v != p) mark(v, delta, d + 1, u);
}

void count(int u, int d = 1, int p = -1) {
    if (center[u] || d > k) return;
    ans += cnt[k - d];
    for (int v: adj[u]) if (v != p) count(v, d + 1, u);
}

void work(int u) {
    cnt[0] = 1;
    for (int v: adj[u]) mark(v, +1, 1); 
    ans += cnt[k];
    for (int v: adj[u]) {
        mark(v, -1, 1);
        count(v, 1);
        mark(v, +1, 1);
    }
    for (int v: adj[u]) mark(v, -1, 1);
}

void compsz(int u, int p = -1) {
    sub[u] = 1;
    for (int v: adj[u]) if (v != p && !center[v]) {
        compsz(v, u);
        sub[u] += sub[v];
    }
}

void build(int u) {
    compsz(u);
    int sz = sub[u], p = -1;
    while (true) {
        int maxv = -1;
        for (int v: adj[u]) if (v != p && !center[v] && (maxv == -1 || sub[v] > sub[maxv])) maxv = v;
        if (maxv == -1 || sub[maxv] <= sz / 2) break;
        p = u;
        u = maxv;
    }
    center[u] = true;
    work(u);
    for (int v: adj[u]) if (!center[v]) build(v);
}

int main() {
    scanf("%d%d", &n, &k);
    FOR(i, n - 1) {
        int u, v; scanf("%d%d", &u, &v); u--; v--;
        adj[u].pb(v); adj[v].pb(u);
    }
    build(0);
    assert(ans % 2 == 0);
    printf("%lld\n", ans / 2);
}
