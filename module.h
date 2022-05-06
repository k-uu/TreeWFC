#ifndef MODULE_H
#define MODULE_H

#include <vector>

#include "prototype.h"

using std::vector;
using std::pair;
using namespace prototype;

namespace module {

    class Module {
        vector<Prototype> possible_prototypes;
        pair<unsigned, unsigned> pos;

        void generatePrototypes(); //use JSON file to generate all allowed prototypes

    public:
        Module(unsigned x, unsigned y);
        void constrain(string socket);
        bool hasCollapsed() const;
        pair<unsigned, unsigned> getPosition() const;
        void collapse(Prototype &p);
        vector<string> getPossibleNeighbors(char dir);
        unsigned entropy() const;
        vector<Prototype> getPrototypes() const;

        static const std::unordered_map<char, int> tileValues;
    };

}

#endif