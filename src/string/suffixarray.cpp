struct SuffixArray {
    lli n, gap;
    //sa = suffix array, pos = inverse of sa
    vi sa, pos, lcp; //lcp[i] = longest common prefix of sa[i], sa[i+1]

    struct Compare {
        const SuffixArray& s;
        Compare(const SuffixArray& s) : s(s) {}
        bool operator()(int i, int j) const {
            if (s.pos[i] != s.pos[j]) return s.pos[i] < s.pos[j];
            i += s.gap;
            j += s.gap;
            return i < s.n && j < s.n ? s.pos[i] < s.pos[j] : i > j;
        }
    };

    SuffixArray(const string& s) : n(s.size()), sa(n), pos(n), lcp(n-1) {
        vi tmp(n);
        FOR(i, n) {
            sa[i] = i;
            pos[i] = s[i];
        }
        Compare cmp(*this);

        for(gap = 1; gap <= n; gap *= 2) {
            sort(sa.begin(), sa.end(), cmp);
            FORU(i, 1, n) tmp[i] = tmp[i - 1] + cmp(sa[i - 1], sa[i]);
            FOR(i, n) pos[sa[i]] = tmp[i];
        }

        //build lcp, can be removed if you don't need it
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
