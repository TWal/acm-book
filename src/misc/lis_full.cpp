// Returns the indices of the longest increasing subsequence of v
vi lis(const vi& v) {
    lli n = v.size(), c = 0;
    vi b(n), p(n), ans;
    for (int i = 0; i < n; ++i) {
        // Longest nondecreasing subsequence: replace with upper_bound
        p[i] = lower_bound(b.begin(), b.begin() + c, v[i]) - b.begin() + 1;
        b[p[i] - 1] = v[i];
        c = max(c, p[i]);
    }
    for (int i = n - 1; i >= 0; --i) if (p[i] == c) {
        ans.pb(i);
        c--;
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

