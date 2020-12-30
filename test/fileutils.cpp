#include "catch.hpp"

#include <string>

// #include "tests_utils.h"

#include "sequence/sequence.h"
#include "sequence/fileutils.h"

using std::cout;
using std::endl;
using std::string;

using sequence::Collection;
using sequence::assemble;
using sequence::parse;

// --------------------------------------------------------------------------
// 
TEST_CASE("collection from file", "fileutils") {
//    vector<sequence::Collection> collections;
//    vector<std::string> remainders;
//    vector<std::string> entries;

//    entries.push_back("one");
//    entries.push_back("two");
//    entries.push_back("three");

//    tie(collections, remainders) = sequence::assemble(entries);
//    REQUIRE(remainders.size() == 3);
//    REQUIRE(remainders.at(0) == "one");
//    REQUIRE(remainders.at(1) == "two");
//    REQUIRE(remainders.at(2) == "three");
//    REQUIRE(collections.size() == 0);
    std::pair<Collection, bool> result;
    result = sequence::fileutils::collection_from_file("C:\\dev\\samples\\sample.1.exr");
    // auto res = test_collection(
    //     result.first,
    //     get_mock("sample.", ".exr", "sample.[1:4].exr", "sample. .exr 1:4 1:4 0 ",
    //         4, 0, 1, 4)
    // );
    // CHECK(res.size() == 0);
    REQUIRE(result.second == true);
}
