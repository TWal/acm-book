llu addmod(llu a, llu b, llu m) {
    return (a >= m-b) ? a+b-m : a+b;
}

llu multmodInt128(llu a, llu b, llu m) {
    unsigned __int128 la = a, lb = b, lm = m;
    return llu((la*lb)%lm);
}

//if you don't have int128, this works in O(log(b))
llu multmod(llu a, llu b, llu m) {
    a %= m; b %= m;
    llu res = 0;
    while(b > 0) {
        if(b%2 == 1) res = addmod(res, a, m);
        a = addmod(a, a, m);
        b = b/2;
    }
    return res;
}

llu powmod(llu a, llu b, llu m) {
    if(b == 0) return 1;
    llu tmp = powmod(multmod(a, a, m), b/2, m);
    return (b%2 == 1) ? multmod(a, tmp, m) : tmp;
}
