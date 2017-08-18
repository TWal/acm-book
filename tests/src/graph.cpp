#include "graph.h"
#include <precontest/template.cpp>
#include <graph/scc_tarjan.cpp>
#include <graph/scc_kosaraju.cpp>
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
        DummySCC dscc(g);
        SCCKosaraju scck(g);
        lli N = g.size();
        FOR(i, N) {
            FOR(j, N) {
                RC_ASSERT(dscc.sameSCC(i, j) == (scck.scc[i] == scck.scc[j]));
            }
        }
    });
}

