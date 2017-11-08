lli invmod(lli a, lli m) {
    return a == 1 ? 1 : (1-invmod(m%a, a)*m)/a+m;
}
