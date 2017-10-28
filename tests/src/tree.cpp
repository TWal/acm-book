#include "tree.h"
#include <precontest/template.cpp>
#include <datastruct/rmq.cpp>
#include <tree/lca.cpp>
#include <tree/lca_dyn.cpp>
#include <tree/heavy_light.cpp>
#include <rapidcheck.h>

lli mod(lli a, lli b) {
    return ((a%b)+b)%b;
}

static rc::Gen<vi> getPermutation(lli size) {
    return rc::gen::mapcat(rc::gen::container<vi>(size, rc::gen::arbitrary<lli>()), [](const vi& v) {
        vi res(v.size());
        FOR(i, v.size()) {
            res[i] = i;
        }
        FOR(i, v.size()) {
            swap(res[i], res[i+mod(v[i], v.size()-i)]);
        }
        return rc::gen::just(res);
    });
}


static rc::Gen<vi> getTree() {
    return rc::gen::mapcat(rc::gen::arbitrary<vi>(), [](const vi& v) {
        return rc::gen::mapcat(getPermutation(v.size()), [=](const vi& p) {
            vi res(v.size());
            FOR(i, v.size()) {
                if(i == 0) res[i] = 0;
                else res[i] = mod(v[i], i);
            }
            vi res2(v.size());
            FOR(i, v.size()) {
                res2[p[i]] = p[res[i]];
            }
            return rc::gen::just(res2);
        });
    });
}

static lli getRoot(const vi& tree) {
    FOR(i, tree.size()) {
        if(tree[i] == i) {
            return i;
        }
    }
    return -1;
}

struct ConnectedChecker {
    vvi g;
    vb seen;
    bool isConnected;
    ConnectedChecker(const vvi& g, lli root) : g(g), seen(g.size(), false), isConnected(true) {
        dfs(root);
        FOR(i, seen.size()) {
            if(!seen[i]) {
                isConnected = false;
            }
        }
    }
    void dfs(lli node) {
        if(seen[node]) return;
        seen[node] = true;
        for(lli neigh : g[node]) {
            dfs(neigh);
        }
    }
};

struct DummyLCA {
    vi p;
    DummyLCA(const vi& p) : p(p) {}
    vi ancestors(lli n) {
        vi res(1, n);
        while(p[n] != n) {
            n = p[n];
            res.pb(n);
        }
        return res;
    }
    lli query(lli a, lli b) {
        vi anca = ancestors(a);
        vi ancb = ancestors(b);
        set<lli> ancbSet(ancb.begin(), ancb.end());
        FOR(i, anca.size()) {
            if(ancbSet.count(anca[i])) {
                return anca[i];
            }
        }
        assert(false);
    }
};

void testTree() {
    rc::check("LCA: tree generator makes connected trees (meta-test)", []() {
        vi tree = *getTree().as("tree");
        RC_PRE(tree.size() >= 1);
        bool backEdge = *rc::gen::arbitrary<bool>().as("backEdge");
        vvi treeGraph(tree.size());
        lli root = -1;
        FOR(i, tree.size()) {
            if(tree[i] != i) {
                treeGraph[tree[i]].pb(i);
                if(backEdge) treeGraph[i].pb(tree[i]);
            } else {
                RC_ASSERT(root == -1);
                root = i;
            }
        }
        RC_ASSERT(root != -1);
        ConnectedChecker cc(treeGraph, root);
        RC_ASSERT(cc.isConnected);
    });

    rc::check("LCA", []() {
        vi tree = *getTree().as("tree");
        RC_PRE(tree.size() >= 1);
        bool backEdge = *rc::gen::arbitrary<bool>().as("backEdge");
        vvi treeGraph(tree.size());
        lli root = -1;
        FOR(i, tree.size()) {
            if(tree[i] != i) {
                treeGraph[tree[i]].pb(i);
                if(backEdge) treeGraph[i].pb(tree[i]);
            } else {
                root = i;
            }
        }
        LCA lca(treeGraph, root);
        DummyLCA dlca(tree);
        FOR(i, tree.size()) {
            FOR(j, tree.size()) {
                RC_ASSERT(lca.query(i, j) == dlca.query(i, j));
            }
        }
    });

    rc::check("LCADyn", []() {
        vi tree = *getTree().as("tree");
        RC_PRE(tree.size() >= 1);
        lli root = -1;
        FOR(i, tree.size()) {
            if(tree[i] == i) {
                root = i;
            }
        }

        LCADyn lca(tree.size(), root);
        DummyLCA dlca(tree);
        vb added(tree.size(), false);
        added[root] = true;
        lli nbAdded = 1;
        RC_ASSERT(lca.query(root, root) == dlca.query(root, root));
        while(nbAdded != tree.size()) {
            FOR(i, tree.size()) {
                if(!added[i] && added[tree[i]]) {
                    added[i] = true;
                    lca.add(i, tree[i]);
                    nbAdded += 1;
                    FOR(a, tree.size()) if(added[a]) {
                        FOR(b, tree.size()) if(added[b]) {
                            RC_ASSERT(lca.query(a, b) == dlca.query(a, b));
                        }
                    }
                }
            }
        }
    });

    rc::check("heavy_light: variables are coherent", []() {
        vi tree = *getTree().as("tree");
        RC_PRE(tree.size() >= 1);
        lli root = getRoot(tree);
        bool rootType = *rc::gen::arbitrary<bool>().as("rootType");
        if(rootType) tree[root] = -1;
        HLD hld(tree, root);

        FOR(i, tree.size()) {
            RC_ASSERT(0 <= hld.chain[i] && hld.chain[i] < hld.nchain);
            RC_ASSERT(0 <= hld.pos[i] && hld.pos[i] < hld.size[hld.chain[i]]);
        }

        vvi chains(hld.nchain);
        FOR(i, hld.nchain) {
            chains[i].resize(hld.size[i], -1);
        }
        FOR(i, tree.size()) {
            lli& val = chains[hld.chain[i]][hld.pos[i]];
            RC_ASSERT(val == -1);
            val = i;
        }
        FOR(i, hld.nchain) FOR(j, hld.size[i]) {
            RC_ASSERT(chains[i][j] >= 0);
            if(j == 0) {
                RC_ASSERT(chains[i][j] == hld.head[i]);
            } else {
                RC_ASSERT(chains[i][j-1] == tree[chains[i][j]]);
            }
        }
    });

    rc::check("heavy_light: at most log2(n)+1 chain from node to root", []() {
        vi tree = *getTree().as("tree");
        RC_PRE(tree.size() >= 1);
        lli root = getRoot(tree);
        bool rootType = *rc::gen::arbitrary<bool>().as("rootType");
        if(rootType) tree[root] = -1;
        HLD hld(tree, root);

        lli log2 = 1;
        while((1<<log2) < tree.size()) ++log2;

        FOR(i, tree.size()) {
            lli cnt = 0;
            lli node = i;
            while(node != root && node >= 0) {
                node = tree[hld.head[hld.chain[node]]];
                ++cnt;
            }
            RC_ASSERT(cnt <= log2+1);
        }
    });

    rc::check("heavy_light: lca is correct", []() {
        vi tree = *getTree().as("tree");
        RC_PRE(tree.size() >= 1);
        lli root = getRoot(tree);
        HLD hld(tree, root);
        DummyLCA dlca(tree);
        FOR(i, tree.size()) {
            FOR(j, tree.size()) {
                RC_ASSERT(hld.lca(i, j) == dlca.query(i, j));
            }
        }
    });
}

