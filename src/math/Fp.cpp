template<lli P>
struct Fp {
    lli n;
    Fp() : n(0) {}
    Fp(lli n_) : n(((n_%P)+P)%P) {}
    inline Fp operator+(Fp other) const { return Fp(n + other.n); }
    inline Fp operator-(Fp other) const { return Fp(n - other.n); }
    inline Fp operator*(Fp other) const { return Fp(n * other.n); }
    inline Fp operator/(Fp other) const { return (*this) * other.pow(P-2); };
    Fp pow(lli b) const {
        if(b == 0) return Fp(1);
        Fp rec = pow(b/2);
        return b%2 == 0 ? rec*rec : (*this)*rec*rec;
    };
};

