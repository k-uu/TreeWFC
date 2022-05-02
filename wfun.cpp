
#include <iostream>
#include "prototype.h"
#include "module.h"

using std::cout;
using prototype::Prototype;
using module::Module;

int main() {
    Prototype p(1, vector<string>{"t0","r2","b1","l0"});
    if (p.tile == 1) {
        cout<<"!";
    }

    Module m(1, 1);
    m.constrain("t0");
    cout << m.entropy();
    for (string s: m.getPossibleNeighbors('t')) {
        cout << s;
    }

}