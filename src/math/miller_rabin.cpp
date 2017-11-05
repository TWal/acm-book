bool millerRabin(llu n, llu a) {
    llu r = n-1;
    while(true) {
        llu x = powmod(a, r, n);
        if(x == n-1) return true;
        if(x != 1) return false;
        if(r%2 == 0) return true;
        r /= 2;
    }
}

bool isPrime(llu n) {
    const llu prs[11] = {2,3,5,7,11,13,17,19,23,29,31};
    if(n < 2) return false;
    FOR(i, 11) {
        if(n == prs[i]) return true;
        if(!millerRabin(n, prs[i])) return false;
    }
    return true;
}

