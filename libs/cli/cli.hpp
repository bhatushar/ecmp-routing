#include <vector>
#include <string>
#include "network/network.hpp"

namespace cli {
    const uint8_t NETWORK = 0, PATHS = 1, PATHS_SRC = 2, PATHS_SRC_DST = 3, SEND = 4, EXIT = 5, NOP = 6;
    const uint8_t ERR_UNKNOWN = 100, ERR_NETWORK_ARGS = 101, ERR_PATHS_ARGS = 102, ERR_SEND_ARGS = 103;
    
    std::vector<std::string> readCmd();
    uint8_t decode(std::vector<std::string>&);

    void printGraph(const network::Graph&);
}
