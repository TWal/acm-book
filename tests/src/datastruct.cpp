#include <precontest/template.cpp>
#include <datastruct/unionfind.cpp>
#include <datastruct/fenwick.cpp>
#include <datastruct/segtree.cpp>
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

struct DummyBIT {
    vi v;
    DummyBIT(lli n) : v(n) {}
    lli get(lli k) {
        lli r = 0;
        FOR(i, k+1) {
            r += v[i];
        }
        return r;
    }
    void add(lli k, lli val) {
        v[k] += val;
    }
};

struct DummySegTree {
    vi v;
    DummySegTree(const vi& vals) : v(vals) {}
    void modify(lli p, lli val) {
        v[p] = val;
    }
    lli query(lli l, lli r) {
        lli res = 0;
        for(lli i = l; i < r; ++i) {
            res += v[i];
        }
        return res;
    }
};

void testDatastruct() {
    rc::check("unionfind: union-find works", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 1);
        UF uf = UF(size);
        DummyUF duf = DummyUF(size);

        auto piigen = rc::gen::tuple(rc::gen::inRange(0, size), rc::gen::inRange(0, size));
        vii commands = *rc::gen::container<vii>(piigen).as("commands");
        vb isFind = *rc::gen::container<vb>(commands.size(), rc::gen::arbitrary<bool>()).as("isFind");

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

    rc::check("fenwick: fenwick tree works", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 1);
        BIT bit = BIT(size);
        DummyBIT dbit = DummyBIT(size);

        auto piigen = rc::gen::tuple(rc::gen::inRange(0, size), rc::gen::arbitrary<lli>());
        vii commands = *rc::gen::container<vii>(piigen).as("commands");
        vb isGet = *rc::gen::container<vb>(commands.size(), rc::gen::arbitrary<bool>()).as("isGet");

        FOR(i, commands.size()) {
            lli k, v;
            tie(k, v) = commands[i];
            if(isGet[i]) {
                RC_ASSERT(bit.get(k) == dbit.get(k));
            } else {
                bit.add(k, v);
                dbit.add(k, v);
            }
        }
    });

    rc::check("segtree: segment tree works", []() {
        vi vals = *rc::gen::arbitrary<vi>().as("vals");
        SegTree st = SegTree(vals);
        DummySegTree dst = DummySegTree(vals);

        vii lr = *rc::gen::container<vii>(rc::gen::tuple(rc::gen::inRange(0, (int)vals.size()), rc::gen::inRange(0, (int)vals.size()))).as("lr");
        vii updates = *rc::gen::container<vii>(lr.size(), rc::gen::tuple(rc::gen::inRange(0, (int)vals.size()), rc::gen::arbitrary<lli>())).as("updates");
        for(pii& t : lr) {
            if(X(t) > Y(t)) {
                t = mt(Y(t), X(t));
            }
        }

        FOR(i, lr.size()) {
            lli l, r, idx, v;
            tie(l, r) = lr[i];
            tie(idx, v) = updates[i];
            st.modify(idx, v);
            dst.modify(idx, v);
            RC_ASSERT(st.query(l, r) == dst.query(l, r));
        }
    });
}
