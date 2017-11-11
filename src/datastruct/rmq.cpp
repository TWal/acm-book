template<class T>
struct RMQ {
    vector<T> v;
    vector<vector<lli>> mins;
    RMQ() {}
    RMQ(const vector<T>& v) { build(v); }
    void build(const vector<T>& vec) {
        v = vec;
        lli n = v.size();
        mins.resize(log2(n)+1);
        mins[0].resize(n);
        FOR(i, n) mins[0][i] = i;
        FOR(i, log2(n)) {
            mins[i+1].resize(n+1-(2<<i)); //2<<i == 1<<(i+1)
            FOR(j, mins[i+1].size()) {
                mins[i+1][j] = indmin(mins[i][j], mins[i][j+(1<<i)]);
            }
        }
    }
    lli indmin(lli i, lli j) { return v[i] < v[j] ? i : j; }
    //argmin(v[l:r])
    lli query(lli l, lli r) {
        lli lg = log2(r-l);
        return indmin(mins[lg][l], mins[lg][r-(1<<lg)]);
    }
};
