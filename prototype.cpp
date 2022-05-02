
#include "prototype.h"

using std::to_string;

namespace prototype {

    Prototype::Prototype(int type, vector<string> sockets) {
        tile = type;
        skt = sockets;
    }

    Prototype::Prototype() {
        tile = 0;
        skt = vector<string>{"t0", "r0", "b0", "l0"};
    }

    bool Prototype::operator==(const Prototype &other) const {
        if (tile != other.tile) return false;

        if (skt.size() != other.skt.size()) return false;

        auto iter = skt.begin();
        auto iter_other = other.skt.begin();

        while (iter != skt.end()) {
            if (*iter != *iter_other) return false;
            iter++;
            iter_other++;
        }

        return true;
    }
}