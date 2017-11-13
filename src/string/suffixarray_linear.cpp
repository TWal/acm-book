struct SuffixArrayLinear {
    lli n;
    SuffixAutomaton saut;
    vector<array<int, 3>> inte;
    vi sa, pos, lcp;

    void build(int e, int prof = 0) {
        if(inte[e][0] == -1) {
            inte[e][2] = prof;
            inte[e][0] = sa.size();
            if(saut.isFinal[e]) sa.pb(prof);
            FOR(i, MAX_TRA) {
                if(saut.aut[e][i] != -1) {
                    build(saut.aut[e][i], prof+1);
                }
            }
            inte[e][1] = sa.size();
        } else {
            FORU(i, inte[e][0], inte[e][1]) {
                sa.pb(sa[i] - inte[e][2] + prof);
            }
        }
    }

    SuffixArrayLinear(const string& s) : n(s.size()), pos(n), lcp(n-1) {
        saut.reserve(n);
        FOR(i, n) saut.add(s[i]);
        saut.computeFinals();

        sa.reserve(n);
        inte.assign(saut.aut.size(), {{-1, -1, -1}});
        build(0);
        FOR(i, n) sa[i] = n-sa[i+1];
        sa.pop_back();

        FOR(i, n) pos[sa[i]] = i;
        lli k = 0;
        FOR(i, n) {
            if(pos[i]+1 != n) {
                lli j = sa[pos[i]+1];
                while(s[i+k] == s[j+k]) ++k;
                lcp[pos[i]] = k;
                if(k > 0) --k;
            }
        }
    }
};
