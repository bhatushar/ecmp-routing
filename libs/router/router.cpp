#include <sstream>
#include <iomanip>
#include "router/router.hpp"

Router::Router(const char name, const network::PathMap& pm): id(name), pathsToRouter(pm) {}

Router::~Router() {
    for (auto& [_, obj]: pathsToRouter)
        delete obj;
}

std::string Router::getNextPath(const char dst) {
    if (pathsToRouter.find(dst) == pathsToRouter.end()) {
        std::stringstream ss;
        ss << "No path from " << id << " to " << dst;
        return ss.str();
    }
    auto& [_, pathIter, paths] = *(pathsToRouter[dst]);
    auto path = paths[pathIter];
    pathIter = (pathIter + 1) % paths.size();
    return path;
}

std::string Router::getPaths(const char dst) const {
    std::stringstream ss;
    if (dst == 0)
        // Show all paths
        for (const auto& [key, _]: pathsToRouter)
            ss << getPathsTo(key);
    else if (pathsToRouter.find(dst) == pathsToRouter.end())
        ss << "No path from " << id << " to " << dst << std::endl;
    else return getPathsTo(dst);
    return ss.str();
}

void Router::destroyAll(std::map<char, Router*>& routers) {
    for (auto& [_, router]: routers)
        delete router;
}

std::string Router::getPathsTo(char dst) const {
    const auto& [pathCost, pathIter, paths] = *(pathsToRouter.at(dst));
    std::stringstream ss;
    ss << id << " => " <<  dst 
        << " [" << pathCost << "]:" << std::endl;
    for (uint32_t i = 0; i < paths.size(); i++) {
        char indicator = (i == pathIter) ? '*' : ' ';
        ss << std::setw(4) << indicator;
        ss << paths[i] << std::endl;
    }
    return ss.str();
}
