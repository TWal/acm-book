//node.nexts represent the trie (the lli is an index of the vector<node> `T`
//repr is the path to follow to get to this node from the root node
//back is a "fail" edge, with the property: node.T[fail].repr is the largest
//  proper suffix of node.repr which is also a prefix of a needle
//ends is the set of word we match at the current node, with the property:
//  it's the set of needles that are a suffix of node.repr
struct node {
    unordered_map<char, lli> nexts;
    lli back;
    unordered_set<lli> ends;
    string repr; //not always useful
};

//N = needles
vector<node> ahoPreprocess(const vector<string>& N) {
    vector<node> T(1, node());
    FOR(i, N.size()) {
        lli n = 0;
        for(char c : N[i]) {
            if(T[n].nexts.count(c) == 0) {
                T[n].nexts[c] = T.size();
                T.pb(node());
                T.back().repr = T[n].repr + c; //not always useful
            }
            n = T[n].nexts[c];
        }
        T[n].ends.insert(i);
    }

    queue<lli> q; q.push(0);
    T[0].back = -1;
    while(!q.empty()) {
        lli n = q.front(); q.pop();
        // Remove this line if only the largest match at each position is needed
        if(T[n].back >= 0) T[n].ends.insert(ALL(T[T[n].back].ends));
        for(auto p : T[n].nexts) {
            lli b = T[n].back;
            while(b >= 0 && T[b].nexts.count(p.first) == 0) b = T[b].back;
            T[p.second].back = (b < 0) ? 0 : T[b].nexts[p.first];
            q.push(p.second);
        }
    }
    return T;
}

//T = preprocessing, H = haystack. Calls report on each match, at the end of the
//matched string: report(i, p) => H.substr(i-N[p].size()+1, N[p].size()) == N[p]
void ahoFind(vector<node>& T, const string& H, function<void(lli, lli)> report){
    lli S = 0;
    FOR(i, H.size()) {
        while(S >= 0 && T[S].nexts.count(H[i]) == 0) S = T[S].back;
        S = (S < 0) ? 0 : T[S].nexts[H[i]];
        for(auto p : T[S].ends) {
            report(i, p);
        }
    }
}
