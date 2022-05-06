#include <math.h>
#include<assert.h>

#include "priority.h"


using std::swap;

namespace priority {

    ModuleQueue::ModuleQueue(unsigned int imageHeight): height(imageHeight), queue() {}

    double ModuleQueue::tilePriority(Module m) {
        unsigned y = m.getPosition().second;

        assert(y < height);

        double multiplier = -cos(M_PI * y / height);
        int tileSum = 0;
        for (const auto &p : m.getPrototypes()) {
            tileSum += conversions[p.tile[0]];
            for (string s: p.skt) {
                tileSum += conversions[s[1]];
            }
        }
        return multiplier * tileSum;
    }

    bool ModuleQueue::higherPriority(Module m1, Module m2) {
        if (m1.entropy() == m2.entropy()) {
            return tilePriority(m1) > tilePriority(m2);
        } else {
            return m1.entropy() < m2.entropy();
        }
    }

    void ModuleQueue::push(Module m) {
        queue.push_back(m);
        heapifyUp(queue.size() - 1);
    }

    Module ModuleQueue::pop() {

        assert(!isEmpty());

        Module ret = queue[0];
        swap(queue[0], queue.back());
        queue.pop_back();

        heapifyDown(0);

        return ret;
    }

    bool ModuleQueue::isEmpty() {
        return queue.size() == 0;
    }

    size_t ModuleQueue::leftChild(size_t parentIdx) {
        return parentIdx * 2 + 1;
    }

    size_t ModuleQueue::rightChild(size_t parentIdx) {
        return parentIdx * 2 + 2;
    }

    size_t ModuleQueue::getParent(size_t childIdx) {
        return (childIdx - 1) / 2;
    }

    void ModuleQueue::heapifyUp(size_t idx) {

        if (idx == 0) return;

        size_t parentIdx = getParent(idx);
        if (higherPriority(queue[idx], queue[parentIdx])) {
            swap(queue[idx], queue[parentIdx]);
            heapifyUp(parentIdx);
        }
    }

    void ModuleQueue::heapifyDown(size_t idx) {

        if (!hasChild(idx)) return;

        size_t leftIdx = leftChild(idx);
        size_t rightIdx = rightChild(idx);

        size_t priorityIdx = rightIdx >= queue.size() || higherPriority(queue[leftIdx], queue[rightIdx]) ?
                leftIdx : rightIdx;

        if (higherPriority(queue[priorityIdx], queue[idx])) {
            swap(queue[priorityIdx], queue[idx]);
            heapifyDown(priorityIdx);
        }

    }

    bool ModuleQueue::hasChild(size_t parentIdx) {
        return leftChild(parentIdx) < queue.size();
    }

    size_t ModuleQueue::size() {
        return queue.size();
    }

}