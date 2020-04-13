#include "catch.hpp"

#include "sequence.h"
#include "collection.h"

using std::cout;
using std::endl;

// --------------------------------------------------------------------------
// 
TEST_CASE("gettings items", "[collection]") {
    vector<sequence::Collection> collections;
    vector<std::string> remainders;
    vector<std::string> items;
    vector<std::string> entries;

    entries.push_back("seq.001.ext");
    entries.push_back("seq.002.ext");
    entries.push_back("seq.003.ext");

    tie(collections, remainders) = sequence::assemble(entries);
    items = collections.at(0).getItems();

    REQUIRE(items.at(0) == "seq.001.ext");
    REQUIRE(items.at(1) == "seq.002.ext");
    REQUIRE(items.at(2) == "seq.003.ext");
}