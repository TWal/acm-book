struct LCA {
    vvi tree;
    vi first;
    vii euler;
    RMQ<pii> rmq;

    LCA(const vvi& tree, lli root) : tree(tree), first(tree.size(), -1) {
        dfs(root, -1, 0);
        rmq.build(euler);
    }

    void dfs(lli node, lli prev, lli depth) {
        first[node] = euler.size();
        euler.pb(mt(depth, node));
        for(lli neigh : tree[node]) if(neigh != prev) {
            dfs(neigh, node, depth+1);
            euler.pb(mt(depth, node));
        }
    }

    lli query(lli a, lli b) {
        lli fa = first[a], fb = first[b];
        if(fa > fb) swap(fa, fb);
        return Y(euler[rmq.query(fa, fb+1)]);
    }
};
