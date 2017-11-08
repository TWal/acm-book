template<typename T>
vector<T> multPolySlow(const vector<T>& p, const vector<T>& q) {
    vector<T> res(p.size() + q.size() - 1, T());
    FOR(i, p.size()) FOR(j, p.size()) res[i+j] = res[i+j] + p[i]*q[j];
    return res;
}

template<typename T>
T linearRec(lli n, vector<T> P, vector<T> vals) {
    assert(vals.size()+1 == P.size());
    assert(vals.size() >= 2);

    vector<T> Q(vals.size(), T()), Ppow(vals.size(), T());
    Q[1] = Ppow[0] = 1;
    while(n > 0) {
        if(n%2 == 1) {
            Ppow = euclid(multPolySlow(Ppow, Q), P).second;
        }
        Q = euclid(multPolySlow(Q, Q), P).second;
        n /= 2;
    }

    T res = T();
    FOR(i, vals.size()) {
        res = res + vals[i]*Ppow[i];
    }
    return res;
}
