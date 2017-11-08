#include "misc.h"
#include <precontest/template.cpp>
#include <misc/lis_short.cpp>
#include <misc/lis_full.cpp>

//ugly hack
#define lower_bound upper_bound
#define lis lis_nondec
#define lisSize lisSize_nondec
#include <misc/lis_short.cpp>
#include <misc/lis_full.cpp>
#undef lisSize
#undef lis
#undef lower_bound

#include <rapidcheck.h>

static lli dumbLisSize(const vi& v, bool nondec) {
    lli n = v.size();
    vi dp(n);
    FOR(i, n) {
        dp[i] = 1;
        if(i != 0) {
            FOR(j, i) {
                bool cond = nondec ? v[j] <= v[i] : v[j] < v[i];
                if(cond) {
                    dp[i] = max(dp[i], 1+dp[j]);
                }
            }
        }
    }
    lli res = 0;
    FOR(i, n) {
        res = max(res, dp[i]);
    }
    return res;
}

void testMisc() {
    rc::check("lis_short, lis_full: returns the same size", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        RC_ASSERT(lisSize(v) == (lli)lis(v).size());
    });

    rc::check("lis_short, lis_full (nondec): returns the same size", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        RC_ASSERT(lisSize_nondec(v) == (lli)lis_nondec(v).size());
    });

    rc::check("lis_full: returns an increasing subsequence", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        vi seq = lis(v);
        FOR(i, seq.size()-1) {
            RC_ASSERT(v[seq[i]] < v[seq[i+1]]);
        }
    });


    rc::check("lis_full (nondec): returns non-decreasing subsequence", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        vi seq = lis_nondec(v);
        FOR(i, seq.size()-1) {
            RC_ASSERT(v[seq[i]] <= v[seq[i+1]]);
        }
    });

    rc::check("lis_short: returns the good size", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        RC_ASSERT(dumbLisSize(v, false) == lisSize(v));
    });

    rc::check("lis_short (nondec): returns the good size", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        RC_ASSERT(dumbLisSize(v, true) == lisSize_nondec(v));
    });
}
