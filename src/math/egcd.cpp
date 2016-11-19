//(g, u, v) = egcd(a, b) => a*u+b*v = g
piii egcd(lli a, lli b) {
    if(a == 0) return mt(b, 0, 1);
    else {
        lli g, u, v;
        tie(g, u, v) = egcd(b%a, a);
        return mt(g, v-(b/a)*u, u);
    }
}
