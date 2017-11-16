// rad[x] = radius of the largest palindrome centered at index x/2
// s   = b a n a n a a
// rad = 0000102010010
vi manacher(const string& s) {
    lli n = s.size();
    lli m = 2*n-1;
    vi rad(m, 0);
    vector<char> t(m, -1);
    FOR(i, n) t[2*i] = s[i];

    lli x = 0;
    FORU(i, 1, m) {
        lli& r = rad[i];
        if(i <= x+rad[x]) r = min(rad[x+x-i], x+rad[x]-i);
        while(i-r-1 >= 0 && i+r+1 < m && t[i-r-1] == t[i+r+1]) ++r;
        if(i+r >= x+rad[x]) x = i;
    }

    FOR(i, m) if(i-rad[i] == 0 || i+rad[i] == m-1) ++rad[i];
    FOR(i, m) rad[i] /= 2;

    return rad;
}
