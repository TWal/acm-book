// const int MAX_TRA = 26; //number of transitions
// char normalize(char c) { //normalize `c` in [0, ..., MAX_TRA-1]
//     return c - 'a';
// }

struct SuffixAutomaton {
    vector<array<lli, MAX_TRA>> aut;
    vi len, link;
    vb isFinal;
    lli last;

    lli insert(int le, int li, int cp = -1) {
        len.pb(le);
        link.pb(li);
        aut.emplace_back();
        FOR(i, MAX_TRA) aut.back()[i] = cp < 0 ? -1 : aut[cp][i];
        return aut.size()-1;
    }

    SuffixAutomaton() {
        last = insert(0, -1);
    }

    void reserve(lli n) {
        aut.reserve(2*n);
        link.reserve(2*n);
        len.reserve(2*n);
    }

    void add(char c) {
        c = normalize(c);
        lli p, cur = insert(len[last]+1, -2);
        for(p = last; p != -1 && aut[p][c] == -1; p = link[p]) {
            aut[p][c] = cur;
        }
        if(p != -1) {
            lli q = aut[p][c];
            if(len[q] == len[p]+1) {
                link[cur] = q;
            } else {
                lli qq = insert(len[p]+1, link[q], q);
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
        for(lli cur = last; cur >= 0; cur = link[cur]) isFinal[cur] = true;
    }
};
