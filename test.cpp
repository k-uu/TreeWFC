#define CATCH_CONFIG_MAIN
#include "util/catch.hpp"

#include "priority.h"
#include "wfcAlgorithm.h"

using priority::ModuleQueue;
using wfc::Model;
using std::cout;

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
    cout << m1;

    Module m2(1, 99); // has more leaves as valid neighbors compared to m1
    m2.constrain("t#");
    m2.constrain("r|");
    m2.constrain("b#");
    m2.constrain("l#");
    cout << m2;


    Module m3(1, 98); // smaller height_ value (higher in image)
    m3.constrain("t#");
    m3.constrain("r#");
    m3.constrain("b#");
    m3.constrain("l|");
    cout << m3;

    q.push(m3);
    q.push(m2);
    q.push(m1);
    REQUIRE(q.size() == 3);
    REQUIRE(q.pop().getPrototypes()[0].tile == "|");
    REQUIRE(q.pop().getPosition().second == 98);
    REQUIRE(q.size() == 1);
    REQUIRE(q.pop().getPrototypes()[0].tile == "#");

    REQUIRE(q.isEmpty());
}

TEST_CASE("algorithm") {

    unsigned width = 10;
    unsigned height = 10;

    Model model = Model(width, height);

    model.iterate(1);

    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            cout << model.getModule(std::make_pair(x, y)).getPrototypes()[0].tile << " ";
        }
        cout << std::endl;
    }

}


