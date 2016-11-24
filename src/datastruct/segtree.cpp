//Works with any monoid (even non-commutative ones)
struct SegTree {
    lli n;
    vi t;
    // v = initial values in the tree
    SegTree(const vi& v) : n(v.size()), t(2*n, 0) {
        copy(v.begin(), v.end(), t.begin()+n);
        for(lli i = n-1; i > 0; --i) t[i] = t[2*i] + t[2*i+1];
    }
    // v[p] = value
    void modify(lli p, lli value) {
        for(t[p += n] = value; p /= 2;) t[p] = t[2*p] + t[2*p+1];
    }
    // returns sum(v[l:r])
    lli query(lli l, lli r) {
        lli resl = 0, resr = 0;
        for(l += n, r += n; l < r; l /= 2, r /= 2) {
            if(l&1) resl = resl + t[l++];
            if(r&1) resr = t[--r] + resr;
        }
        return resl + resr;
    }
};
