struct RMQ {
    vvi mins;
    RMQ(const vi& v) : mins(1, v) {
        lli n = v.size();
        mins.resize(log2(n)+1, vi());
        FOR(i, log2(n)) {
            mins[i+1].resize(n+1-(2<<i)); //2<<i == 1<<(i+1)
            FOR(j, mins[i+1].size()) {
                mins[i+1][j] = min(mins[i][j], mins[i][j+(1<<i)]);
            }
        }
    }

    //min(v[l:r])
    lli query(lli l, lli r) {
        lli lg = log2(r-l);
        return min(mins[lg][l], mins[lg][r-(1<<lg)]);
    }
};
