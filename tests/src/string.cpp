#include <precontest/template.cpp>
#include <string/kmp.cpp>
#include <string/zfunc.cpp>
#include <string/ahocorasick.cpp>
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

static bool isPrefix(const string& big, const string& small) {
    return big.substr(0, small.size()) == small;
}

static string getSuffix(const string& s, lli size) {
    return s.substr(s.size()-size, size);
}

static bool isSuffix(const string& big, const string& small) {
    return small.size() <= big.size() && getSuffix(big, small.size()) == small;
}

void testString() {
    rc::check("kmp: kmpPreprocess(p)[i] is a border of p[0..i]", []() {
        string p = *getString().as("needle");
        RC_PRE(p.size() > 0);
        vi b = kmpPreprocess(p);

        for(lli i = 1; i < (lli)b.size(); ++i) {
            RC_ASSERT(p.substr(0, b[i]) == p.substr(i-b[i], b[i]));
        }
    });

    rc::check("kmp: kmpPreprocess(p)[i] is maximum", []() {
        string p = *getString().as("needle");
        RC_PRE(p.size() > 0);
        vi b = kmpPreprocess(p);

        for(lli i = 1; i < (lli)b.size(); ++i) {
            for(lli j = b[i]+1; j < i; ++j) {
                RC_ASSERT(p.substr(0, j) != p.substr(i-j, j));
            }
        }
    });

    rc::check("kmp: kmpSearch finds the substrings", []() {
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

    rc::check("zfunc: s[i..i+z[i]] is a prefix of s", []() {
        string s = *getString();
        RC_PRE(s.size() > 0);
        vi z = zfunc(s);
        for(lli i = 0; i < (lli)s.size(); ++i) {
            RC_ASSERT(s.substr(0, z[i]) == s.substr(i, z[i]));
        }
    });

    rc::check("zfunc: z[i] is maximum", []() {
        string s = *getString();
        RC_PRE(s.size() > 0);
        vi z = zfunc(s);
        for(lli i = 0; i < (lli)s.size(); ++i) {
            if(i+z[i] < s.size()) {
                RC_ASSERT(s[z[i]] != s[i+z[i]]);
            }
        }
    });

    rc::Gen<vector<string>> needlesGen = rc::gen::nonEmpty(rc::gen::container<vector<string>>(rc::gen::nonEmpty(rc::gen::scale(0.3, getString())))).as("needles");

    rc::check("ahocorasick: trie is a tree", [&]() {
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);
        vb seen(prep.size(), false);
        function<void(lli)> dfs = [&](lli node) {
            RC_ASSERT(!seen[node] && "not acyclic");
            seen[node] = true;
            for(auto p : prep[node].nexts) {
                dfs(p.second);
            }
        };
        dfs(0);
        for(bool b : seen) {
            RC_ASSERT(b && "not connected");
        }
    });

    rc::check("ahocorasick: node.repr is good", [&]() {
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);
        function<void(lli, const string&)> dfs = [&](lli node, const string& s) {
            RC_ASSERT(s == prep[node].repr);
            for(auto p : prep[node].nexts) {
                dfs(p.second, s + p.first);
            }
        };
        dfs(0, "");
    });

    rc::check("ahocorasick: (lemma 1) node.back.repr is a proper suffix of node.repr and a prefix of a keyword", [&]() {
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);
        for(node n : prep) {
            if(n.back < 0) continue;
            string backrepr = prep[n.back].repr;
            RC_ASSERT(n.repr.size() > backrepr.size() && "suffix non proper");
            RC_ASSERT(isSuffix(n.repr, backrepr) && "not a suffix");
            bool isNeedlePrefix = false;
            for(string needle : needles) {
                if(needle.size() >= backrepr.size() && isPrefix(needle, backrepr)) {
                    isNeedlePrefix = true;
                }
            }
            RC_ASSERT(isNeedlePrefix);
        }
    });

    rc::check("ahocorasick: (lemma 1) node.back.repr is the maximal one", [&]() {
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);
        for(node n : prep) {
            if(n.back < 0) continue;
            for(lli i = prep[n.back].repr.size()+1;  i < n.repr.size()-1; ++i) {
                string cur = getSuffix(n.repr, i);
                for(string needle : needles) {
                    RC_ASSERT(!(needle.size() >= cur.size() && isPrefix(needle, cur)));
                }
            }
        }
    });

    rc::check("ahocorasick: (lemma 2) node.ends are suffix of node.repr", [&]() {
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);
        for(node n : prep) {
            for(lli out : n.ends) {
                RC_ASSERT(isSuffix(n.repr, needles[out]));
            }
        }
    });

    rc::check("ahocorasick: (lemma 2) needles that are suffix of node.repr are in node.ends", [&]() {
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);
        for(node n : prep) {
            FOR(i, needles.size()) {
                if(isSuffix(n.repr, needles[i])) {
                    RC_ASSERT(n.ends.count(i) == 1);
                }
            }
        }
    });

    rc::check("ahocorasick: ahoFind works", [&]() {
        string haystack = *getString().as("haystack");
        vector<string> needles = *needlesGen;
        vector<node> prep = ahoPreprocess(needles);

        vector<unordered_set<lli>> result(haystack.size());
        ahoFind(prep, haystack, [&](lli i, lli p) {
            result[i-needles[p].size()+1].insert(p);
        });

        FOR(i, haystack.size()) {
            unordered_set<lli> curRes;
            FOR(j, needles.size()) {
                if(haystack.substr(i, needles[j].size()) == needles[j]) {
                    curRes.insert(j);
                }
            }
            RC_ASSERT(curRes == result[i]);
        }
    });
}

