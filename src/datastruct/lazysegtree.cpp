int log2(int x){ return 31 - __builtin_clz(x); }

//Works with any commutative monoid
//Must implement: T() -> zero; T operator+(const T&), T operator*(lli) (exp)
template<class T>
struct LazySegTree {
    lli n;
    vector <T> s, d; //sum, delta
    LazySegTree(vector<T>& v) : n(1LL<<((lli)log2(v.size())+1)), s(2*n, T()), d(s) {
        copy(v.begin(), v.end(), s.begin()+n);
        for(lli i = n-1; i > 0; --i) s[i] = s[2*i] + s[2*i+1];
    }

    void push(lli i, lli a, lli b) {
        if(i < n && !(d[i] == T())){
            lli c = (a+b)/2;
            add__(2*i  , a  , c, d[i]);
            add__(2*i+1, c+1, b, d[i]);
            d[i] = T();
        }
    }

    void add__(lli i, lli a, lli b, T v){
        d[i] = d[i] + v;
        s[i] = s[i] + v*(b-a+1);
    }
    void add_(lli i, lli a, lli b, lli l, lli r, T v){
        if(l > b || r < a) return;
        if(l <= a && b <= r) return add__(i, a, b, v);
        push(i, a, b);
        lli c = (a+b)/2;
        add_(2*i  , a  , c, l, r, v);
        add_(2*i+1, c+1, b, l, r, v);
        s[i] = s[2*i] + s[2*i+1];
    }
    // v[l:r] += value
    void add(lli l, lli r, T v){
        add_(1, 0, n-1, l, r-1, v);
    }

    T query_(lli i, lli a, lli b, lli l, lli r){
        if(l > b || r < a) return T();
        push(i, a, b);
        if(l <= a && b <= r) return s[i];
        lli c = (a+b)/2;
        return query_(2*i, a, c, l, r) + query_(2*i+1, c+1, b, l, r);
    }
    // sum(v[l:r])
    T query(lli l, lli r){
        return query_(1, 0, n-1, l, r-1);
    }
};
