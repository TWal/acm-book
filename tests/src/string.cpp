#include <precontest/template.cpp>
#include <string/kmp.cpp>
#include <rapidcheck.h>

//Generates a "binary" string because it is more interesting to test
//string-processing algorithms with reduced alphabet size
const char alphaBegin = 'a';
const char alphaEnd = 'b';
static rc::Gen<string> getString() {
    return rc::gen::map<string>([&](const string& s) {
        string res(s);
        FOR(i, s.size()) {
            res[i] = ((unsigned char)s[i])%(alphaEnd-alphaBegin+1)+alphaBegin;
        }
        return res;
    });
}

void testString() {
    rc::check("kmpPreprocess(p)[i] is a border of p[0..i]", []() {
        string p = *getString().as("needle");
        RC_PRE(p.size() > 0);
        vi b = kmpPreprocess(p);

        for(lli i = 1; i < (lli)b.size(); ++i) {
            RC_ASSERT(p.substr(0, b[i]) == p.substr(i-b[i], b[i]));
        }
    });

    rc::check("kmpPreprocess(p)[i] is maximum", []() {
        string p = *getString().as("needle");
        RC_PRE(p.size() > 0);
        vi b = kmpPreprocess(p);

        for(lli i = 1; i < (lli)b.size(); ++i) {
            for(lli j = b[i]+1; j < i; ++j) {
                RC_ASSERT(p.substr(0, j) != p.substr(i-j, j));
            }
        }
    });

    rc::check("kmpSearch finds the substrings", []() {
        string t = *getString().as("haystack");
        string p = *rc::gen::scale(0.3, getString()).as("needle");
        RC_PRE(p.size() > 0);
        RC_PRE(t.size() > p.size());

        vi b = kmpPreprocess(p);

        vb res(t.size(), false);
        kmpSearch(b, p, t, [&](lli i) { res[i] = true; });
        FOR(i, res.size()) {
            RC_ASSERT((t.substr(i, p.size()) == p) == res[i]);
        }
    });
}
