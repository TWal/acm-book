struct HLD {
    //n = number of nodes | nchain = number of chains | par[node] = parent of node
    //treesize[node] = size of node's subtree | depth[node] = depth of node
    //head[chainid] = highest node of chain | pos[node] = position of node in chain
    //chain[node] = chainid of node | size[chainid] = size of chain
    lli n, nchain;
    vi par, treesize, depth, head, pos, chain, size;
    vvi adj;

    HLD(const vi& par, lli root) : n(par.size()), nchain(0), par(par), depth(n),
    treesize(n), head(n, -1), pos(n), chain(n), size(n, 0), adj(n) {
        FOR(i, n) if(par[i] >= 0 && par[i] != i) adj[par[i]].pb(i);
        compSub(root, 0);
        compHLD(root);
        nchain += 1;
    }

    lli compSub(lli node, lli d) {
        treesize[node] = 1;
        depth[node] = d;
        for(lli neigh : adj[node]) {
            treesize[node] += compSub(neigh, d+1);
        }
        return treesize[node];
    }

    void compHLD(lli node) {
        if(head[nchain] == -1) head[nchain] = node;
        chain[node] = nchain;
        pos[node] = size[nchain]++;

        lli maxn = -1, maxs = -1;
        for(lli neigh : adj[node]) {
            if(treesize[neigh] > maxs) {
                maxs = treesize[neigh];
                maxn = neigh;
            }
        }
        if(maxn >= 0) compHLD(maxn);
        for(lli neigh : adj[node]) {
            if(neigh != maxn) {
                nchain += 1;
                compHLD(neigh);
            }
        }
    }

    //This function can be adapted to agregate some information on the path
    lli lca(lli a, lli b) {
        while(chain[a] != chain[b]) {
            if(depth[head[chain[a]]] < depth[head[chain[b]]]) {
                swap(a, b);
            }
            a = par[head[chain[a]]];
        }
        if(depth[a] < depth[b]) {
            swap(a, b);
        }
        return b;
    }
};
