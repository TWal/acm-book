
using T = double;
const T inf = 1e100, eps = 1e-10, pi = acos(-1);

struct Pt {
    T x, y;

    Pt() {}
    Pt(T x, T y) : x(x), y(y) {}

    Pt operator + (Pt p) const { return Pt(x + p.x, y + p.y); }
    Pt operator - (Pt p) const { return Pt(x - p.x, y - p.y); }
    Pt operator * (T k) const { return Pt(k * x, k * y); }
    Pt operator / (T k) const { return Pt(x / k, y / k); }

    bool operator == (Pt p) const {
        return fabs(x - p.x) < eps && fabs(y - p.y) < eps;
    }
};

T dot(Pt a, Pt b) { return a.x * b.x + a.y * b.y; }
T cross(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }
T dist2(Pt a, Pt b) { return dot(a - b, a - b); }
// atan2l for long double
T angle(Pt a, Pt b) { return atan2(cross(a, b), dot(a, b)); }
// rotate a by t radians counter-clockwise
Pt rotateccw(Pt a, T t) {
    return Pt(a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t));
}

// Project c on (ab) (a != b)
Pt project_on_line(Pt a, Pt b, Pt c) {
    return a + (b - a) * dot(c - a, b - a) / dist2(a, b);
}
// Project c on [a,b] (a != b)
Pt project_on_segment(Pt a, Pt b, Pt c) {
    T t = dot(c - a, b - a) / dist2(a, b);
    if (t < -eps) return a;
    if (t > +eps) return b;
    return a + (b - a) * t;
}

bool strictly_same_sign(T x, T y) {
    return (x > eps && y > eps) || (x < -eps && y < -eps);
}
bool same_sign(T x, T y) {
    return (x > -eps && y > -eps) || (x < eps && y < eps);
}

// (ab) // (cd)?
bool lines_parallel(Pt a, Pt b, Pt c, Pt d) {
    return fabs(cross(b - a, d - c)) < eps;
}
// (ab) = (cd)?
bool lines_equal(Pt a, Pt b, Pt c, Pt d) {
    return lines_parallel(a, b, c, d) && fabs(cross(c - a, b - a)) < eps;
}
// [ab] and [cd] intersect?
bool segments_intersect(Pt a, Pt b, Pt c, Pt d) {
    if (lines_equal(a, b, c, d))
        return dot(c-a, c-b) < eps || dot(d-a, d-b) < eps || dot(c-b, d-b) < eps;
    if (strictly_same_sign(cross(b - a, c - a), cross(b - a, d - a))) return false;
    if (strictly_same_sign(cross(d - c, a - c), cross(d - c, b - c))) return false;
    return true;
}
// (ab) inter (cd), provided that (ab) and (cd) are not parallel
Pt lines_intersection(Pt a, Pt b, Pt c, Pt d) {
    return a + (b - a) * cross(d - c, c - a) / cross(d - c, b - a);
}

// center of circle going through a,b,c
// a,b,c should not be colinear
Pt circle_center(Pt a, Pt b, Pt c) {
    Pt p = (a + b) / 2, q = (a + c) / 2;
    return lines_intersection(p, p + rotateccw(b - a, pi / 2),
                              q, q + rotateccw(c - a, pi / 2));
}

// Returns 1 if a is in the interior of p, 0 in the exterior
// and 0 or 1 on the boundary
bool in_polygon(vector<Pt> p, Pt a) {
    int n = p.size(), is = 0;
    FOR(i, n) {
        int j = (i + 1) % n;
        is ^= (p[i].y > a.y) != (p[j].y > a.y) &&
            a.x < p[i].x + (p[j].x - p[i].x) * (a.y - p[i].y) / (p[j].y - p[i].y);
    }
    return is;
}
// Returns 1 if point is on the boundary of p, 0 otherwise
bool on_polygon(vector<Pt> p, Pt a) {
    int n = p.size();
    FOR(i, n) if (dist2(a, project_on_segment(p[i], p[(i + 1) % n], a)) < eps) {
        return true;
    }
    return false;
}

// (ab) inter S(c, r)
vector<Pt> circle_line_intersection(Pt a, Pt b, Pt c, T r) {
    vector<Pt> ans;
    Pt d = b - a;
    T D = cross(a - c, b - c), e = r * r * dot(d, d) - D * D;
    if (e < -eps) return ans;
    e = sqrt(fabs(e));
    int sgn = d.y > -eps ? +1 : -1;
    ans.pb(c + Pt(D * d.y + sgn * d.x * e, -D * d.x + fabs(d.y) * e) / dot(d, d));
    if (e > eps) {
        ans.pb(c + Pt(D*d.y - sgn*d.x*e, -D*d.x - fabs(d.y)*e) / dot(d, d));
    }
    return ans;
}

// S(a, r) inter S(b, R)
vector<Pt> circle_circle_intersection(Pt a, Pt b, T r, T R) {
    vector<Pt> ans;
    T d = sqrt(dist2(a, b));
    if (d > r + R + eps || d + min(r, R) < max(r, R) - eps) return ans;
    // fabs to avoid sqrt(-0...)
    T x = (d * d - R * R + r * r) / 2 / d, y = sqrt(fabs(r * r - x * x));
    Pt v = (b - a) / d;
    ans.pb(a + v * x + rotateccw(v, pi / 2) * y);
    if (y > eps) ans.pb(a + v * x - rotateccw(v, pi / 2) * y);
    return ans;
}

// Signed area of the polygon p
T poly_signed_area(vector<Pt> p) {
    int n = p.size();
    T ans = 0;
    FOR(i, n) ans += cross(p[i], p[(i + 1) % n]);
    return ans / 2;
}

// (Unsigned area) of the polygon p
T poly_area(vector<Pt> p) { return fabs(poly_signed_area(p)); }

// Centroid of the polygon p
Pt poly_centroid(vector<Pt> p) {
    int n = p.size();
    Pt c(0, 0);
    FOR(i, n) c = c + (p[i] + p[(i + 1) % n]) * cross(p[i], p[(i + 1) % n]);
    return c / 6 / poly_signed_area(p);
}

// Is the polygon p simple?
bool poly_simple(vector<Pt> p) {
    int n = p.size();
    FOR(i, n) FORU(k, i + 1, n) {
        int j = (i + 1) % n, l = (k + 1) % n;
        if (i != l && j != k && segments_intersect(p[i], p[j], p[k], p[k])) {
            return false;
        }
    }
    return true;
}

// Two methods to compute the convex hull of the set p
vector<Pt> convex_hull(vector<Pt> p) {
    if (SZ(p) == 1) return p;
    sort(p.begin(), p.end(), [&](Pt a, Pt b) { return mp(a.y, a.x) < mp(b.y, b.x); });
    vector<Pt> h;
    FOR(step, 2) {
        auto s = h.size();
        for (Pt a: p) {
            // < eps -> < -eps to include points on the boundary
            while (h.size() >= s + 2
              && cross(h[SZ(h) - 1] - h[SZ(h) - 2], a - h[SZ(h) - 2]) < eps) {
                h.pop_back();
            }
            h.pb(a);
        }
        h.pop_back(); reverse(p.begin(), p.end());
    }
    return h;
}

vector<Pt> other_convex_hull(vector<Pt> p) {
    for (Pt& a: p) if (mp(a.y, a.x) < mp(p[0].y, p[0].x)) swap(a, p[0]);
    sort(p.begin() + 1, p.end(), [&](Pt a, Pt b) {
        T o = angle(a - p[0], b - p[0]);
        if (o < -eps || o > eps) return o > eps;
        return dist2(a, p[0]) < dist2(b, p[0]);
    });
    vector<Pt> h;
    for (Pt a: p) {
        // < eps -> < -eps to include points on the boundary
        while (h.size() >= 2
          && cross(h[SZ(h) - 1] - h[SZ(h) - 2], a - h[SZ(h) - 2]) < eps) {
            h.pop_back();
        }
        h.pb(a);
    }
    return h;
}
