#include <precontest/template.cpp>
#include <datastruct/unionfind.cpp>
#include <datastruct/fenwick.cpp>
#include <datastruct/fenwick_range.cpp>
#include <datastruct/segtree.cpp>
#include <datastruct/lazysegtree.cpp>
#include <datastruct/treap.cpp>
#include <datastruct/treap_gensegtree.cpp>
#include <datastruct/rmq.cpp>
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

struct CommMonoid {
    vi v_;
    CommMonoid() : v_() {}
    CommMonoid(const vi& v) : v_(v) {
        sort(v_.begin(), v_.end());
    }
    CommMonoid operator+(const CommMonoid& rhs) const {
        vi res;
        lli i = 0, j = 0;
        while(i < v_.size() && j < rhs.v_.size()) {
            if(v_[i] < rhs.v_[j]) res.pb(v_[i++]);
            else res.pb(rhs.v_[j++]);
        }
        while(i < v_.size()) res.pb(v_[i++]);
        while(j < rhs.v_.size()) res.pb(rhs.v_[j++]);
        return res;
    }
    CommMonoid operator*(lli m) const {
        vi res;
        for(lli i : v_) {
            FOR(_, m) {
                res.pb(i);
            }
        }
        return res;
    };
    bool operator==(const CommMonoid& rhs) const {
        return v_ == rhs.v_;
    }
};

ostream& operator<<(ostream& os, const CommMonoid& a) {
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

template<>
struct Arbitrary<CommMonoid> {
    static Gen<CommMonoid> arbitrary() {
        return gen::build<CommMonoid>(
            gen::set(&CommMonoid::v_, rc::gen::arbitrary<vi>())
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


template<class T, class SUT>
struct SegTreeAddRange: rc::state::Command<DummySegTree<T>, SUT> {
    lli l, r;
    T val;
    SegTreeAddRange(const DummySegTree<T>& dseg) :
        l(*rc::gen::inRange((lli)0, (lli)dseg.v.size())), r(*rc::gen::inRange((lli)l, (lli)dseg.v.size())), val(*rc::gen::arbitrary<T>()) {}

    void apply(DummySegTree<T>& dseg) const override {
        dseg.incRange(l, r, val);
    }
    void run(const DummySegTree<T>& dseg, SUT& seg) const override {
        seg.add(l, r, val);
    }
    void show(std::ostream& os) const override {
        os << "AddRange(" << l << ", " << r << ", " << val << ")";
    }
};

static void testSegTree() {
    rc::check("segtree", []() {
        vector<NonCommMonoid> vals = *rc::gen::arbitrary<vector<NonCommMonoid>>().as("vals");

        SegTree<NonCommMonoid> st = SegTree<NonCommMonoid>(vals);
        DummySegTree<NonCommMonoid> dst = DummySegTree<NonCommMonoid>(vals);

        rc::state::check(dst, st, rc::state::gen::execOneOfWithArgs<
                SegTreeModify<NonCommMonoid, SegTree<NonCommMonoid>>,
                SegTreeInc<NonCommMonoid, SegTree<NonCommMonoid>>,
                SegTreeQuery<NonCommMonoid, SegTree<NonCommMonoid>>
        >());
    });

    rc::check("lazysegtree", []() {
        vector<CommMonoid> vals = *rc::gen::arbitrary<vector<CommMonoid>>().as("vals");

        LazySegTree<CommMonoid> st = LazySegTree<CommMonoid>(vals);
        DummySegTree<CommMonoid> dst = DummySegTree<CommMonoid>(vals);

        rc::state::check(dst, st, rc::state::gen::execOneOfWithArgs<
                SegTreeQuery<CommMonoid, LazySegTree<CommMonoid>>,
                SegTreeAddRange<CommMonoid, LazySegTree<CommMonoid>>
        >());
    });
}



/*   _____
    |_   _| __ ___  __ _ _ __
      | || '__/ _ \/ _` | '_ \
      | || | |  __/ (_| | |_) |
      |_||_|  \___|\__,_| .__/
                        |_|
*/

struct MyTreap : public Treap<lli, MyTreap> {
    MyTreap(lli s_) : Treap<lli, MyTreap>(s_) {}
};

struct TreapInsert : rc::state::Command<lli, MyTreap*> {
    lli x;
    TreapInsert(const lli&) : x(*rc::gen::arbitrary<lli>()) {}
    void run(const lli&, MyTreap*& treap) const override {
        treap = treap->insert(x);
    }
    void show(std::ostream& os) const override {
        os << "Insert(" << x << ")";
    }
};

struct TreapCheckHeap : rc::state::Command<lli, MyTreap*> {
    TreapCheckHeap(const lli&) {}
    bool checkHeap(MyTreap* tr, lli mini) const {
        if(tr->h < mini) return false;
        bool res = true;
        if(tr->l) res = res && checkHeap(tr->l, tr->h);
        if(tr->r) res = res && checkHeap(tr->r, tr->h);
        return res;
    }
    void run(const lli&, MyTreap*& treap) const override {
        RC_ASSERT(checkHeap(treap, numeric_limits<lli>::min()));
    }
    void show(std::ostream& os) const override {
        os << "CheckHeap()";
    }
};

struct TreapCheckBst : rc::state::Command<lli, MyTreap*> {
    TreapCheckBst(const lli&) {}
    bool checkBst(MyTreap* tr, lli mini, lli maxi) const {
        if(tr->s < mini || tr->s > maxi) return false;
        bool res = true;
        if(tr->l) res = res && checkBst(tr->l, mini, tr->s);
        if(tr->r) res = res && checkBst(tr->r, tr->s, maxi);
        return res;
    }
    void run(const lli&, MyTreap*& treap) const override {
        RC_ASSERT(checkBst(treap, numeric_limits<lli>::min(), numeric_limits<lli>::max()));
    }
    void show(std::ostream& os) const override {
        os << "CheckBst()";
    }
};

struct TreapCheckSize : rc::state::Command<lli, MyTreap*> {
    TreapCheckSize(const lli&) {}
    lli checkSize(MyTreap* tr) const {
        lli size = 1;
        if(tr->l) size += checkSize(tr->l);
        if(tr->r) size += checkSize(tr->r);
        RC_ASSERT(size == tr->size);
        return size;
    }
    void run(const lli&, MyTreap*& treap) const override {
        RC_ASSERT(checkSize(treap));
    }
    void show(std::ostream& os) const override {
        os << "CheckSize()";
    }
};

template<class Ord, class T>
struct DummyGenSegTree {
    vector<tuple<Ord, T>> v;
    DummyGenSegTree(Ord s) : v(1, mt(s, T())) {}

    void insert(Ord s, T val) {
        v.pb(mt(s, val));
        sort(v.begin(), v.end());
    }
    void add(Ord left, Ord right, T val) {
        FOR(i, v.size()) {
            if(X(v[i]) >= left && X(v[i]) <= right) {
                Y(v[i]) = Y(v[i]) + val;
            }
        }
    }
    T query(Ord left, Ord right) const {
        T res = T();
        FOR(i, v.size()) {
            if(X(v[i]) >= left && X(v[i]) <= right) {
                res = res + Y(v[i]);
            }
        }
        return res;
    }
};

struct IntMonoid {
    lli v;
    IntMonoid() : v(0) {}
    IntMonoid(lli v_) : v(v_) {}
    IntMonoid operator+(IntMonoid rhs) const {
        return v + rhs.v;
    }
    bool operator==(IntMonoid rhs) const {
        return v == rhs.v;
    }
    IntMonoid operator*(lli rhs) const {
        return v*rhs;
    }
    bool operator<(IntMonoid rhs) const {
        return v < rhs.v;
    }
};

ostream& operator<<(ostream& os, const IntMonoid& a) {
    os << a.v;
    return os;
}


using GST = GenSegTree<double, IntMonoid>;
using DGST = DummyGenSegTree<double, IntMonoid>;

struct GenSegTreeInsert : rc::state::Command<DGST, GST*> {
    double s;
    IntMonoid val;
    GenSegTreeInsert(const DGST&) : s(*rc::gen::arbitrary<double>()), val(*rc::gen::arbitrary<lli>()) {}
    void apply(DGST& dseg) const override {
        dseg.insert(s, val);
    }
    void run(const DGST&, GST*& seg) const override {
        seg = seg->insert(s, val);
    }
    void show(std::ostream& os) const override {
        os << "Insert(" << s << ", " << val << ")";
    }
};

struct GenSegTreeAdd : rc::state::Command<DGST, GST*> {
    double left;
    double right;
    IntMonoid val;
    GenSegTreeAdd(const DGST&) : left(*rc::gen::arbitrary<double>()), right(*rc::gen::arbitrary<double>()), val(*rc::gen::arbitrary<lli>()) {
        if(left > right) swap(left, right);
    }
    void apply(DGST& dseg) const override {
        dseg.add(left, right, val);
    }
    void run(const DGST&, GST*& seg) const override {
        seg->add(left, right, val);
    }
    void show(std::ostream& os) const override {
        os << "Add(" << left << ", " << right << ", " << val << ")";
    }
};

struct GenSegTreeQuery : rc::state::Command<DGST, GST*> {
    double left;
    double right;
    GenSegTreeQuery(const DGST&) : left(*rc::gen::arbitrary<double>()), right(*rc::gen::arbitrary<double>()) {
        if(left > right) swap(left, right);
    }
    void run(const DGST& dseg, GST*& seg) const override {
        RC_ASSERT(dseg.query(left, right) == seg->query(left, right));
    }
    void show(std::ostream& os) const override {
        os << "Query(" << left << ", " << right << ")";
    }
};

static void testTreap() {
    rc::check("treap", []() {
        lli meh = 0;
        MyTreap* treap = new MyTreap(0);

        rc::state::check(meh, treap, rc::state::gen::execOneOfWithArgs<TreapInsert, TreapCheckHeap, TreapCheckBst, TreapCheckSize>());
    });

    rc::check("gensegtree", []() {
        GST* gst = new GST(0);
        DGST dgst(0);

        rc::state::check(dgst, gst, rc::state::gen::execOneOfWithArgs<GenSegTreeInsert, GenSegTreeAdd, GenSegTreeQuery>());
    });
}


/*  ____  __  __  ___
   |  _ \|  \/  |/ _ \
   | |_) | |\/| | | | |
   |  _ <| |  | | |_| |
   |_| \_\_|  |_|\__\_\
*/

static void testRMQ() {
    rc::check("RMQ", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        RC_PRE(v.size() >= 1);

        RMQ rmq(v);

        FOR(l, v.size()) {
            FORU(r, l+1, v.size()+1) {
                lli rmqMin = rmq.query(l, r);
                lli calcMin = v[l];
                FORU(i, l, r) {
                    calcMin = min(calcMin, v[i]);
                }
                RC_ASSERT(rmqMin == calcMin);
            }
        }
    });
}

void testDatastruct() {
    testUF();
    testFenwick();
    testSegTree();
    testTreap();
    testRMQ();
}

