#include <fstream>

#include "module.h"
#include "prototype.h"

using prototype::Prototype;
using std::make_pair;
using std::map;
using std::endl;

namespace module {

    Module::Module(unsigned x, unsigned y) {
        pos_ = make_pair(x , y);
        generatePrototypes();
    }

    void Module::collapse(Prototype &p) {
        for (auto i = possible_prototypes_.begin(); i != possible_prototypes_.end(); i++) {
            Prototype target = *i;
            if (target == p) {
                possible_prototypes_ = vector<Prototype>{target};
                return;
            }
        }
    }

    void Module::constrain(string socket) {
        constrain(vector<string>{socket});
    }

    void Module::constrain(vector<string> sockets) {
        // time complexity: O(n)

        unsigned size = possible_prototypes_.size();
        unsigned rear = size;
        unsigned i = 0;

        while(i < rear) {

            bool compatible = false;
            for (string s : possible_prototypes_[i].skt) { // cycle through 4 sockets to find target socket
                for (string socket: sockets) {
                    if (s == socket) { //if any of the sockets fit, the prototype is compatible;
                        compatible = true;
                    }
                }
            }
            if (compatible) {
                i++;
            } else {
                rear--;
                std::swap(possible_prototypes_[i], possible_prototypes_[rear]);
            }

        }
        while (rear < size) {
            possible_prototypes_.pop_back();
            size--;
        }
    }

    bool Module::hasCollapsed() const {
        return possible_prototypes_.size() == 1;
    }

    vector<string> Module::getPossibleNeighbors(pair<unsigned, unsigned> dir) {
        vector<string> ret = vector<string>();

        static map<pair<int, int>, char> toChar{{make_pair(0, -1),'t'},
                                                        {make_pair(1, 0),'r'},
                                                        {make_pair(0, 1),'b'},
                                                        {make_pair(-1,0),'l'}};

        static map<pair<int, int>, char> comp{{make_pair(0, -1),'b'},
                                              {make_pair(1, 0),'l'},
                                              {make_pair(0, 1),'t'},
                                              {make_pair(-1,0),'r'}};

        for (const Prototype &proto : possible_prototypes_) {
            for (const string &socket : proto.skt) {

                if (socket[0] == toChar[dir]) {
                    string complement = socket;
                    complement[0] = comp[dir];
                    ret.push_back(complement);
                }
            }
        }

        return ret;
    }

    void Module::generatePrototypes() {
        std::ifstream f("prototypes.json");
        json j = nlohmann::json::parse(f);
        f.close();
        possible_prototypes_ = j.get<vector<Prototype>>();
    }

    pair<unsigned int, unsigned int> Module::getPosition() const {
        return pos_;
    }

    unsigned int Module::entropy() const {
        return possible_prototypes_.size();
    }

    // returns a single prototype if collapsed
    vector<Prototype> Module::getPrototypes() const {
        return possible_prototypes_;
    }

    const map<char, int> Module::tileValues = map<char, int>{{'#',-1}, {'_',0}, {'|',1}};

}

std::ostream & operator << (std::ostream &out, const module::Module &m) {
    out << "Number of Possible Prototypes: " << m.entropy() << endl;
    for (const Prototype &p : m.getPrototypes()) {

        out << " " << p.skt[0][1] << endl;
        out << p.skt[3][1] << p.tile << p.skt[1][1] << endl;
        out << " " << p.skt[2][1] << endl;
        out << endl;
    }
    return out;
}