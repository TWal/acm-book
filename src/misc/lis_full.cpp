//returns the indices of the longest increasing subsequence of `v`
vi lis(const vi& v) {
    lli n = v.size();
    vi p(n, 0), m(n+1, 0);
    lli l = 0;
    FOR(i, n) {
        lli beg = 1, end = l;
        while(beg <= end) {
            lli mid = (beg+end)/2;
            if(v[m[mid]] < v[i]) beg = mid+1;
            else end = mid-1;
        }
        p[i] = m[beg-1];
        m[beg] = i;
        l = max(l, beg);
    }
    vi res(l);
    lli k = m[l];
    FOR(i, l) {
        res[l-i-1] = k;
        k = p[k];
    }
    return res;
}
