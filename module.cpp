#include <fstream>

#include "module.h"
#include "prototype.h"

using prototype::Prototype;
using std::make_pair;
using std::unordered_map;

namespace module {

    Module::Module(unsigned x, unsigned y) {
        pos = make_pair(x ,y);
        generatePrototypes();
    }

    void Module::collapse(Prototype &p) {
        for (auto i = possible_prototypes.begin(); i != possible_prototypes.end(); i++) {
            Prototype target = *i;
            if (target == p) {
                possible_prototypes = vector<Prototype>{target};
                return;
            }
        }
    }

    void Module::constrain(string socket) {
        if (!hasCollapsed()) {
            unsigned rear = possible_prototypes.size();
            unsigned size = rear;
            for (unsigned i = 0; i < rear; i++) {
                for (string s : possible_prototypes[i].skt) {
                    if (s[0] == socket[0] && s[1] != socket[1]) {
                        rear--;
                        std::swap(possible_prototypes[i], possible_prototypes[rear]);
                    }
                }
            }
            while (rear < size) {
                possible_prototypes.pop_back();
                size--;
            }
        }
    }

    bool Module::hasCollapsed() const {
        return possible_prototypes.size() == 1;
    }

    vector<string> Module::getPossibleNeighbors(char dir) {
        vector<string> ret = vector<string>();

        static unordered_map<char, char> comp{{'t','b'},
                                              {'r','l'},
                                              {'b','t'},
                                              {'l','r'}};

        if (!hasCollapsed()) {
            for (const Prototype &proto : possible_prototypes) {
                for (const string &socket : proto.skt) {

                    if (socket[0] == dir) {
                        string complement = socket;
                        complement[0] = comp[dir];
                        ret.push_back(complement);
                    }
                }
            }
        }
        return ret;
    }

    void Module::generatePrototypes() {
        std::ifstream f("prototypes.json");
        json j = nlohmann::json::parse(f);
        f.close();
        possible_prototypes = j.get<vector<Prototype>>();
    }

    pair<unsigned int, unsigned int> Module::getPosition() const {
        return pos;
    }

    unsigned int Module::entropy() const {
        return possible_prototypes.size();
    }

    // returns a single prototype if collapsed
    vector<Prototype> Module::getPrototypes() const {
        return possible_prototypes;
    }

    const unordered_map<char, int> Module::tileValues = unordered_map<char, int>{{'#',-1},
                                                                               {'_',0},
                                                                               {'|',1}};

}