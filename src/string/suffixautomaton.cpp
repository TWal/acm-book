// const int MAX_TRA = 26; //number of transitions
// char normalize(char c) { //normalize `c` in [0, ..., MAX_TRA-1]
//     return c - 'a';
// }

struct SuffixAutomaton {
    array<int, MAX_TRA> defNode;
    vector<array<int, MAX_TRA>> aut;
    vector<int> len, link;
    vb isFinal;
    int last;

    int insert(int le, int li, int cp = -1) {
        len.pb(le);
        link.pb(li);
        aut.pb(cp == -1 ? defNode : aut[cp]);
        return aut.size()-1;
    }

    SuffixAutomaton() {
        FOR(i, MAX_TRA) defNode[i] = -1;
        last = insert(0, -1);
    }

    void reserve(int n) {
        aut.reserve(2*n);
        link.reserve(2*n);
        len.reserve(2*n);
    }

    void add(char c) {
        c = normalize(c);
        int p, cur = insert(len[last]+1, -2);
        for(p = last; p != -1 && aut[p][c] == -1; p = link[p]) {
            aut[p][c] = cur;
        }
        if(p != -1) {
            int q = aut[p][c];
            if(len[q] == len[p]+1) {
                link[cur] = q;
            } else {
                int qq = insert(len[p]+1, link[q], q);
                link[q] = link[cur] = qq;
                for(; p != -1 && aut[p][c] == q; p = link[p]) {
                    aut[p][c] = qq;
                }
            }
        } else {
            link[cur] = 0;
        }
        last = cur;
    }

    void computeFinals() {
        isFinal.assign(aut.size(), false);
        for(int cur = last; cur >= 0; cur = link[cur]) isFinal[cur] = true;
    }
};
