
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


}