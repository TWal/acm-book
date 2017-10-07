template<class T>
bool isZero(T x) { assert(false); }

//adapt this function for your favorite field!
template<>
bool isZero<double>(double x) {
    return fabs(x) < 1e-6;
}

template<class T>
struct Gauss {
    using vt  = vector<T>;
    using vvt = vector<vt>;

    lli N, id;
    //A = upper diagonal matrix (free family), B = how elements of A are generated
    vvt A, B;
    vi V; //indices of the free vectors

    Gauss(lli N) { reset(N); }

    void reset(int N_) {
        N = N_, id = 0;
        A.assign(N, vt(N, 0));
        B.assign(N, vt(N, 0));
        V.clear();
    }

    lli solve__(vt& x, vt& y) {
        assert(x.size() == N);
        y.assign(N, 0);
        FOR(i, N) if(!isZero(x[i])) {
            if(!isZero(A[i][i])) {
                T c = x[i] / A[i][i];
                FOR(j, N) x[j] = x[j] - A[i][j]*c;
                FOR(j, N) y[j] = y[j] + B[i][j]*c;
            } else {
                return i;
            }
        }
        return N;
    }

    //Solve the linear system, give the linear combination in y
    //If m[i] is the i'th vector given in `add`, then x = sum_i y[i]*m[V[i]]
    bool solve(vt x, vt& y) { //You want to pass `x` by copy. Don't try to optimize
        return solve__(x, y) == N;
    }

    //Add a vector in the free family. Return true if this vector is not a
    //linear combination of the previous vectors
    bool add(vt x) { //You want to pass `x` by copy. Don't try to optimize
        if(V.size() == N) return false;
        vt y;
        lli k = solve__(x, y);
        if(k < N) {
            A[k] = x;
            FOR(j, N) B[k][j] = T(j == V.size()) - y[j];
            V.pb(id);
        }
        ++id;
        return k < N;
    }
};
