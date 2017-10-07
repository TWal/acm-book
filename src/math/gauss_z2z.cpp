template<lli N>
struct GaussZ2Z {
    bitset<N> A[N]; // upper diagonal matrix (free family)
    bitset<N> B[N]; // how elements of A are generated
    lli id;
    vi V; //indices of the free vectors

    GaussZ2Z() { reset(); }

    void reset() {
        id = 0;
        FOR(i, N) A[i].reset();
        FOR(i, N) B[i].reset();
        V.clear();
    }

    lli solve__(bitset<N>& x, bitset<N>& y) {
        y.reset();
        FOR(i, N) if(x[i]) {
            if(A[i][i]) {
                x ^= A[i];
                y ^= B[i];
            } else {
                return i;
            }
        }
        return N;
    }

    //Solve the linear system, give the linear combination in y
    //If m[i] is the i'th vector given in `add`, then x = sum_i y[i]*m[V[i]]
    bool solve(bitset<N> x, bitset<N>& y) { //You want to pass `x` by copy.
        return solve__(x, y) == N;
    }

    //Add a vector in the free family. Return true if this vector is not a
    //linear combination of the previous vectors
    bool add(bitset<N> x) { //You want to pass `x` by copy.
        if(V.size() == N) return false;
        bitset<N> y;
        lli k = solve__(x, y);
        if(k < N) {
            A[k] = x;
            y.flip(V.size());
            B[k] = y;
            V.pb(id);
        }
        ++id;
        return k < N;
    }
};
