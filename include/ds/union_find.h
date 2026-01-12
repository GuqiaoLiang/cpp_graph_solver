#pragma once

#include <vector>

class UnionFind {
public:
    explicit UnionFind(int n);

    int Find(int v);
    bool Connected(int v1, int v2);
    void Union(int v1, int v2);

    int SizeOf(int v) const;
    int Parent(int v) const;

private:
    std::vector<int> parent_;

    void Check(int v) const;
};
