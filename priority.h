#ifndef PRIORITY_H
#define PRIORITY_H

#include "module.h"


using module::Module;

namespace priority {

    // using zero-index heap
    class ModuleQueue {

        unsigned height_;
        vector<Module> queue_;

        std::map<char, int> conversions_ = Module::tileValues;

        /**
         * Returns a double in [0, 4 * m.entropy()] giving the priority of the m. Larger priority value -> higher
         * priority
         * @param m
         * @return priority value
         */
        double tilePriority(Module m);
        void heapifyUp(size_t idx);
        void heapifyDown(size_t idx);
        size_t getParent(size_t childIdx);
        size_t leftChild(size_t parentIdx);
        size_t rightChild(size_t parentIdx);
        bool hasChild(size_t parentIdx);
        bool higherPriority(Module m1, Module m2);

    public:
        ModuleQueue(unsigned imageHeight);
        void push(Module m);
        Module pop();
        bool isEmpty();
        size_t size();
    };
}

#endif