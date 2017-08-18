struct Edge {
    Edge(lli to_, lli c_, lli cost_) : to(to_), c(c_), f(0), cost(cost_) {}
    lli to;
    lli c; //capacity
    lli f; //flow
    lli cost;
};

struct Graph {
    vector<Edge> e;
    vvi adj;
    lli s, t;
    Graph(lli n, lli s_, lli t_) : e(), adj(n), s(s_), t(t_) {}
    lli addEdge(lli a, lli b, lli c, lli cost = 0) {
        lli res = e.size();
        e.pb(Edge(b, c, cost)); e.pb(Edge(a, 0, -cost));
        adj[a].pb(res); adj[b].pb(res+1);
        return res;
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
