#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <fstream>
#include <vector>
#include <map>

namespace network {
    struct ConnectedRouter {
        char id;
        uint32_t linkCost;
        ConnectedRouter(char, uint32_t);
    };

    struct Node {
        char id;
        uint32_t costFromSource = 0;
        std::vector<Node*> parents;
        bool isConnected = false;
        Node(char);
    };

    struct RouterPaths {
        uint32_t pathCost;
        uint32_t pathIter = 0;
        std::vector<std::string> paths;
        RouterPaths(uint32_t, std::vector<std::string>&);
    };

    typedef std::map<char, std::vector<ConnectedRouter*>> Graph;
    typedef std::map<char, RouterPaths*> PathMap;

    Graph build(std::ifstream&);

    PathMap shortestPath(const Graph&, const char);
}

#endif
