struct SCCTarjan {
    vvi adj;
    lli N, T = 0, nscc = 0;
    vi scc, ind, low;
    stack<lli> st;
    vb os; //onstack

    SCCTarjan(vvi& _adj) : adj(_adj),N(adj.size()),scc(N),ind(N,-1),low(N),os(N,0) {
        FOR(i, N) if(ind[i] < 0) dfs(i);
    }

    void dfs(lli v) {
        ind[v] = low[v] = T++;
        st.push(v); os[v] = true;
        for(lli n : adj[v]) {
            if(ind[n] < 0) {
                dfs(n);
                low[v] = min(low[v], low[n]);
            } else if(os[n]) {
                low[v] = min(low[v], ind[n]);
            }
        }
        if(low[v] == ind[v]) {
            while(true) {
                lli w = st.top(); st.pop(); os[w] = false;
                scc[w] = nscc;
                if(v == w) break;
            }
            nscc += 1;
        }
    }
};
