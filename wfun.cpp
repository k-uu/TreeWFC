
#include <iostream>
#include "prototype.h"
#include "module.h"

using std::cout;
using prototype::Prototype;
using module::Module;

int main() {
    Prototype p("_", vector<string>{"t_","r_","b_","l|"});
    if (p.tile == "_") {
        cout<<"!";
    }

    Module m(1, 1);
    m.constrain("t_");
    cout << m.entropy();
    for (string s: m.getPossibleNeighbors('t')) {
        cout << s;
    }

}