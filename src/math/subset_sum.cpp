//vafter[i] = sum_(j submask of i) vbefore[j]
//Fourier transform for convolutions like FOR(i, n) FOR(j, n) c[i|j] += a[i]*b[j]
void subsetSum(vi& v, bool inverse) {
    assert((v.size() & (v.size()-1)) == 0); //check that v.size() is a power of 2
    lli z = v.size() >> 1;
    while(z > 0) {
        FOR(i, v.size()) {
            if(i & z) continue;
            v[i|z] += (inverse ? -v[i] : v[i]);
        }
        z >>= 1;
    }
}

