template<class Ord, class Tr>
struct Treap {
    Tr *l, *r;
    Ord s; //for the BST
    lli h; //for the heap
    lli size;
    Treap()     : l(0), r(0), s(),  h(rand()), size(0) {}
    Treap(Ord s_) : l(0), r(0), s(s_), h(rand()), size(1) {}
    virtual void update() {
        size = 1;
        if(l) size += l->size;
        if(r) size += r->size;
    }
    virtual void push() {}

    //internal method, precondition: minimum `s` in b >= maximum `s` in a
    static Tr* merge(Tr* a, Tr* b) {
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

    tuple<Tr*, Tr*> split(Ord k) {
        push();
        if(k <= s) {
            auto p = (l ? l->split(k) : mt(nullptr, nullptr));
            l = Y(p);
            update();
            return mt(X(p), (Tr*)this);
        } else {
            auto p = (r ? r->split(k) : mt(nullptr, nullptr));
            r = X(p);
            update();
            return mt((Tr*)this, Y(p));
        }
    }

    Tr* insert(Ord k) {
        auto p = split(k);
        return merge(X(p), merge(new Tr(k), Y(p)));
    }
};

