struct Dinic : public Graph {
    vi d, w;
    Dinic(lli n, lli s, lli t) : Graph(n, s, t) {}
    lli dfs(lli v, lli f) {
        if(v == t) return f;
        lli cf = 0;
        for(; w[v] < (lli)adj[v].size(); w[v]++) {
            lli k = adj[v][w[v]];
            Edge& ed = e[k];
            if(ed.c > ed.f && d[ed.to] == d[v] + 1){
                lli df = dfs(ed.to, min(f, ed.c - ed.f));
                if(df > 0){
                    cf += df;
                    f -= df;
                    ed.f += df;
                    e[k^1].f -= df;
                }
            }
        }
        return cf;
    }

    lli flow() {
        lli N = adj.size();
        lli f = 0;
        while(true) {
            d.assign(N, -1);
            queue<lli> q; q.push(s); d[s] = 0;
            while(!q.empty()){
                lli v = q.front(); q.pop();
                if(v == t) break;
                for(lli k : adj[v]) {
                    Edge ed = e[k];
                    if(ed.c > ed.f && d[ed.to] == -1) {
                        d[ed.to] = d[v] + 1;
                        q.push(ed.to);
                    }
                }
            }
            if(d[t] == -1) return f;
            w.assign(N, 0);
            f += dfs(s, (1ll<<62));
            assert(dfs(s, (1ll<<62)) == 0); //we can remove this
        }
        return f;
    }
};
