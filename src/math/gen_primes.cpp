vi genPrimes(lli N) {
    vb isp(N, false);
    isp[2] = true;
    for(lli i = 3; i < N; i += 2) isp[i] = true;
    for(lli i = 3; i < N; ++i) {
        if(isp[i]) for(lli j = i*i; j < N; j += 2*i) {
            isp[j] = false;
        }
    }
    vi primes;
    FOR(i, N) if(isp[i]) primes.pb(i);
    return primes;
}

