template<typename T>
pair<vector<T>, vector<T>> euclid(vector<T> f, const vector<T>& g) {
    assert(f.size() >= g.size());
    lli m = g.size();
    vector<T> q(f.size()-m+1, 0);
    FORD(i, m-1, f.size()) {
        T c = f[i]/g.back();
        FOR(j, m) {
            f[i-j] = f[i-j] - c*g[m-j-1];
        }
        q[i+1-m] = c;
    }
    f.resize(m-1);
    return mp(q, f);
}
