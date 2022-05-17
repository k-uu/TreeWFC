#ifndef WFC_ALGORITHM_H
#define WFC_ALGORITHM_H

#include "module.h"
#include "priority.h"

#include <vector>

using module::Module;
using priority::ModuleQueue;

namespace wfc {

    class Model {

        vector<Module> cells_;
        unsigned height_;
        unsigned width_;
        ModuleQueue queue_;

        pair<unsigned, unsigned> minEntropy();
        void collapseAt(pair<unsigned, unsigned> coords);
        void propagate(pair<unsigned, unsigned> coords);
        double collapseBias(Prototype p, pair<unsigned, unsigned> coords);
        vector< pair<int, int> > getDirections(pair<unsigned, unsigned> coords);

    public:

        /**
         * initialize the image for wfc
         * @param width of the image
         * @param height of the image
         * @param start is in [1, width] and determines the start of propagation
         */
        Model(unsigned width, unsigned height);

        void iterate(unsigned start);

        int state();

        Module &getModule(pair<unsigned, unsigned> coords);

    };
}

#endif //WFC_ALGORITHM_H
