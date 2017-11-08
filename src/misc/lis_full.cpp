// Returns the indices of the longest increasing subsequence of v
vi lis(const vi& v) {
    lli n = v.size(), c = 0;
    vi b(n), p(n);
    FOR(i, n) {
        // Longest nondecreasing subsequence: replace with upper_bound
        p[i] = lower_bound(b.begin(), b.begin() + c, v[i]) - b.begin() + 1;
        b[p[i] - 1] = v[i];
        c = max(c, p[i]);
    }
    vi ans(c);
    FORD(i, 0, n) if (p[i] == c) ans[--c] = i;
    return ans;
}
