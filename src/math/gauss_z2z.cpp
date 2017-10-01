template<lli N>
struct GaussZ2Z {
    bitset<N> A[N]; // upper diagonal matrix (free family)
    bitset<N> B[N]; // how elements of A are generated
    lli id;

    GaussZ2Z() { reset(); }

    void reset() {
        id = 0;
        FOR(i, N) A[i].reset();
        FOR(i, N) B[i].reset();
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
    bool solve(bitset<N> x, bitset<N>& y) { //You want to pass `x` by copy.
        return solve__(x, y) == N;
    }

    //Add a vector in the free family. Return true if this vector is not a
    //linear combination of the previous vectors
    bool add(bitset<N> x) { //You want to pass `x` by copy.
        assert(id < N);
        bitset<N> y;
        lli k = solve__(x, y);
        if(k < N) {
            A[k] = x;
            y.flip(id);
            B[k] = y;
        }
        ++id;
        return k < N;
    }
};
