struct UF {
    vi p;
    UF(lli n) : p(n) { FOR(i, n) p[i] = i; }
    lli find(lli i) { return p[i] == i ? i : p[i] = find(p[i]); }
    void unite(lli i, lli j) { p[find(i)] = find(j); }
};
