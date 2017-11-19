// const int MAX_TRA = 26; //number of transitions
// int normalize(char c) { //normalize `c` in [0, ..., MAX_TRA-1]
//     return c - 'a';
// }

struct PalindromicTree {
    //len = length of the palindrome, sufCount = number of suffix palindrome
    //link = node of the maximum suffix palindrome, occAsMax = used in computeOcc
    struct Node {
        int len, link, sufCount, occAsMax, next[MAX_TRA];
        Node() : occAsMax(0) { FOR(i, MAX_TRA) next[i] = -1; }
    };

    string s;
    vector<Node> t; //tree. node 0: root with len -1, node 1: root with len 0
    int suff; //node of the current maximum suffix palindrome

    PalindromicTree() {
        suff = 1;
        t.resize(2);
        t[0].len = -1; t[0].link = 0; t[0].sufCount = 0;
        t[1].len = 0; t[1].link = 0; t[1].sufCount = 0;
    }

    int suffix(int x, int i) {
        while(i-t[x].len-1 < 0 || s[i-t[x].len-1] != s[i]) x = t[x].link;
        return x;
    }


    //returns true if `c` created a new distinct palindrome
    bool add(char c) {
        int let = normalize(c);
        int pos = s.size();
        s.pb(c);
        suff = suffix(suff, pos);

        int& x = t[suff].next[let];
        bool newNode = (x == -1);
        if(newNode) {
            x = t.size();
            t.emplace_back();
            t[x].len = t[suff].len + 2;
            t[x].link = suff == 0 ? 1 : t[suffix(t[suff].link, pos)].next[let];
            t[x].sufCount = 1+t[t[x].link].sufCount;
        }
        ++t[x].occAsMax;
        suff = x;
        return newNode;
    }

    // occ[i] = number of occurences of the node i in the string
    vi computeOcc() {
        vi occ(t.size());
        FOR(i, t.size()) occ[i] = t[i].occAsMax;
        FORD(i, 0, occ.size()) occ[t[i].link] += occ[i];
        return occ;
    }
};
