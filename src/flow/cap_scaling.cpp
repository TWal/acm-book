struct CapScaling : public Graph {
    vb seen;
    CapScaling(lli n, lli s, lli t) : Graph(n, s, t) {}

    lli dfs(lli v, lli f) {
        if(v == t) return true;
        if(seen[v]) return false;
        seen[v] = true;
        for(lli k : adj[v]) {
            Edge ed = e[k];
            if(ed.c - ed.f >= f && dfs(ed.to, f)) {
                e[k].f += f;
                e[k^1].f -= f;
                return true;
            }
        }
        return false;
    }

    lli flow() {
        lli df = 0;
        lli f = 0;
        for(Edge ed : e) f = max(f, ed.c);
        while(f > 0){
            seen.assign(adj.size(), false);
            if(dfs(s, f)) df += f;
            else f /= 2;
        }
        return df;
    }
};
