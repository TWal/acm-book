llu pollardrho(llu n) {
    if(n == 4) return 2; //prevent an infinite loop
    llu x = 2, y = 2, c = 1, d = 1;
    while(d == 1) {
        x = addmod(multmod(x, x, n), c, n);
        y = addmod(multmod(y, y, n), c, n);
        y = addmod(multmod(y, y, n), c, n);
        d = __gcd(x < y ? y-x : x-y, n);
        if(d == n) {
            x = y = rand()%n;
            c = rand()%n;
            d = 1;
        }
    }
    return d;
}
