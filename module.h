#ifndef MODULE_H
#define MODULE_H

#include <vector>

#include "prototype.h"

using std::vector;
using std::pair;
using namespace prototype;

namespace module {

    class Module {
        vector<Prototype> possible_prototypes_;
        pair<unsigned, unsigned> pos_;

        void generatePrototypes(); //use JSON file to generate all allowed prototypes

    public:
        Module(unsigned x, unsigned y);
        void constrain(vector<string> sockets);
        void constrain(string socket);
        bool hasCollapsed() const;
        pair<unsigned, unsigned> getPosition() const;
        void collapse(Prototype &p);
        vector<string> getPossibleNeighbors(pair<unsigned, unsigned> dir);
        unsigned entropy() const;
        vector<Prototype> getPrototypes() const;



        static const std::map<char, int> tileValues;
    };
}

std::ostream & operator << (std::ostream &out, const module::Module &m);

#endif