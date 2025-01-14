#include "catch.hpp"

#include "sequence/sequence.h"
#include "sequence/collection.h"

using std::cout;
using std::endl;

// --------------------------------------------------------------------------
// 
TEST_CASE("gettings items", "collection") {
    vector<sequence::Collection> collections;
    vector<std::string> remainders;
    vector<std::string> items;
    vector<std::string> entries;

    entries.push_back("seq.001.ext");
    entries.push_back("seq.002.ext");
    entries.push_back("seq.003.ext");

    tie(collections, remainders) = sequence::assemble(entries);
    collections.at(0).info();
 
    items = collections.at(0).items();
    cout << items.at(0) << endl;
 
    REQUIRE(items.at(0) == "seq.001.ext");
    REQUIRE(items.at(1) == "seq.002.ext");
    REQUIRE(items.at(2) == "seq.003.ext");

    // cout << collections.at(0).getItem(2) << endl;
    REQUIRE(collections.at(0).getItem(1).first == "seq.001.ext");
    REQUIRE(collections.at(0).getItem(2).first == "seq.002.ext");
    REQUIRE(collections.at(0).getItem(3).first == "seq.003.ext");
}

// --------------------------------------------------------------------------
// @todo
// TEST_CASE("inserting/removing items", "[collection]") {
// 	vector<sequence::Collection> collections;

// }