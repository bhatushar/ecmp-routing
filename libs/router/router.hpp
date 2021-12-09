#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include "network/network.hpp"

class Router {
    char id;
    network::PathMap pathsToRouter;
    std::string getPathsTo(char) const;
public:
    Router(const char, const network::PathMap&);
    std::string getNextPath(char);
    std::string getPaths(char = 0) const;
    static void destroyAll(std::map<char, Router*>&);
    ~Router();
};


#endif
