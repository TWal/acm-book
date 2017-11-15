#include <precontest/template.cpp>
#include <datastruct/fenwick.cpp>
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
    vb activation(n, true);

    FOR(_, q) {
        lli type;
        scanf("%lld", &type);
        if(type == 0) {
            lli i, l, k;
            scanf("%lld%lld%lld", &i, &l, &k);
            auto it = revValues.find(k);
            if(it == revValues.end()) printf("0\n");
            else printf("%d\n", wt.rank(it->second, i, l+1));
        } else {
            lli r;
            scanf("%lld", &r);
            activation[r] = !activation[r];
            wt.activate(r, activation[r], false);
        }
    }
    return 0;
}

