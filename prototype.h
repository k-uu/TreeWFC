#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <string>
#include <vector>

#include "util/json.hpp"

using std::string;
using nlohmann::json;
using std::vector;

namespace prototype {

    class Prototype {
    public:
        int tile;
        vector<string> skt;

        Prototype(int type, vector<string> sockets);
        Prototype();
        bool operator== (Prototype const &other) const;
    };

    inline void from_json(const json &j, Prototype &p) {
        j.at("tile").get_to(p.tile);
        j.at("sockets").get_to(p.skt);
    }
}

#endif