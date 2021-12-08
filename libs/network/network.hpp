#include <fstream>
#include <vector>
#include <map>

namespace network {
    struct ConnectedRouter {
        char id;
        uint32_t linkCost;
        ConnectedRouter(char, uint32_t);
    };

    typedef std::map<char, std::vector<ConnectedRouter*>> Graph;

    Graph build(std::ifstream&);
}
