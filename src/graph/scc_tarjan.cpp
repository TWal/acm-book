struct SCCTarjan {
    vvi adj;
    lli N, T = 0, nscc = 0;
    vi scc, ind;
    stack<lli> st;
    vb os; //onstack

    SCCTarjan(vvi& _adj) : adj(_adj), N(adj.size()), scc(N), ind(N,-1), os(N,0) {
        FOR(i, N) if(ind[i] < 0) dfs(i);
    }

    lli dfs(lli v) {
        lli lowv = ind[v] = T++;
        st.push(v); os[v] = true;
        for(lli n : adj[v]) {
            if(ind[n] < 0) lowv = min(lowv, dfs(n));
            else if(os[n]) lowv = min(lowv, ind[n]);
        }
        if(lowv == ind[v]) {
            while(true) {
                lli w = st.top(); st.pop(); os[w] = false;
                scc[w] = nscc;
                if(v == w) break;
            }
            nscc += 1;
        }
        return lowv;
    }
};
