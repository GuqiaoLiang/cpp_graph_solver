#include "ds/union_find.h"
#include <stdexcept>

UnionFind::UnionFind(int n) : parent_(n, -1) {}

void UnionFind::Check(int v) const {
    if (v < 0 || v >= parent_.size()) {
        throw std::out_of_range("vertex out of range");
    }
}

int UnionFind::Find(int v) {
    Check(v);
    if (parent_[v] < 0) {
        return v;
    }
    parent_[v] = Find(parent_[v]);  // path compression
    return parent_[v];
}

bool UnionFind::Connected(int v1, int v2) {
    Check(v1);
    Check(v2);
    return Find(v1) == Find(v2);
}

int UnionFind::SizeOf(int v) const {
    Check(v);
    int r = v;
    while (parent_[r] >= 0) {
        r = parent_[r];
    }
    return -parent_[r];
}

int UnionFind::Parent(int v) const {
    Check(v);
    return parent_[v];
}

void UnionFind::Union(int v1, int v2) {
    int r1 = Find(v1);
    int r2 = Find(v2);
    if (r1 == r2) return;

    int size1 = -parent_[r1];
    int size2 = -parent_[r2];

    if (size1 > size2) {
        parent_[r1] -= size2;
        parent_[r2] = r1;
    } else {
        parent_[r2] -= size1;
        parent_[r1] = r2;   // tie -> r2 (和你 Java 版一致)
    }
}
