#include <algorithm>
#include <unordered_map>
#include "network/network.hpp"

namespace network {
    // Prototypes for local functions
    bool farther(const Node*, const Node*);
    void constructPaths(const Node*, std::vector<std::string>&, std::vector<char>&);
    
    // Contrustors
    ConnectedRouter::ConnectedRouter(char name, uint32_t cost): id(name), linkCost(cost) {}
    Node::Node(char name): id(name) {}
    RouterPaths::RouterPaths(uint32_t cost, std::vector<std::string>& p): pathCost(cost), paths(p) {}

    Graph build(std::ifstream& fin) {
        Graph g;
        while (!fin.eof()) {
            char node1, node2;
            uint32_t cost;
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

    void destroy(Graph& g) {
        for (auto& [_, vec]: g)
            for (auto& obj: vec)
                delete obj;
    }

    PathMap shortestPath(const Graph& g, const char src) {
        std::unordered_map<char, Node*> nodeMap;
        std::vector<Node*> queue;
        // Create a Node object for each vertext in graph
        for (const auto& [name, _]: g) {
            Node* node = new Node(name);
            if (name == src) {
                node->costFromSource = 0;
                node->isConnected = true;
            }
            nodeMap[name] = node;
            queue.emplace_back(node);
        }
        // Start Dijkstra's path finding algorithm
        std::make_heap(queue.begin(), queue.end(), farther);
        while (!queue.empty()) {
            // Get node with least cost from source
            std::pop_heap(queue.begin(), queue.end(), farther);
            Node* parent = queue.back();
            queue.pop_back();
            // Relax all adjacent nodes
            for (const auto* router: g.at(parent->id)) {
                Node* child = nodeMap[router->id];
                uint32_t totalCost = parent->costFromSource + router->linkCost;
                if (!child->isConnected || child->costFromSource > totalCost) {
                    // Child is either not explored or a better path is found
                    child->isConnected = true;
                    child->costFromSource = totalCost;
                    // Remove existing parents
                    child->parents = { parent };
                    // Ideally, only need to decrease key here
                } else if (child->costFromSource == totalCost)
                    // New path with same cost found
                    child->parents.emplace_back(parent);
            }
            // Highly unoptimized, but it works
            std::make_heap(queue.begin(), queue.end(), farther);
        }
        // Construct path strings
        PathMap pm;
        for (const auto& [name, node]: nodeMap) {
            if (name == src) continue;
            std::vector<std::string> paths;
            std::vector<char> rPath;
            constructPaths(node, paths, rPath);
            pm[name] = new RouterPaths(node->costFromSource, paths);
        }
        // Release memory
        for (auto& [_, node]: nodeMap)
            delete node;
        return pm;
    }

    /**
     * True if n1 is farther from the source than n2, false otherwise.
     * Used as the comparator for the priority queue in Dijkstra's algorithm.
     */
    bool farther(const network::Node* n1, const network::Node*n2) {
        if (!n1->isConnected) return true;
        if (!n2->isConnected) return false;
        return n1->costFromSource > n2->costFromSource;
    }

    /**
     * Converts all simple paths from leaves to root (node) into strings of form "A -> B -> C".
     */
    void constructPaths(const network::Node* node, std::vector<std::string>& paths, std::vector<char>& rPath) {
        rPath.push_back(node->id);
        for (const network::Node* parent: node->parents)
            constructPaths(parent, paths, rPath);
        if (node->parents.empty()) {
            std::string path = "";
            for (auto i = rPath.rbegin(); next(i) != rPath.rend(); i++) {
                path += *i;
                path += " -> ";
            }
            path += rPath.front();
            paths.push_back(path);
        }
        rPath.pop_back();
    }
}

