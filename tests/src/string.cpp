#include <precontest/template.cpp>
#include <string/kmp.cpp>
#include <string/zfunc.cpp>
#include <string/ahocorasick.cpp>
#include <string/manacher.cpp>

const int MAX_TRA = 26;
int normalize(char c) { //normalize `c` in [0, ..., MAX_TRA-1]
    return c - 'a';
}
#include <string/suffixautomaton.cpp>
#include <string/suffixarray.cpp>
#include <string/suffixarray_linear.cpp>
#include <string/palindromic_tree.cpp>

#include <datastruct/unionfind.cpp>
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

static bool isPalindrome(const string& s) {
    string t = s;
    reverse(t.begin(), t.end());
    return t == s;
}

static vector<string> allPalindromes(const string& s) {
    lli n = s.size();
    vector<string> res;
    FOR(i, n) {
        FORU(j, i+1, n+1) {
            string t = s.substr(i, j-i);
            if(isPalindrome(t)) {
                res.pb(t);
            }
        }
    }
    return res;
}

static vector<string> getNodeRepresentation(const PalindromicTree& pt) {
    vector<string> res(pt.t.size());

    FOR(i, pt.t.size()) {
        FOR(j, 26) {
            char c = 'a'+j;
            lli cur = pt.t[i].next[j];
            if(cur != -1) {
                if(i == 0) {
                    res[cur] = c;
                } else {
                    res[cur] = c + res[i] + c;
                }
            }
        }
    }

    return res;
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

    rc::check("suffixautomaton: is connected and acyclic", []() {
        string s = *getString().as("str");
        SuffixAutomaton sa;
        sa.reserve(s.size());
        for(char c : s) sa.add(c);
        struct Dfs {
            SuffixAutomaton& sa;
            vi state;
            Dfs(SuffixAutomaton& sa_) : sa(sa_), state(sa.aut.size(), 0) {}
            void dfs(lli node) {
                RC_ASSERT(state[node] != 1);
                if(state[node] == 2) return;
                state[node] = 1;
                FOR(i, MAX_TRA) {
                    if(sa.aut[node][i] >= 0) {
                        dfs(sa.aut[node][i]);
                    }
                }
                state[node] = 2;
            }
        };
        Dfs dfs(sa);
        dfs.dfs(0);
        FOR(i, dfs.state.size()) {
            RC_ASSERT(dfs.state[i] == 2);
        }
    });

    rc::check("suffixautomaton: match suffixes and only them", []() {
        string s = *getString().as("str");
        SuffixAutomaton sa;
        sa.reserve(s.size());
        for(char c : s) sa.add(c);
        sa.computeFinals();
        struct Dfs {
            SuffixAutomaton& sa;
            vector<vector<string>> match;
            Dfs(SuffixAutomaton& sa_) : sa(sa_), match(sa.aut.size()) {}
            void dfs(lli node, const string& s) {
                match[node].pb(s);
                FOR(i, MAX_TRA) {
                    if(sa.aut[node][i] >= 0) {
                        dfs(sa.aut[node][i], s + (char)('a' + i));
                    }
                }
            }
        };
        Dfs dfs(sa);
        dfs.dfs(0, "");

        vector<string> suffaut;
        FOR(i, sa.aut.size()) {
            if(sa.isFinal[i]) {
                suffaut.insert(suffaut.end(), dfs.match[i].begin(), dfs.match[i].end());
            }
        }
        vector<string> suff;
        FOR(i, s.size()+1) {
            suff.pb(s.substr(s.size()-i, i));
        }
        sort(suffaut.begin(), suffaut.end());
        sort(suff.begin(), suff.end());
        RC_ASSERT(suffaut == suff);
    });

    rc::check("suffixautomaton: size is linear", []() {
        string s = *getString().as("str");
        RC_PRE(s.size() >= 3);
        SuffixAutomaton sa;
        sa.reserve(s.size());
        for(char c : s) sa.add(c);
        size_t N = s.size();
        RC_ASSERT(sa.aut.size() <= 2*N);
        RC_ASSERT(sa.link.size() <= 2*N);
        RC_ASSERT(sa.len.size() <= 2*N);
    });


    rc::check("suffixautomaton: automaton is minimal", []() {
        string s = *getString().as("str");
        SuffixAutomaton sa;
        sa.reserve(s.size());
        for(char c : s) sa.add(c);
        sa.computeFinals();
        size_t N = sa.aut.size();
        //complete the automaton with a "dead-end" state
        FOR(i, sa.aut.size()) {
            FOR(j, MAX_TRA) {
                if(sa.aut[i][j] == -1) {
                    sa.aut[i][j] = N;
                }
            }
        }
        sa.aut.pb(array<int, MAX_TRA>());
        FOR(j, MAX_TRA) {
            sa.aut[N][j] = N;
        }
        sa.isFinal.pb(false);
        N += 1;
        //use Moore's algorithm
        UF uf(N);
        lli fstate = -1, nfstate = -1;
        FOR(i, N) {
            if(sa.isFinal[i]) {
                if(fstate >= 0) uf.unite(fstate, i);
                else fstate = i;
            } else {
                if(nfstate >= 0) uf.unite(nfstate, i);
                else nfstate = i;
            }
        }
        FOR(i, N) {
            UF curuf(N);
            bool cont = false;
            FOR(i, N) {
                if(uf.find(i) != i) {
                    cont = true;
                    break;
                }
            }
            if(!cont) break;
            FOR(q, N) {
                FOR(qp, N) {
                    if(uf.find(q) == uf.find(qp)) {
                        bool ok = true;
                        FOR(j, MAX_TRA) {
                            if(uf.find(sa.aut[q][j]) != uf.find(sa.aut[qp][j])) {
                                ok = false;
                                break;
                            }
                        }
                        if(ok) {
                            curuf.unite(q, qp);
                        }
                    }
                }
            }
            uf = curuf;
        }
        FOR(i, N) {
            RC_ASSERT(uf.find(i) == i);
        }
    });

    rc::check("suffixarray: sa is the suffix array", []() {
        string s = *getString().as("str");
        RC_PRE(s.size() >= 1);
        vector<string> suf;
        FOR(i, s.size()) {
            suf.pb(s.substr(s.size()-i-1, i+1));
        }
        sort(suf.begin(), suf.end());
        SuffixArray sa(s);
        FOR(i, s.size()) {
            RC_ASSERT(sa.sa[i] == s.size()-suf[i].size());
        }
    });

    rc::check("suffixarray: pos is inverse of sa", []() {
        string s = *getString().as("str");
        RC_PRE(s.size() >= 1);
        SuffixArray sa(s);
        FOR(i, s.size()) {
            RC_ASSERT(sa.pos[sa.sa[i]] == i);
        }
    });

    rc::check("suffixarray: lcp is the longest common prefix", []() {
        string s = *getString().as("str");
        RC_PRE(s.size() >= 1);
        SuffixArray sa(s);
        FOR(i, s.size()-1) {
            lli lcp = 0;
            while(s[sa.sa[i]+lcp] == s[sa.sa[i+1]+lcp]) lcp += 1;
            RC_ASSERT(lcp == sa.lcp[i]);
        }
    });

    rc::check("suffixarray_linear: same as suffixarray", []() {
        string s = *getString().as("str");
        RC_PRE(s.size() >= 1);
        SuffixArray sa(s);
        SuffixArrayLinear sa_lin(s);
        RC_ASSERT(sa.sa == sa_lin.sa);
        RC_ASSERT(sa.pos == sa_lin.pos);
        RC_ASSERT(sa.lcp == sa_lin.lcp);
    });

    rc::check("manacher", []() {
        string s = *getString().as("s");
        lli n = s.size();
        RC_PRE(n >= 1);
        lli m = 2*n+1;
        vector<char> t(m, -1);
        FOR(i, n) t[2*i+1] = s[i];
        vi rad(m, 0);
        FOR(i, m) {
            lli& r = rad[i];
            while(i-r-1 >= 0 && i+r+1 < m && t[i-r-1] == t[i+r+1]) ++r;
        }
        rad.pop_back();
        rad.erase(rad.begin());
        for(lli& r : rad) r /= 2;
        RC_ASSERT(manacher(s) == rad);
    });

    rc::check("palindromic_tree: check add", []() {
        string s = *getString().as("s");

        string curs;
        PalindromicTree pt;
        for(char c : s) {
            vector<string> oldpals = allPalindromes(curs);
            bool b = pt.add(c);
            curs.pb(c);
            vector<string> newpals = allPalindromes(curs);
            set<string> soldpals(oldpals.begin(), oldpals.end());
            set<string> snewpals(newpals.begin(), newpals.end());
            RC_ASSERT(b == (soldpals != snewpals));
        }
    });

    rc::check("palindromic_tree: check that pt contains palindromes of s and only them", []() {
        string s = *getString().as("s");

        vector<string> pals = allPalindromes(s);

        PalindromicTree pt;
        for(char c : s) pt.add(c);
        vector<string> ptpals = getNodeRepresentation(pt);

        //check that pt contains palindromes of s and only them
        set<string> sptpals(ptpals.begin()+2, ptpals.end());
        set<string> spals(pals.begin(), pals.end());
        RC_ASSERT(sptpals == spals);
    });

    rc::check("palindromic_tree: check link", []() {
        string s = *getString().as("s");

        vector<string> pals = allPalindromes(s);

        PalindromicTree pt;
        for(char c : s) pt.add(c);
        vector<string> ptpals = getNodeRepresentation(pt);

        FOR(i, pt.t.size()) {
            lli suf = pt.t[i].link;
            if(suf == 0) {
                RC_ASSERT(i == 0 || i == 1);
                continue;
            }
            lli m = ptpals[i].size();
            string cur;
            FORU(j, 1, m) {
                cur = ptpals[i].substr(j, string::npos);
                if(isPalindrome(cur)) break;
            }
            RC_ASSERT(ptpals[suf] == cur);
        }
    });

    rc::check("palindromic_tree: check sufCount", []() {
        string s = *getString().as("s");

        vector<string> pals = allPalindromes(s);

        PalindromicTree pt;
        for(char c : s) pt.add(c);
        vector<string> ptpals = getNodeRepresentation(pt);

        FOR(i, pt.t.size()) {
            if(i == 0) continue;
            lli count = 0;
            FOR(j, ptpals[i].size()) {
                if(isPalindrome(ptpals[i].substr(j, string::npos))) ++count;
            }
            RC_ASSERT(pt.t[i].sufCount == count);
        }
    });

    rc::check("palindromic_tree: check occ", []() {
        string s = *getString().as("s");

        vector<string> pals = allPalindromes(s);

        PalindromicTree pt;
        for(char c : s) pt.add(c);
        vector<string> ptpals = getNodeRepresentation(pt);

        multiset<string> spals(pals.begin(), pals.end());
        vi occ = pt.computeOcc();
        multiset<string> sptpals;
        FOR(i, pt.t.size()) {
            if(i <= 1) continue;
            FOR(j, occ[i]) {
                sptpals.insert(ptpals[i]);
            }
        }
        RC_ASSERT(sptpals == spals);
    });
}
