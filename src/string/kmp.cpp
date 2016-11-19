//p is the needle, returns the maximum border (proper prefix&suffix) of p[0..i]
vi kmpPreprocess(const string& p) {
    lli i = 0, j = -1;
    vi b(p.size()+1, 0);
    b[i] = j;
    while(i < p.size()) {
        while(j >= 0 && p[i] != p[j]) j = b[j];
        b[++i] = ++j;
    }
    return b;
}

//b = kmpPreprocess(p), p = needle, t = haystack, calls `report` on each match
void kmpSearch(const vi& b, const string& p,
        const string& t, function<void(lli)> report) {
    lli i = 0, j = 0;
    while(i < t.size()) {
        while(j >= 0 && t[i] != p[j]) j = b[j];
        i++; j++;
        if(j == p.size()) {
            report(i-j);
            j = b[j];
        }
    }
}

