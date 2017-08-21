struct BIT {
    vi v;
    BIT(lli n) : v(n+1, 0) {}
    //sum(v[0:i+1])
    lli get(lli i) {
        lli r = 0;
        for(i += 1; i > 0; i -= i & -i) r += v[i];
        return r;
    }
    //v[i] += val
    void add(lli i, lli val) {
        for(i += 1; i < v.size(); i += i & -i) v[i] += val;
    }
};
