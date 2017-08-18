struct SCCKosaraju {
    lli N, nscc = 0;
    vvi adj, tadj;
    vi scc, order;
    vb seen;

    SCCKosaraju(vvi& _adj) : N(_adj.size()), adj(_adj), tadj(N), scc(N) {
        FOR(i, N) {
            for(lli j : adj[i]) {
                tadj[j].pb(i);
            }
        }
        seen.assign(N, false);
        FOR(i, N) if(!seen[i]) {
            dfs(i, true);
        }
        seen.assign(N, false);
        FORD(i, 0, N) if(!seen[order[i]]) {
            dfs(order[i], false);
            nscc += 1;
        }
    }

    void dfs(lli v, bool fstpass) { //vertex, adjlist, is first pass
        seen[v] = true;
        for(lli n : (fstpass ? adj : tadj)[v]) if(!seen[n]) {
            dfs(n, fstpass);
        }
        if(fstpass) order.pb(v);
        else scc[v] = nscc;
    }
};
