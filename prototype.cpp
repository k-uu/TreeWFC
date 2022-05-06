
#include "prototype.h"

using std::to_string;

namespace prototype {

    /**
     *
     * @param type representation of object: '|' -> trunk, '#' -> leaves, '_' -> blank space
     * @param sockets
     */
    Prototype::Prototype(string type, vector<string> sockets) {
        tile = type;
        skt = sockets;
    }

    Prototype::Prototype() {
        tile = "_";
        skt = vector<string>();
    }

    bool Prototype::operator==(const Prototype &other) const {
        if (tile != other.tile) return false;

        vector<string> sockets = skt;
        vector<string> other_sockets = other.skt;

        for (int i = 0; i < 4; i++) {
            if (sockets[i] != other_sockets[i]) return false;
        }
        return true;
    }

    bool Prototype::operator!=(const Prototype &other) const {
        return !operator==(other);
    }
}