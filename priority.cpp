#include <math.h>
#include<assert.h>

#include "priority.h"


using std::swap;

namespace priority {

    ModuleQueue::ModuleQueue(unsigned int imageHeight): height_(imageHeight), queue_() {}

    double ModuleQueue::tilePriority(Module m) {
        unsigned y = m.getPosition().second;

        assert(y < height_);

        double multiplier = -cos(M_PI * y / height_);
        int tileSum = 0;
        for (const auto &p : m.getPrototypes()) {
            tileSum += conversions_[p.tile[0]];
            for (const string &s: p.skt) {
                tileSum += conversions_[s[1]];
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
        queue_.push_back(m);
        heapifyUp(queue_.size() - 1);
    }

    Module ModuleQueue::pop() {

        assert(!isEmpty());

        Module ret = queue_[0];
        swap(queue_[0], queue_.back());
        queue_.pop_back();

        heapifyDown(0);

        return ret;
    }

    bool ModuleQueue::isEmpty() {
        return queue_.size() == 0;
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
        if (higherPriority(queue_[idx], queue_[parentIdx])) {
            swap(queue_[idx], queue_[parentIdx]);
            heapifyUp(parentIdx);
        }
    }

    void ModuleQueue::heapifyDown(size_t idx) {

        if (!hasChild(idx)) return;

        size_t leftIdx = leftChild(idx);
        size_t rightIdx = rightChild(idx);

        size_t priorityIdx = rightIdx >= queue_.size() || higherPriority(queue_[leftIdx], queue_[rightIdx]) ?
                leftIdx : rightIdx;

        if (higherPriority(queue_[priorityIdx], queue_[idx])) {
            swap(queue_[priorityIdx], queue_[idx]);
            heapifyDown(priorityIdx);
        }

    }

    bool ModuleQueue::hasChild(size_t parentIdx) {
        return leftChild(parentIdx) < queue_.size();
    }

    size_t ModuleQueue::size() {
        return queue_.size();
    }

}