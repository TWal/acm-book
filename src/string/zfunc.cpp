//z[i] = length of longest sustring starting at s[i] which is also a prefix of s
vi zfunc(const string& s) {
    lli l = -1, r = -1, n = s.size();
    vi z(n, n);
    for(lli i = 1; i < n; ++i) {
        //the min should be understood as i+z[i-l] < r ? z[i-l] : r-i
        z[i] = i >= r ? 0 : min(r-i, z[i-l]);
        while(i+z[i] < n && s[i+z[i]] == s[z[i]]) ++z[i];
        if(i+z[i] > r) l = i, r = i+z[i];
    }
    return z;
}
