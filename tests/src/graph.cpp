#include "graph.h"
#include <precontest/template.cpp>
#include <graph/scc_tarjan.cpp>
#include <graph/scc_kosaraju.cpp>
#include <graph/articulation.cpp>
#include <graph/bridges.cpp>
#include <rapidcheck.h>

static rc::Gen<vvi> getGraph() {
    return rc::gen::withSize([](int n) {
        n += 1;
        return rc::gen::map(rc::gen::container<vector<bool>>(n*(n-1), rc::gen::arbitrary<bool>()), [=](const vb& v) {
            vvi graph(n);
            lli k = 0;
            FOR(i, n) {
                FOR(j, n) {
                    if(i == j) continue;
                    if(v[k++]) {
                        graph[i].pb(j);
                    }
                }
            }
            return graph;
        });
    });
}

static rc::Gen<vvi> getUndirectedGraph() {
    return rc::gen::withSize([](int n) {
        n += 1;
        return rc::gen::map(rc::gen::container<vector<bool>>(n*(n-1)/2, rc::gen::arbitrary<bool>()), [=](const vb& v) {
            vvi graph(n);
            lli k = 0;
            FOR(i, n) {
                FORU(j, i+1, n) {
                    if(i == j) continue;
                    if(v[k++]) {
                        graph[i].pb(j);
                        graph[j].pb(i);
                    }
                }
            }
            return graph;
        });
    });
}


struct DummySCC {
    lli N;
    vvb mat, tmat;
    DummySCC(const vvi& adj) : N(adj.size()), mat(N, vb(N)), tmat(N, vb(N)) {
        FOR(i, N) {
            mat[i][i] = true;
            tmat[i][i] = true;
            for(lli j : adj[i]) {
                mat[i][j] = true;
                tmat[j][i] = true;
            }
        }

        //Floyd-Warshall
        FOR(k, N) {
            FOR(i, N) {
                FOR(j, N) {
                    if(mat[i][k] && mat[k][j]) mat[i][j] = true;
                    if(tmat[i][k] && tmat[k][j]) tmat[i][j] = true;
                }
            }
        }
    }

    bool sameSCC(lli i, lli j) {
        return mat[i][j] && tmat[i][j];
    }
};

struct CompConnVert {
    vvi adj;
    lli N, ncc;
    vb seen;
    CompConnVert(const vvi& _adj, lli removedVert) : adj(_adj), N(adj.size()), ncc(0), seen(N, false) {
        if(removedVert >= 0) seen[removedVert] = true;
        FOR(i, N) if(!seen[i]) {
            dfs(i);
            ncc += 1;
        }
    }
    void dfs(lli v) {
        seen[v] = true;
        for(lli n : adj[v]) if(!seen[n]) {
            dfs(n);
        }
    }
};

struct CompConnEdge {
    vvi adj;
    lli N, ncc, e1, e2;
    vb seen;
    CompConnEdge(const vvi& _adj, lli _e1, lli _e2) : adj(_adj), N(adj.size()), ncc(0), e1(_e1), e2(_e2), seen(N, false) {
        FOR(i, N) if(!seen[i]) {
            dfs(i);
            ncc += 1;
        }
    }
    bool ok(lli _e1, lli _e2) {
        return !((e1 == _e1 && e2 == _e2) || (e1 == _e2 && e2 == _e1));
    }
    void dfs(lli v) {
        seen[v] = true;
        for(lli n : adj[v]) if(!seen[n] && ok(v, n)) {
            dfs(n);
        }
    }
};


void testGraph() {
    rc::check("scc_tarjan", []() {
        vvi g = *getGraph();
        DummySCC dscc(g);
        SCCTarjan scct(g);
        lli N = g.size();
        FOR(i, N) {
            FOR(j, N) {
                RC_ASSERT(dscc.sameSCC(i, j) == (scct.scc[i] == scct.scc[j]));
            }
        }
    });

    rc::check("scc_kosaraju", []() {
        vvi g = *getGraph();
        SCCTarjan scct(g);
        SCCKosaraju scck(g);
        lli N = g.size();
        FOR(i, N) {
            FOR(j, N) {
                RC_ASSERT((scct.scc[i] == scct.scc[j]) == (scck.scc[i] == scck.scc[j]));
            }
        }
    });

    rc::check("articulation", []() {
        vvi g = *getUndirectedGraph();
        CompConnVert nbcc(g, -1);
        Articulation art(g);
        lli N = g.size();
        FOR(i, N) {
            CompConnVert newNbcc(g, i);
            RC_ASSERT((nbcc.ncc < newNbcc.ncc) == art.ap[i]);
        }
    });

    rc::check("bridge", []() {
        auto edgeEqual = [](pii e1, pii e2) -> bool {
            return (X(e1) == X(e2) && Y(e1) == Y(e2)) || (X(e1) == Y(e2) && Y(e1) == X(e2));
        };
        vvi g = *getUndirectedGraph();
        CompConnEdge nbcc(g, -1, -1);
        Bridges bridges(g);
        lli N = g.size();
        FOR(i, N) {
            for(lli j : g[i]) {
                bool isBridge = false;
                for(pii e : bridges.bridges) {
                    if(edgeEqual(e, mt(i, j))) {
                        isBridge = true;
                        break;
                    }
                }
                CompConnEdge newNbcc(g, i, j);
                RC_ASSERT((nbcc.ncc < newNbcc.ncc) == isBridge);
            }
        }
    });
}

