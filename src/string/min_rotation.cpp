lli min_rotation(const string& s) {
    string t = s+s;

    lli a = 0;
    FOR(b, s.size()) FOR(i, s.size()) {
        if(a+i == b || t[a+i] < t[b+i]) { b += max(0ll, i-1); break; }
        if(t[a+i] > t[b+i]) { a = b; break; }
    }
    return a;
}
