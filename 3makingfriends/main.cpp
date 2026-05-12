#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

struct Edge
{
    int u, v, weight;
};

struct UnionFind
{
    std::vector<int> parent;
    std::vector<int> size;

    UnionFind(int n)
    {
        parent.resize(n + 1);
        size.resize(n + 1, 1);

        for (int i = 0; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    int find(int i)
    {
        if (parent[i] == i)
        {
            return i;
        }

        return parent[i] = find(parent[i]);
    }

    bool unite(int i, int j)
    {
        int root_i = find(i);
        int root_j = find(j);

        if (root_i == root_j)
        {
            return false;
        }

        if (size[root_i] < size[root_j])
        {
            std::swap(root_i, root_j);
        }

        parent[root_j] = root_i;
        size[root_i] += size[root_j];

        return true;
    }
};

int mst(std::vector<Edge> &edges, int people)
{
    int components = people;
    int totalWeight = 0;
    UnionFind uf(people);

    while (components > 1)
    {
        std::vector<int> cheapest(people + 1, -1);

        for (int i = 0; i < edges.size(); i++)
        {
            int root_u = uf.find(edges[i].u);
            int root_v = uf.find(edges[i].v);

            if (root_u != root_v)
            {
                if (cheapest[root_u] == -1 || edges[i].weight < edges[cheapest[root_u]].weight)
                {
                    cheapest[root_u] = i;
                }

                if (cheapest[root_v] == -1 || edges[i].weight < edges[cheapest[root_v]].weight)
                {
                    cheapest[root_v] = i;
                }
            }
        }

        for (int i = 1; i <= people; i++)
        {
            if (cheapest[i] != -1)
            {
                int edge_idx = cheapest[i];

                bool merged = uf.unite(edges[edge_idx].u, edges[edge_idx].v);

                if (merged)
                {
                    totalWeight += edges[edge_idx].weight;
                    components--;
                }
            }
        }
    }

    return totalWeight;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int people;
    int pairs;

    std::cin >> people;
    std::cin >> pairs;

    std::vector<Edge> edges;
    edges.reserve(pairs);

    for (int i = 0; i < pairs; i++)
    {
        int u, v, weight;
        std::cin >> u >> v >> weight;

        edges.push_back({u, v, weight});
    }

    int result = mst(edges, people);
    std::cout << result;

    return 0;
}