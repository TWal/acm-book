struct BITRange {
    BIT bit1, bitx;
    BITRange(lli n) : bit1(n), bitx(n) {}
    //sum(v[0:i+1])
    lli get(lli i) {
        return i*bitx.get(i) + bit1.get(i);
    }
    //v[i:] += val or FORU(k, i, n) v[k] += val
    void addRange(lli i, lli val) {
        bitx.add(i, val);
        bit1.add(i, -(i-1)*val);
    }
    //v[i] += val
    void add(lli i, lli val) {
        bit1.add(i, val);
    }
};
