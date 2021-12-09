#include <iostream>
#include <string>
#include "network/network.hpp"
#include "router/router.hpp"
#include "cli/cli.hpp"

int main() {
    std::vector<std::string> cmdVec;
    network::Graph g;
    std::map<char, Router*> routers;
    std::ifstream fin("network.txt");

    if (!fin.is_open()) {
        std::cout << "Cannot open network.txt to read graph data!";
        return EXIT_FAILURE;
    }

    g = network::build(fin);
    for (const auto& [rid, _]: g) {
        auto pm = network::shortestPath(g, rid);
        routers[rid] = new Router(rid, pm);
    }
    cli::printGraph(g);

    do {
        char src = 0, dst = 0;
        cmdVec = cli::readCmd();
        uint8_t code = cli::decode(cmdVec);
        switch (code)
        {
        case cli::NETWORK:
            cli::printGraph(g);
            break;
        case cli::PATHS:
            for (const auto& [_, router]: routers)
                std::cout << router->getPaths() << std::endl;
            break;
        case cli::PATHS_SRC_DST:
            dst = cmdVec[2][0];
        case cli::PATHS_SRC:
            src = cmdVec[1][0];
            if (routers.find(src) == routers.end())
                std::cout << "'" << src << "' is not a router" << std::endl;
            else
                std::cout << routers[src]->getPaths(dst);
            break;
        case cli::SEND:
            src = cmdVec[1][0];
            dst = cmdVec[2][0];
            if (routers.find(src) == routers.end())
                std::cout << "'" << src << "' is not a router" << std::endl;
            else
                std::cout << routers[src]->getNextPath(dst) << std::endl;
            break;
        // Errors
        case cli::ERR_UNKNOWN:
            std::cout << "'" << cmdVec[0] << "' is not a valid command" << std::endl;
            break;
        case cli::ERR_NETWORK_ARGS:
            std::cout << "Invalid arguments passed to 'network'" << std::endl;
            break;
        case cli::ERR_PATHS_ARGS:
            std::cout << "'paths' requires at most 2 arguments: paths [src [dst]]" << std::endl;
            break;
        case cli::ERR_SEND_ARGS: 
            std::cout << "'send' requires 2 arguments: send src dst" << std::endl;
            break;
        }
    } while (cmdVec.front() != "exit");
    
    Router::destroyAll(routers);
    network::destroy(g);
    return EXIT_SUCCESS;
}
