#include <precontest/template.cpp>

struct BIT {
    BIT() {}
    BIT(lli) {}
    void add(lli, lli) {}
    lli get(lli x) const { return x; }
};

#include <datastruct/wavelettree.cpp>

int main() {
    lli n, q;
    scanf("%lld%lld", &n, &q);
    vi a(n);
    FOR(i, n) scanf("%lld", &a[i]);

    vi values = a;
    sort(ALL(values));
    auto it = unique(ALL(values));
    values.resize(distance(values.begin(), it));

    map<lli, lli> revValues;
    FOR(i, values.size()) revValues[values[i]] = i;
    FOR(i, n) a[i] = revValues[a[i]];

    WaveletTree wt(values.size(), a);

    FOR(_, q) {
        lli type;
        scanf("%lld", &type);
        if(type == 0) {
            lli i, l, k;
            scanf("%lld%lld%lld", &i, &l, &k);
            lli d = wt.kth(k, 0, i+1);
            if(wt.rank(d, 0, n) < l) {
                printf("-1\n");
                continue;
            }
            lli beg = 0, end = n;
            while(end-beg > 1) {
                lli mid = (beg+end)/2;
                if(wt.rank(d, 0, mid) >= l) {
                    end = mid;
                } else {
                    beg = mid;
                }
            }
            printf("%lld\n", beg);
        } else {
            lli i;
            scanf("%lld", &i);
            if(i == n-1) continue;
            wt.wswap(i);
        }
    }
    return 0;
}
