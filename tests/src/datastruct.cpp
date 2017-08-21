#include <precontest/template.cpp>
#include <datastruct/unionfind.cpp>
#include <datastruct/fenwick.cpp>
#include <datastruct/fenwick_range.cpp>
#include <datastruct/segtree.cpp>
#include <datastruct/lazysegtree.cpp>
#include <rapidcheck.h>
#include <rapidcheck/state.h>

/*   _   _       _                    __ _           _
    | | | |_ __ (_) ___  _ __        / _(_)_ __   __| |
    | | | | '_ \| |/ _ \| '_ \ _____| |_| | '_ \ / _` |
    | |_| | | | | | (_) | | | |_____|  _| | | | | (_| |
     \___/|_| |_|_|\___/|_| |_|     |_| |_|_| |_|\__,_|
*/

struct DummyUF {
    vi p;
    DummyUF(lli n) : p(n) {
        FOR(i, n) p[i] = i;
    }
    lli find(lli i) const {
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

struct UFUnite : rc::state::Command<DummyUF, UF> {
    lli i, j;
    UFUnite(const DummyUF& duf) :
        i(*rc::gen::inRange((lli)0, (lli)duf.p.size())), j(*rc::gen::inRange((lli)0, (lli)duf.p.size())) {}
    void apply(DummyUF& duf) const override {
        duf.unite(i, j);
    }
    void run(const DummyUF& duf, UF& uf) const override {
        uf.unite(i, j);
    }
    void show(std::ostream& os) const override {
        os << "Unite(" << i << ", " << j << ")";
    }
};

struct UFFind : rc::state::Command<DummyUF, UF> {
    lli i, j;
    UFFind(const DummyUF& duf) :
        i(*rc::gen::inRange((lli)0, (lli)duf.p.size())), j(*rc::gen::inRange((lli)0, (lli)duf.p.size())) {}
    void run(const DummyUF& duf, UF& uf) const override {
        RC_ASSERT((duf.find(i) == duf.find(j)) == (uf.find(i) == uf.find(j)));
    }
    void show(std::ostream& os) const override {
        os << "Find(" << i << ", " << j << ")";
    }
};


static void testUF() {
    rc::check("unionfind", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 1);
        UF uf = UF(size);
        DummyUF duf = DummyUF(size);
        rc::state::check(duf, uf, rc::state::gen::execOneOfWithArgs<UFUnite, UFFind>());
    });
}

/*   _____                  _      _      _____
    |  ___|__ _ ____      _(_) ___| | __ |_   _| __ ___  ___
    | |_ / _ \ '_ \ \ /\ / / |/ __| |/ /   | || '__/ _ \/ _ \
    |  _|  __/ | | \ V  V /| | (__|   <    | || | |  __/  __/
    |_|  \___|_| |_|\_/\_/ |_|\___|_|\_\   |_||_|  \___|\___|
*/

struct DummyBIT {
    vi v;
    DummyBIT(lli n) : v(n) {}
    lli get(lli k) const {
        lli r = 0;
        FOR(i, k+1) r += v[i];
        return r;
    }
    void add(lli k, lli val) {
        v[k] += val;
    }
    void addRange(lli k, lli val) {
        FORU(i, k, (lli)v.size()) v[i] += val;
    }
};

template<class SUT>
struct BITAdd : rc::state::Command<DummyBIT, SUT> {
    lli k, val;
    BITAdd(const DummyBIT& dbit) :
        k(*rc::gen::inRange((lli)0, (lli)dbit.v.size())), val(*rc::gen::arbitrary<lli>()) {}
    void apply(DummyBIT& dbit) const override {
        dbit.add(k, val);
    }
    void run(const DummyBIT& dbit, SUT& bit) const override {
        bit.add(k, val);
    }
    void show(std::ostream& os) const override {
        os << "Add(" << k << ", " << val << ")";
    }
};

template<class SUT>
struct BITGet : rc::state::Command<DummyBIT, SUT> {
    lli k;
    BITGet(const DummyBIT& dbit) :
        k(*rc::gen::inRange((lli)0, (lli)dbit.v.size())) {}
    void run(const DummyBIT& dbit, SUT& bit) const override {
        RC_ASSERT(dbit.get(k) == bit.get(k));
    }
    void show(std::ostream& os) const override {
        os << "Get(" << k << ")";
    }
};

template<class SUT>
struct BITAddRange: rc::state::Command<DummyBIT, SUT> {
    lli k, val;
    BITAddRange(const DummyBIT& dbit) :
        k(*rc::gen::inRange((lli)0, (lli)dbit.v.size())), val(*rc::gen::arbitrary<lli>()) {}
    void apply(DummyBIT& dbit) const override {
        dbit.addRange(k, val);
    }
    void run(const DummyBIT& dbit, SUT& bit) const override {
        bit.addRange(k, val);
    }
    void show(std::ostream& os) const override {
        os << "AddRange(" << k << ", " << val << ")";
    }
};

static void testFenwick() {
    rc::check("fenwick", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 1);
        BIT bit = BIT(size);
        DummyBIT dbit = DummyBIT(size);
        rc::state::check(dbit, bit, rc::state::gen::execOneOfWithArgs<BITAdd<BIT>, BITGet<BIT>>());
    });

    rc::check("fenwick_range", []() {
        const int size = *rc::gen::withSize([](int n) { return rc::gen::just(n); });
        RC_PRE(size >= 1);
        BITRange bit = BITRange(size);
        DummyBIT dbit = DummyBIT(size);
        rc::state::check(dbit, bit, rc::state::gen::execOneOfWithArgs<BITAdd<BITRange>, BITAddRange<BITRange>, BITGet<BITRange>>());
    });
}


/*   ____                                  _     _____
    / ___|  ___  __ _ _ __ ___   ___ _ __ | |_  |_   _| __ ___  ___
    \___ \ / _ \/ _` | '_ ` _ \ / _ \ '_ \| __|   | || '__/ _ \/ _ \
     ___) |  __/ (_| | | | | | |  __/ | | | |_    | || | |  __/  __/
    |____/ \___|\__, |_| |_| |_|\___|_| |_|\__|   |_||_|  \___|\___|
                |___/
*/

struct NonCommMonoid {
    vi v_;
    NonCommMonoid() : v_(vi()) {}
    NonCommMonoid(const vi& v) : v_(v) {}
    NonCommMonoid operator+(const NonCommMonoid& rhs) const {
        vi res = v_;
        for(lli i : rhs.v_) res.push_back(i);
        return res;
    }

    bool operator==(const NonCommMonoid& rhs) const {
        return v_ == rhs.v_;
    }
};

ostream& operator<<(ostream& os, const NonCommMonoid& a) {
    os << "[";
    FOR(i, a.v_.size()) {
        os << a.v_[i];
        if(i+1 != (lli)a.v_.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

namespace rc {

template<>
struct Arbitrary<NonCommMonoid> {
    static Gen<NonCommMonoid> arbitrary() {
        return gen::build<NonCommMonoid>(
            gen::set(&NonCommMonoid::v_, rc::gen::arbitrary<vi>())
        );
    }
};

} // namespace rc


template<class T>
struct DummySegTree {
    vector<T> v;
    DummySegTree(const vector<T>& vals) : v(vals) {}
    void modify(lli p, T val) {
        v[p] = val;
    }
    void inc(lli p, T value) {
        v[p] = v[p] + value;
    }
    void incRange(lli l, lli r, T value) {
        FORU(i, l, r) v[i] = v[i] + value;
    }
    T query(lli l, lli r) const {
        T res;
        FORU(i, l, r) res = res + v[i];
        return res;
    }
};

template<class T, class SUT>
struct SegTreeModify: rc::state::Command<DummySegTree<T>, SUT> {
    lli p;
    T val;
    SegTreeModify(const DummySegTree<T>& dseg) :
        p(*rc::gen::inRange((lli)0, (lli)dseg.v.size())), val(*rc::gen::arbitrary<T>()) {}
    void apply(DummySegTree<T>& dseg) const override {
        dseg.modify(p, val);
    }
    void run(const DummySegTree<T>& dseg, SUT& seg) const override {
        seg.modify(p, val);
    }
    void show(std::ostream& os) const override {
        os << "Modify(" << p << ", " << val << ")";
    }
};


template<class T, class SUT>
struct SegTreeInc: rc::state::Command<DummySegTree<T>, SUT> {
    lli p;
    T val;
    SegTreeInc(const DummySegTree<T>& dseg) :
        p(*rc::gen::inRange((lli)0, (lli)dseg.v.size())), val(*rc::gen::arbitrary<T>()) {}
    void apply(DummySegTree<T>& dseg) const override {
        dseg.inc(p, val);
    }
    void run(const DummySegTree<T>& dseg, SUT& seg) const override {
        seg.inc(p, val);
    }
    void show(std::ostream& os) const override {
        os << "Inc(" << p << ", " << val << ")";
    }
};

template<class T, class SUT>
struct SegTreeQuery: rc::state::Command<DummySegTree<T>, SUT> {
    lli l, r;
    SegTreeQuery(const DummySegTree<T>& dseg) :
        l(*rc::gen::inRange((lli)0, (lli)dseg.v.size())), r(*rc::gen::inRange((lli)l, (lli)dseg.v.size())){}

    void run(const DummySegTree<T>& dseg, SUT& seg) const override {
        RC_ASSERT(dseg.query(l, r) == seg.query(l, r));
    }
    void show(std::ostream& os) const override {
        os << "Query(" << l << ", " << r << ")";
    }
};

static void testSegTree() {
    rc::check("segtree", []() {
        vvi vals = *rc::gen::arbitrary<vvi>().as("vals");

        vector<NonCommMonoid> mvals;
        for(vi v : vals) mvals.pb(v);
        SegTree<NonCommMonoid> st = SegTree<NonCommMonoid>(mvals);
        DummySegTree<NonCommMonoid> dst = DummySegTree<NonCommMonoid>(mvals);

        rc::state::check(dst, st, rc::state::gen::execOneOfWithArgs<
                SegTreeModify<NonCommMonoid, SegTree<NonCommMonoid>>,
                SegTreeInc<NonCommMonoid, SegTree<NonCommMonoid>>,
                SegTreeQuery<NonCommMonoid, SegTree<NonCommMonoid>>
        >());
    });
}

void testDatastruct() {
    testUF();
    testFenwick();
    testSegTree();
}

