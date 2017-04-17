#include <precontest/template.cpp>
#include <flow/graph.cpp>
#include <flow/dinic.cpp>
#include <flow/cap_scaling.cpp>
#include <rapidcheck.h>

template<typename T>
static rc::Gen<T> getGraph() {
    return rc::gen::withSize([](int n) {
        n += 2;
        return rc::gen::map(rc::gen::container<vector<int>>(n*(n-1), rc::gen::inRange(-10000, 10000)), [=](const vector<int>& v) {
            T graph(n, 0, n-1);
            lli k = 0;
            FOR(i, n) {
                FOR(j, n) {
                    if(i == j) continue;
                    lli c = v[k++];
                    if(c > 0) {
                        graph.addEdge(i, j, c);
                    } else if(c < 0) {
                        graph.addEdge(j, i, -c);
                    }
                }
            }
            return graph;
        });
    });
}

#define CHECK(g) \
    lli f = g.flow();                  \
    lli N = g.adj.size();              \
    vi flows(N, 0);                    \
    FOR(i, N) {                        \
        FOR(k, g.adj[i].size()) {      \
            Edge e = g.e[g.adj[i][k]]; \
            if(e.c > 0) {              \
                RC_ASSERT(e.f <= e.c); \
                flows[i] -= e.f;       \
                flows[e.to] += e.f;    \
            }                          \
        }                              \
    }                                  \
    FOR(i, N) {                        \
        if(i != g.s && i != g.t) {     \
            RC_ASSERT(flows[i] == 0);  \
        }                              \
    }                                  \
    RC_ASSERT(flows[g.s] == -f);       \
    RC_ASSERT(flows[g.t] == f);

void testFlow() {
    rc::check("dinic: flow is valid", []() {
        Dinic g = *getGraph<Dinic>();
        CHECK(g);
    });

    rc::check("graph: check minCut", []() {
        Dinic g = *getGraph<Dinic>();
        lli flow = g.flow();
        vb minCut = g.minCut();
        RC_ASSERT(minCut[g.s] == true);
        RC_ASSERT(minCut[g.t] == false);

        lli N = g.adj.size();
        lli cut = 0;
        FOR(i, N) {
            FOR(k, g.adj[i].size()) {
                Edge e = g.e[g.adj[i][k]];
                if(e.c > 0 && minCut[i] == true && minCut[e.to] == false) {
                    cut += e.c;
                }
            }
        }

        RC_ASSERT(flow == cut);
    });

    rc::check("cap_scaling: flow is valid", []() {
        CapScaling g = *getGraph<CapScaling>();
        CHECK(g);
    });

    rc::check("dinic & cap_scaling: same flow", []() {
        Dinic d = *getGraph<Dinic>();
        CapScaling c(0, 0, 0);
        c.e = d.e;
        c.adj = d.adj;
        c.s = d.s;
        c.t = d.t;
        RC_ASSERT(d.flow() == c.flow());
    });
}

