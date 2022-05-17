//
// Created by Oskar Blyt on 2022-05-07.
//

#include <map>

#include <iostream>

#include "wfcAlgorithm.h"


using std::make_pair;

namespace wfc {

    // overload +operator -> TODO: this is messy
    pair<unsigned, unsigned> operator+ (const pair<unsigned, unsigned>& lhs, const pair<int, int>& rhs) {
        return make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
    }

    Model::Model(unsigned int width, unsigned int height): queue_(height) {

        width_ = width;
        height_ = height;

        cells_.reserve(width * height); //improves perfomance

        for (unsigned x = 0; x < width; x++) {
            for (unsigned y = 0; y < height; y++) {
                cells_.push_back(Module(x, y));
            }
        }
    }

    int Model::state() {
        for (const Module &m : cells_) { // can this be more efficient?
            if (m.entropy() != 1) return 1;
            if (m.entropy() == 0) return -1;
        }
        return 0;
    }

    pair<unsigned int, unsigned int> Model::minEntropy() {
        Module min = cells_.front();

        for (const Module &m : cells_) {
            if ((m.entropy() < min.entropy() && !m.hasCollapsed()) || min.hasCollapsed()) { //collapsed modules are not included
                min = m;
            }
        }
        return min.getPosition();
    }

    Module &Model::getModule(pair<unsigned int, unsigned int> coords) {
        unsigned idx = coords.second + height_ * coords.first;

        return cells_[idx];
    }

    vector<pair<int, int> > Model::getDirections(pair<unsigned int, unsigned int> coords) {
        vector<pair<int, int> > dirs = vector<pair<int, int> >();

        if (coords.second < height_ - 1) { // has a bottom neighbor
            dirs.push_back(make_pair(0, 1));
        }

        if (coords.first > 0) { // has a left neighbor
            dirs.push_back(make_pair(-1, 0));
        }

        if (coords.first < width_ - 1) { // has a right neighbor
            dirs.push_back(make_pair(1, 0));
        }

        if (coords.second > 0) { // has a top neighbor
            dirs.push_back(make_pair(0, -1));
        }

        return dirs;
    }

    void Model::propagate(pair<unsigned int, unsigned int> coords) {

        std::map<std::pair<unsigned, unsigned>, bool> visited;
        Module target = getModule(coords);

        queue_.push(target);
        visited[target.getPosition()] = true;

        while (!queue_.isEmpty()) {

            pair<unsigned, unsigned> currCoords = queue_.pop().getPosition();

            vector< pair<int, int> > validDir = getDirections(currCoords);

            for (const pair<unsigned, unsigned> &dir : validDir) {

                pair<unsigned, unsigned> otherCoords = currCoords + dir;

                vector<string> possibleNeighbors = getModule(currCoords).getPossibleNeighbors(dir);

                Module &neighbor = getModule(otherCoords);

                neighbor.constrain(possibleNeighbors);

                auto iter = visited.find(otherCoords);

                if (iter == visited.end()) {
                    visited.insert(make_pair(otherCoords, true));
                    queue_.push(getModule(otherCoords));
                }
            }

        }
    }

    double Model::collapseBias(Prototype p, pair<unsigned int, unsigned int> coords) {
        double multiplier = -cos(M_PI * coords.second / height_);

        std::map<char, int> conversion = module::Module::tileValues;

        double bias = conversion[p.tile[1]];

        for (const string &s: p.skt) {
            bias += conversion[s[1]];
        }
        return bias * multiplier + (rand() % 5);
    }


    void Model::collapseAt(pair<unsigned int, unsigned int> coords) {

        vector<Prototype> prototypes = getModule(coords).getPrototypes();

        Prototype target = prototypes.front();
        for (const auto &p : prototypes) {
            if (collapseBias(p, coords) > collapseBias(target, coords)) {
                target = p;
            }
        }
        getModule(coords).collapse(target);
    }

    void Model::iterate(unsigned start) {

        assert(start <= width_ && start > 0);

        pair<unsigned, unsigned> startCoords = make_pair(start - 1, height_ - 1);

        Module &seed = getModule(startCoords);
        seed.constrain("t|");
        seed.constrain("r|");
        seed.constrain("l|");
        seed.constrain("b|");
        collapseAt(startCoords);
        propagate(startCoords);

        while(state()) {
            if (state() == -1) {
                std::cout << "contradictory state. Stopping iteration..." << std::endl;
                break;
            }
            pair<unsigned, unsigned> coords = minEntropy();
            collapseAt(coords);
            propagate(coords);
        }
    }
}