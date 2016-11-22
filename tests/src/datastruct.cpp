#include <precontest/template.cpp>
#include <datastruct/unionfind.cpp>
#include <rapidcheck.h>

//Slow union-find without any optimization so it works
struct DummyUF {
    vi p;
    DummyUF(lli n) : p(n) {
        FOR(i, n) p[i] = i;
    }
    lli find(lli i) {
        return p[i];
    }
    void unite(lli i, lli j) {
        lli iparent = p[i];
        FOR(k, p.size()) {
            if(p[k] == iparent) {
                p[k] = p[j];
            }
        }
    }
};

void testDatastruct() {
    rc::check("union-find works", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 1);
        UF uf = UF(size);
        DummyUF duf = DummyUF(size);
        //This is where haskell's do-notation is missing
        //do 
        //    a <- inRange(0, size)
        //    b <- inRange(0, size)
        //    return mt(a, b)
        auto piigen = rc::gen::mapcat(rc::gen::inRange(0, size), [&](lli a) {
            return rc::gen::mapcat(rc::gen::inRange(0, size), [&](lli b) {
                return rc::gen::just(mt(a, b));
            });
        });
        vii commands = *rc::gen::container<vii>(piigen);
        vb isFind = *rc::gen::container<vb>(commands.size(), rc::gen::arbitrary<bool>());

        FOR(i, commands.size()) {
            lli v1, v2;
            tie(v1, v2) = commands[i];
            if(isFind[i]) {
                RC_ASSERT((uf.find(v1) == uf.find(v2)) == (duf.find(v1) == duf.find(v2)));
            } else {
                uf.unite(v1, v2);
                duf.unite(v1, v2);
            }
        }
    });
}
