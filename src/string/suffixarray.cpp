struct SuffixArray {
    lli n;
    //sa = suffix array, pos = inverse of sa
    vi sa, pos, lcp; //lcp[i] = longest common prefix of sa[i], sa[i+1]
    vector<pair<pii, lli>> v;

    SuffixArray(const string& s) : n(s.size()), sa(n), lcp(n-1), pos(n+1), v(n+1) {
        FOR(i, n+1) pos[i] = (i != n) ? s[i] : 0;
        for(lli k = 1; k <= n; k *= 2) {
            FOR(i, n+1) v[i] = mp(mp(pos[i], pos[(i+k) % (n+1)]), i);
            sort(v.begin(), v.end());
            lli cnt = 0;
            FORU(i, 1, n+1) {
                if (v[i-1].fst != v[i].fst) ++cnt;
                pos[v[i].snd] = cnt;
            }
            if(cnt ==  n) break;
        }
        FOR(i, n) sa[i] = v[i+1].snd;

        //build lcp, can be removed if you don't need it
        pos.pop_back();
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
