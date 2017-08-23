//Works with any monoid (even non-commutative ones)
//Must implement: T() -> zero; T operator+(const T&)
template<class T>
struct SegTree {
    lli n;
    vector<T> t;
    // v = initial values in the tree
    SegTree(const vector<T>& v) : n(v.size()), t(2*n, T()) {
        copy(v.begin(), v.end(), t.begin()+n);
        for(lli i = n-1; i > 0; --i) t[i] = t[2*i] + t[2*i+1];
    }
    // v[p] = value
    void modify(lli p, T value) {
        for(t[p += n] = value; p /= 2;) t[p] = t[2*p] + t[2*p+1];
    }
    // v[p] = v[p] + value (copy-paste modify)
    void inc(lli p, T value) {
        p += n;
        for(t[p] = t[p] + value; p /= 2;) t[p] = t[2*p] + t[2*p+1];
    }
    // returns sum(v[l:r])
    T query(lli l, lli r) {
        T resl = T(), resr = T();
        for(l += n, r += n; l < r; l /= 2, r /= 2) {
            if(l&1) resl = resl + t[l++];
            if(r&1) resr = t[--r] + resr;
        }
        return resl + resr;
    }
};
