#include <iostream>
#include "cli/cli.hpp"

namespace cli {
    std::vector<std::string> readCmd() {
        size_t curr = 0, next;
        std::string cmd;
        std::vector<std::string> cmdVec;
        std::cout << "> ";
        std::getline(std::cin, cmd);
        // Split cmd into words
        next = cmd.find(' ');
        while (next != std::string::npos) {
            cmdVec.push_back(cmd.substr(curr, next - curr));
            curr = next + 1;
            next = cmd.find(' ', curr);
        }
        cmdVec.push_back(cmd.substr(curr, std::min(next, cmd.size()) - curr));
        return cmdVec;
    }

    uint8_t decode(std::vector<std::string>& cmdVec) {
        /*
            Commands:
                network
                paths [src [dst]]
                send src dst
        */
        uint8_t code = ERR_UNKNOWN;
        if (cmdVec[0] == "") code = NOP;
        else if (cmdVec[0] == "network")
            if (cmdVec.size() == 1) code = NETWORK;
            else code = ERR_NETWORK_ARGS;
        else if (cmdVec[0] == "paths") {
            if (cmdVec.size() == 1) code = PATHS;
            else if (cmdVec.size() == 2) code = PATHS_SRC;
            else if (cmdVec.size() == 3) code = PATHS_SRC_DST;
            else code = ERR_PATHS_ARGS;
        } else if (cmdVec[0] == "send") {
            if (cmdVec.size() == 3) code = SEND;
            else code = ERR_SEND_ARGS;
        } else if (cmdVec[0] == "exit") code = EXIT;
        return code;
    }

    void printGraph(const network::Graph& g) {
        for (const auto& [src, neighbours]: g) {
            std::cout << src << " -> ";
            for (const auto* router: neighbours)
                std::cout << "[" << router->id 
                << " " << router->linkCost << "] ";
            std::cout << std::endl;
        }
    }
}

