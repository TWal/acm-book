//Calculate inverses mod p in a range. i*res[i] = 1 mod p
vi invmodRange(lli size, lli p) {
    vi res(size, -1);
    res[1] = 1;
    FORU(i, 2, size) {
        lli c = p/i+1;
        res[i] = (c*res[c*i-p])%p;
    }
    return res;
}
