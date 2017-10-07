//LCA where you can add new leaf dynamically /!\ the tree must remain connected!
struct LCADyn {
    lli n, logn;
    vvi prev;
    vi depth;

    LCADyn(lli n, lli root) : n(n), logn(log2(n)), prev(n,vi(logn+1)), depth(n,-1) {
        depth[root] = 0;
        FOR(i, logn+1) prev[root][i] = root;
    }

    void add(lli node, lli parent) {
        assert(depth[parent] >= 0);
        depth[node] = depth[parent]+1;
        prev[node][0] = parent;
        FOR(i, logn) prev[node][i+1] = prev[prev[node][i]][i];
    }

    lli query(lli a, lli b) {
        assert(depth[a] >= 0 && depth[b] >= 0);
        if(depth[a] < depth[b]) swap(a, b);
        lli i = logn;
        while(depth[a] > depth[b]) {
            if(depth[prev[a][i]] >= depth[b]) {
                a = prev[a][i];
            }
            --i;
        }

        if(a == b) return a;
        i = logn;
        while(i >= 0) {
            if(prev[a][i] != prev[b][i]) {
                a = prev[a][i], b = prev[b][i];
            }
            --i;
        }

        return prev[a][0];
    }
};
