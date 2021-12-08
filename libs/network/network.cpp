#include "network/network.hpp"

using namespace network;

ConnectedRouter::ConnectedRouter(char name, uint32_t cost): id(name), linkCost(cost) {}

Graph build(std::ifstream& fin) {
    size_t routerCount;
    Graph g;
    fin >> routerCount;
    while (!fin.eof()) {
        char node1, node2;
        unsigned int cost;
        fin >> node1 >> node2 >> cost;
        if (g.find(node1) == g.end())
            g[node1] = std::vector<ConnectedRouter*>();
        if (g.find(node2) == g.end())
            g[node2] = std::vector<ConnectedRouter*>();
        g[node1].emplace_back(new ConnectedRouter(node2, cost));
        g[node2].emplace_back(new ConnectedRouter(node1, cost));
    }
    return g;
}
