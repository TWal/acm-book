#include "misc.h"
#include <precontest/template.cpp>
#include <misc/lis_short.cpp>
#include <misc/lis_full.cpp>
#include <rapidcheck.h>

void testMisc() {
    rc::check("lis_short, lis_full: returns the same size", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        RC_ASSERT(lisSize(v) == (lli)lis(v).size());
    });

    rc::check("lis_full: returns an increasing subsequence", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        vi seq = lis(v);
        lli l = seq.size();
        FOR(i, l-1) {
            RC_ASSERT(v[seq[i]] < v[seq[i+1]]);
        }
    });

    rc::check("lis_short: returns the good size", []() {
        vi v = *rc::gen::arbitrary<vi>().as("vector");
        lli n = v.size();
        vi dp(n);
        FOR(i, n) {
            dp[i] = 1;
            if(i != 0) {
                FOR(j, i) {
                    if(v[j] < v[i]) {
                        dp[i] = max(dp[i], 1+dp[j]);
                    }
                }
            }
        }
        lli res = 0;
        FOR(i, n) {
            res = max(res, dp[i]);
        }
        RC_ASSERT(res == lisSize(v));
    });
}
