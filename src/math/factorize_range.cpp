vvi factorizeRange(lli l, lli r) {
    assert(l >= 2);
    lli nb = r-l;
    vi lr(nb);
    FOR(i, nb) lr[i] = l+i;
    vvi fact(nb);
    for(lli i = 2; i*i <= r; ++i) {
        for(lli ki = ((l+i-1)/i)*i; ki < r; ki += i) {
            lli ind = ki-l;
            while(lr[ind]%i == 0) {
                lr[ind] /= i;
                fact[ind].pb(i);
            }
        }
    }
    FOR(i, nb) if(lr[i] != 1) fact[i].pb(lr[i]);
    return fact;
}
