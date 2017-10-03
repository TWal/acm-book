//Works when abs(v[i]) < 2^63/v.size()
//If before, abs(v[i]) <= M, then after abs(v[i]) <= M*v.size()
//Fourier transform for convolutions like FOR(i, n) FOR(j, n) c[i^j] += a[i]*b[j]
void hadamard(vi& v, bool inverse) {
    assert((v.size() & (v.size()-1)) == 0); //check that v.size() is a power of 2
    lli z = v.size() >> 1;
    while(z > 0) {
        FOR(i, v.size()) {
            if(i & z) continue;
            lli a = v[i], b = v[i|z];
            v[i] = a+b, v[i|z] = a-b;
        }
        z >>= 1;
    }
    if(inverse) {
        FOR(i, v.size()) {
            v[i] /= (lli)v.size();
        }
    }
}

