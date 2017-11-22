// Monotonic queue (min/max on a sliding window)
// Default behavior: min queue
template<class T>
struct MonotonicQueue {
    deque<pair<T, lli>> q;

    bool empty() { return q.empty(); }

    // Add v to the queue at time t
    // t must increase at each call
    void add(T v, lli t) {
        // Change > to < to get a max queue
        while (!q.empty() && q.back().fst > v) q.pop_back();
        q.pb({v, t});
    }

    // Delete the element from time t
    // Return true iff. the element is still in the queue
    // Successive calls must be in the same order as add
    bool remove(lli t) {
        if (!q.empty() && q.front().snd == t) {
            q.pop_front();
            return true;
        }
        return false;
    }

    T get() { return q.front().fst; }
};

