struct Articulation {
    vvi adj; //undirected adjacency list
    lli N, T = 0;
    vi ind;
    vb ap; //articulation points

    Articulation(vvi& _adj) : adj(_adj), N(adj.size()), ind(N,-1), ap(N,0) {
        FOR(i, N) if(ind[i] < 0) dfs(i, -1);
    }

    lli dfs(lli v, lli parent) {
        lli nbChildren = 0, lown, lowv = ind[v] = T++;
        for(lli n : adj[v]) {
            if(ind[n] < 0) {
                nbChildren += 1;
                lowv = min(lowv, lown = dfs(n, v));
                if(lown >= ind[v]) ap[v] = true;
            } else if(n != parent) {
                lowv = min(lowv, ind[n]);
            }
        }
        if(parent < 0) ap[v] = (nbChildren > 1);
        return lowv;
    }
};
