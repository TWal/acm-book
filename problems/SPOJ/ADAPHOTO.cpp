#include <precontest/template.cpp>
#include <string/suffixarray.cpp>

char buf[1000*1000+42];

int main() {
    scanf("%s", buf); string s1 = buf;
    scanf("%s", buf); string s2 = buf;
    lli n = s1.size(), m = s2.size();

    string s = s1;
    s.pb('#');
    s += s2;

    SuffixArray sa(s);

    lli res = -1;
    FOR(i, s.size()-1) {
        if((sa.sa[i] < n) != (sa.sa[i+1] < n)) {
            res = max(res, sa.lcp[i]);
        }
    }
    printf("%lld\n", res);
    return 0;
}
