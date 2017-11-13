#include <precontest/template.cpp>
#include <string/suffixarray.cpp>

char buf[1000*1000];
int main() {
    scanf("%s", buf); string s = buf;
    lli n = s.size();

    SuffixArray sa(s);

    vi res(26, 0);
    FOR(i, n) {
        res[s[sa.sa[i]]-'a'] += n-sa.sa[i]-(i+1 == n ? 0 : sa.lcp[i]);
    }

    FOR(i, 26) printf("%lld%c", res[i], " \n"[i==25]);
}
