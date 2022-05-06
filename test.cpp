#define CATCH_CONFIG_MAIN
#include "util/catch.hpp"

#include "module.h"
#include "priority.h"

using module::Module;
using priority::ModuleQueue;

TEST_CASE("prototype equility") {
    Prototype p1("_", vector<string>{"t|","r_","b#","l|"});
    Prototype p2 = p1;
    Prototype p3("_", vector<string>{"t|","r#","b_","l|"}); // swapped sockets
    Prototype p4("|", vector<string>{"t|","r#","b_","l|"}); // different tile;

    REQUIRE(p1 == p2);
    REQUIRE(p1 != p3);
    REQUIRE(p1 != p4);
}

TEST_CASE("collapse and constrain") {
    Module m(1, 1);
    REQUIRE(m.entropy() != 1);

    SECTION("collapse") {
        Prototype p = m.getPrototypes()[2];
        m.collapse(p);
        REQUIRE(m.entropy() == 1);
        REQUIRE(m.getPrototypes()[0] == p);
    }

    SECTION("constrain") {
        Prototype p("#", vector<string>{"t#","r#","b#","l#"});
        m.constrain("t#");
        m.constrain("r#");
        m.constrain("b#");
        m.constrain("l#");
        m.constrain("l_"); // check that a collapsed module is not further constrained
        REQUIRE(m.entropy() == 1);
        REQUIRE(m.getPrototypes()[0] == p);
    }
}

TEST_CASE("tilePriority") {
    ModuleQueue q(100);
    Module m1(1, 99);
    m1.constrain("t|");
    m1.constrain("r|");
    m1.constrain("b|");
    m1.constrain("l|");
    Module m2(1, 98);
    m2.constrain("t#");
    m2.constrain("r#");
    m2.constrain("b#");
    m2.constrain("l#");

    q.push(m1);
    q.push(m2);
    REQUIRE(q.size() == 2);
    REQUIRE(q.pop().getPosition().second == 99);
    q.pop();
    REQUIRE(q.isEmpty());
}


