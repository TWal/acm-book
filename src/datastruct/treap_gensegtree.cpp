template<class Ord, class T>
struct GenSegTree : public Treap<Ord, GenSegTree<Ord, T>> {
    typedef Treap<Ord, GenSegTree<Ord, T>> M;
    using M::l; using M::r; using M::s; using M::size;
    T value, sum, delta;
    Ord mini, maxi;
    GenSegTree(Ord s_) : M(s_), mini(s_), maxi(s_) {}
    GenSegTree(Ord s_, T v) : M(s_), value(v), sum(v), mini(s_), maxi(s_) {}

    virtual void update() {
        M::update();
        sum = value, mini = maxi = s;
        if(l) sum = l->sum + sum, mini = min(mini, l->mini);
        if(r) sum = sum + r->sum, maxi = max(maxi, r->maxi);
    }

    void __add(T d) {
        value = value + d, delta = delta + d, sum = sum + d * size;
    }

    virtual void push() {
        if(l) l->__add(delta);
        if(r) r->__add(delta);
        delta = T();
    }

    //add v to the value of the elements in [left, right]
    void add(Ord left, Ord right, T v) {
        if(left > maxi || right < mini) return;
        if(left <= mini && maxi <= right) return __add(v);
        if(left <= s && s <= right) value = value + v;
        push();
        if(l) l->add(left, right, v);
        if(r) r->add(left, right, v);
        update();
    }

    //sum the value of the element in [left, right]
    T query(Ord left, Ord right) {
        if(left > maxi || right < mini) return T();
        push();
        if(left <= mini && maxi <= right) return sum;
        T res = (left <= s && s <= right ? value : T());
        if(l) res = res + l->query(left, right);
        if(r) res = res + r->query(left, right);
        return res;
    }

    //Add the element `k` in the set, with value `val`
    GenSegTree* insert(Ord k, T val) {
        auto p = M::split(k);
        return M::merge(X(p), M::merge(new GenSegTree(k, val), Y(p)));
    }
};
