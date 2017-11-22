// some other primes:
// 1e9+7, 1e9+9, 752099093, 875767681, 927539071, 588795733, 683894117
// smaller: 7193, 29389, 11621, 2053, 24923
const int nmod = 3;
const lli mods[nmod] = {2147483647, 2147483629, 2147483587}, base = 1665525;

struct HashT {
    lli h[nmod];

    HashT() { fill(h, h + nmod, 0); }

    lli& operator [] (int i) { return h[i]; }

    bool operator < (const HashT& o) const {
        FOR(k, nmod) if (h[k] != o.h[k]) return h[k] < o.h[k];
        return false;
    }

    bool operator == (const HashT& o) const {
        FOR(k, nmod) if (h[k] != o.h[k]) return false;
        return true;
    }
};

struct Hash {
    vector<HashT> powh, preh;

    template<typename T>
    Hash(T v, int n) {
        powh.resize(n + 1); preh.resize(n + 1);
        FOR(k, nmod) powh[0][k] = 1;
        FOR(i, n) FOR(k, nmod) {
            powh[i + 1][k] = powh[i][k] * base % mods[k];
            preh[i + 1][k] = (preh[i][k] * base % mods[k] + v[i]) % mods[k];
        }
    }

    // [off, ..., off+len-1]
    HashT get(int off, int len) {
        HashT ans;
        FOR(k, nmod) ans[k] = ((preh[off + len][k] - preh[off][k] * powh[len][k]) % mods[k] + mods[k]) % mods[k];
        return ans;
    }

    // Returns the hash of `ab`, with |b| = blen
    HashT append(HashT a, HashT b, int blen) {
        HashT ans;
        FOR(k, nmod) ans[k] = (a[k] * powh[blen][k] % mods[k] + b[k]) % mods[k];
        return ans;
    }
};

