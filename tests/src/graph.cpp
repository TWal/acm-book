#include "graph.h"
#include <precontest/template.cpp>
#include <graph/scc_tarjan.cpp>
#include <graph/scc_kosaraju.cpp>
#include <graph/articulation.cpp>
#include <graph/bridges.cpp>
#include <graph/dominator_tree.cpp>
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

struct NodeDominators {
    vvi adj;
    lli N;
    vb seen1, seen2;
    vi doms;
    NodeDominators(const vvi& adj, lli source, lli node) : adj(adj), N(adj.size()), seen1(N, false), seen2(seen1) {
        dfs(source, -1, seen1);
        dfs(source, node, seen2);
        FOR(i, N) {
            if(seen1[i] && !seen2[i]) {
                doms.pb(i);
            }
        }
    }
    void dfs(lli n, lli forbidden, vb& seen) {
        if(n == forbidden) return;
        if(seen[n]) return;
        seen[n] = true;
        for(lli neigh : adj[n]) {
            dfs(neigh, forbidden, seen);
        }
    }
};

struct DumbDomTree {
    lli N;
    vvi doms, invdom;
    vi dompar;
    DumbDomTree(const vvi& adj, lli source) : N(adj.size()), doms(N), invdom(N), dompar(N) {
        FOR(i, N) {
            NodeDominators nd(adj, source, i);
            invdom[i] = nd.doms;
        }
        FOR(i, N) {
            for(lli j : invdom[i]) {
                doms[j].pb(i);
            }
        }
        FOR(i, N) {
            if(i == source) {
                dompar[i] = source;
                continue;
            }
            dompar[i] = -1;
            for(lli d : doms[i]) if(d != i) {
                if(dompar[i] == -1 || inside(dompar[i], doms[d])) {
                    dompar[i] = d;
                }
            }
        }
    }
    bool inside(lli x, const vi& v) {
        FOR(i, v.size()) {
            if(v[i] == x) return true;
        }
        return false;
    }
};

struct TreeParents {
    vvi adj;
    vi par;
    TreeParents(const vvi& adj, lli root) : adj(adj), par(adj.size(), -1) {
        dfs(root, root);
    }
    void dfs(lli node, lli parent) {
        if(par[node] >= 0) return;
        par[node] = parent;
        for(lli neigh : adj[node]) {
            dfs(neigh, node);
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

    rc::check("dominator_tree", []() {
        vvi g = *getGraph().as("graph");
        lli n = g.size();
        lli source = *rc::gen::inRange<lli>(0, n);
        DominatorTree dt(g, source);

        TreeParents tp(dt.dom, source);
        DumbDomTree ddt(g, source);

        RC_ASSERT(tp.par == ddt.dompar);
    });
}

