const lli INF = 1e18;

struct MCMF_DFS {
    vi last, curr, seen; vi pi;
    vi ne, adj; vi flow, cap, cost;
    int N, M;

    MCMF_DFS(int N) : N(N), M(0) {
        last = vi(N, -1);
        curr = seen = pi = vi(N);
    }

    // u -> v (c, w). c = capacity, w = cost (weight)
    int add(int u, int v, lli c, lli w) {
        adj.pb(v); flow.pb(0); cap.pb(c);
        cost.pb(+w); ne.pb(last[u]); last[u] = M++;
        adj.pb(u); flow.pb(0); cap.pb(0);
        cost.pb(-w); ne.pb(last[v]); last[v] = M++;
        return M - 2;
    }

    lli push(int u, int t, lli f) {
        if (u == t) return f;
        if (seen[u]) return 0;
        seen[u] = true;
        for (int e = curr[u]; e != -1; e = ne[e]) {
            int v = adj[e];
            if (flow[e] < cap[e] && pi[u] == pi[v] + cost[e])
                if (lli df = push(v, t, min(f, cap[e] - flow[e]))) {
                    flow[e] += df;
                    flow[e ^ 1] -= df;
                    return df;
                }
        }
        return 0;
    }

    pii mcmf(int s, int t) {
        lli answ = 0, ansf = 0;
        pi.assign(N, INF);
        pi[t] = 0;
        bool done;
        do {
            done = true;
            FOR(u, N) for (int e = last[u]; e != -1; e = ne[e])
                if (flow[e] < cap[e] && pi[adj[e]] + cost[e] < pi[u]) {
                    pi[u] = pi[adj[e]] + cost[e];
                    done = false;
                }
        } while (!done);
        for (;;) {
            seen.assign(N, false);
            copy(ALL(last), curr.begin());
            while (lli df = push(s, t, INF)) {
                answ += pi[s] * df;
                ansf += df;
                seen.assign(N, false);
            }
            lli inc = INF;
            FOR(u, N) if (seen[u]) for (int e = last[u]; e != -1; e = ne[e])  {
                int v = adj[e];
                if (flow[e] < cap[e] && !seen[v]) {
                    inc = min(inc, pi[v] + cost[e] - pi[u]);
                }
            }
            if (inc == INF) break;
            FOR(u, N) if (seen[u]) pi[u] += inc;
        }
        return mt(answ, ansf);
    }
};

struct MCMF_Dijkstra {
    vi last, how; vi dist, pi;
    vi ne, from, adj; vi flow, cap, cost;
    int N, M;

    MCMF_Dijkstra(int N) : N(N), M(0) {
        last = vi(N, -1);
        how = dist = pi = vi(N);
    }

    // u -> v (c, w). c = capacity, w = cost (weight)
    int add(int u, int v, lli c, lli w) {
        from.pb(u); adj.pb(v); flow.pb(0); cap.pb(c);
        cost.pb(+w); ne.pb(last[u]); last[u] = M++;
        from.pb(v); adj.pb(u); flow.pb(0); cap.pb(0);
        cost.pb(-w); ne.pb(last[v]); last[v] = M++;
        return M - 2;
    }

    pii mcmf(int s, int t) {
        lli answ = 0, ansf = 0;
        pi.assign(N, INF);
        pi[s] = 0;
        bool done;
        do {
            done = true;
            FOR(u, N) for (int e = last[u]; e != -1; e = ne[e])
                if (flow[e] < cap[e] && pi[u] + cost[e] < pi[adj[e]]) {
                    pi[adj[e]] = pi[u] + cost[e];
                    done = false;
                }
        } while (!done);
        while (true) {
            dist.assign(N, INF);
            set<pii> S;
            S.insert(mt(0, s));
            dist[s] = 0;
            while (!S.empty()) {
                int u = Y(*S.begin()); S.erase(S.begin());
                if (u == t) break;
                for (int e = last[u]; e != -1; e = ne[e]) {
                    if (flow[e] == cap[e]) continue;
                    int v = adj[e];
                    lli val = dist[u] + pi[u] + cost[e] - pi[v];
                    if (val < dist[v]) {
                        S.erase(mt(dist[v], v));
                        dist[v] = val;
                        how[v] = e;
                        S.insert(mt(dist[v], v));
                    }
                }
            }
            if (dist[t] == INF) break;
            lli inc = cap[how[t]] - flow[how[t]];
            for (int u = t; u != s; u = from[how[u]])
                inc = min(inc, cap[how[u]] - flow[how[u]]);
            for (int u = t; u != s; u = from[how[u]]) {
                flow[how[u]] += inc;
                flow[how[u] ^ 1] -= inc;
                answ += cost[how[u]] * inc;
            }
            ansf += inc;
            FOR(u, N) pi[u] = min(pi[u] + dist[u], INF);
        }

        return mt(answ, ansf);
    }
};
