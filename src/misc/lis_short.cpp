//Returns the size of the longest increasing subsequence of `v`
lli lisSize(const vi& v) {
    vi d;
    FOR(i, v.size()) {
        auto it = lower_bound(d.begin(), d.end(), v[i]);
        if(it == d.end()) d.pb(v[i]);
        else *it = v[i];
    }
    return d.size();
}
 
