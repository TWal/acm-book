struct Bridges {
    vvi adj; //undirected adjacency list
    lli N, T = 0;
    vi ind;
    vii bridges;

    Bridges(vvi& _adj) : adj(_adj), N(adj.size()), ind(N,-1) {
        FOR(i, N) if(ind[i] < 0) dfs(i, -1);
    }

    lli dfs(lli v, lli parent) {
        lli lown, lowv = ind[v] = T++;
        for(lli n : adj[v]) {
            if(ind[n] < 0) {
                lowv = min(lowv, lown = dfs(n, v));
                if(lown > ind[v]) bridges.pb(mt(v, n));
            } else if(n != parent) {
                lowv = min(lowv, ind[n]);
            }
        }
        return lowv;
    }
};
