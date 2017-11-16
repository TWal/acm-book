template<typename T>
struct ImplicitTreap {
    using IT = ImplicitTreap<T>;
    lli h, sz;
    T val, sum, delta;
    bool rev;
    IT *l, *r;

    ImplicitTreap(T v) : h(rand()), sz(1), val(v), sum(v), delta(), rev(false),
                         l(nullptr), r(nullptr) {}
    ~ImplicitTreap() { delete l; delete r; }

    static lli size(IT* t) { return t ? t->sz : 0; }

    void update() {
        sz = 1 + size(l) + size(r);
        sum = val + (l ? l->sum : T()) + (r ? r->sum : T());
    }

    void push() {
        if(rev) {
            rev = false;
            swap(l, r);
            if(l) l->rev = !l->rev;
            if(r) r->rev = !r->rev;
        }
        if(delta != T()) {
            if(l) l->_add(delta);
            if(r) r->_add(delta);
            delta = T();
        }
    }

    void _add(T d) { val = val + d, delta = delta + d, sum = sum + d * sz; }

    static IT* merge(IT* a, IT* b) {
        if(!a || !b) return a ? a : b;
        a->push(); b->push();
        if(a->h < b->h) {
            a->r = merge(a->r, b);
            a->update();
            return a;
        } else {
            b->l = merge(a, b->l);
            b->update();
            return b;
        }
    }

    static tuple<IT*, IT*> split(IT* t, lli k, lli add = 0) {
        if(!t) return mt(nullptr, nullptr);
        t->push();
        lli curKey = add + size(t->l);
        IT* a;
        if(k <= curKey) {
            tie(a, t->l) = split(t->l, k, add);
            t->update();
            return mt(a, t);
        } else {
            tie(t->r, a) = split(t->r, k, curKey+1);
            t->update();
            return mt(t, a);
        }
    }

    static tuple<IT*, IT*, IT*> doubleSplit(IT* t, lli l, lli r) {
        IT *a, *b, *c, *d;
        tie(a, b) = split(t, r);
        tie(c, d) = split(a, l);
        return mt(c, d, b);
    }

    T& kth(lli k) {
        push();
        lli curKey = size(l);
        if(k == curKey) return val;
        if(k < curKey) return l->kth(k);
        else return r->kth(k-curKey-1);
    }
};

template<typename T>
struct LogVector {
    using IT = ImplicitTreap<T>;
    IT* tr;

    LogVector() : tr(nullptr) {}

    T& operator[](lli k) { return tr->kth(k); }

    lli size() { return IT::size(tr); }

    //same as v.insert(v.begin()+k, v);
    void insert(lli k, const T& v) {
        IT *a, *b;
        tie(a, b) = IT::split(tr, k);
        tr = IT::merge(a, IT::merge(new IT(v), b));
    }

    //same as v.erase(v.begin()+l, v.begin()+r);
    void erase(lli l, lli r) {
        IT *a, *b, *c;
        tie(a, b, c) = IT::doubleSplit(tr, l, r);
        delete b;
        tr = IT::merge(a, c);
    }

    //same as reverse(v.begin()+l, v.begin()+r)
    void reverse(lli l, lli r) {
        IT *a, *b, *c;
        tie(a, b, c) = IT::doubleSplit(tr, l, r);
        if(b) b->rev = !b->rev;
        tr = IT::merge(a, IT::merge(b, c));
    }

    //same as FORU(i, l, r) v[i] += v
    void add(lli l, lli r, const T& v) {
        IT *a, *b, *c;
        tie(a, b, c) = IT::doubleSplit(tr, l, r);
        if(b) b->_add(v);
        tr = IT::merge(a, IT::merge(b, c));
    }

    //same as FORU(i, l, r) sum += v[i]
    T sum(lli l, lli r) {
        IT *a, *b, *c;
        tie(a, b, c) = IT::doubleSplit(tr, l, r);
        T res = b ? b->sum : T();
        tr = IT::merge(a, IT::merge(b, c));
        return res;
    }
};
