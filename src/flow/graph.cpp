struct Edge {
    lli to, f, c, cost; //f = flow, c = capacity
};

struct Graph {
    vector<Edge> e;
    vvi adj;
    lli s, t;
    Graph(lli n, lli s_, lli t_) : e(), adj(n), s(s_), t(t_) {}
    lli addEdge(lli a, lli b, lli c1, lli c2 = 0, lli cost = 0) {
        adj[a].pb(e.size()); e.pb(Edge{b, 0, c1, cost});
        adj[b].pb(e.size()); e.pb(Edge{a, 0, c2, -cost});
        return adj[a].back();
    }
    vb minCut() {
        struct Dfs { //I use a struct because std::function has a huge constant
            Graph& g; vb seen;
            Dfs(Graph& g_) : g(g_), seen(g.adj.size(), false) {}
            void dfs(lli v) {
                if(seen[v]) return;
                seen[v] = true;
                for(lli i : g.adj[v]) if(g.e[i].c > g.e[i].f) dfs(g.e[i].to);
            }
        };
        Dfs d(*this);
        d.dfs(s);
        return d.seen;
    }
};
