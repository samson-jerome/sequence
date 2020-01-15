
#include "catch.hpp"

#include "sequence.h"
#include "collection.h"

using std::cout;
using std::endl;

// --------------------------------------------------------------------------
// Check remainders
TEST_CASE("assembling single files", "[assemble]") {
    vector<sequence::Collection> collections;
    vector<std::string> remainders;
    vector<std::string> entries;

    entries.push_back("one");
    entries.push_back("two");
    entries.push_back("three");

    tie(collections, remainders) = sequence::assemble(entries);
    REQUIRE(remainders.size() == 3);
    REQUIRE(collections.size() == 0);
}

// --------------------------------------------------------------------------
// Check collections
TEST_CASE("assembling collections", "[assemble]") {
  vector<sequence::Collection> collections;
  vector<std::string> remainders;
  vector<std::string> entries;

  entries.push_back("aaa.1.ext");
  entries.push_back("aaa.2.ext");
  entries.push_back("aaa.3.ext");
  entries.push_back("aaa.4.ext");
  entries.push_back("bbb.1.ext");
  entries.push_back("bbb.2.ext");
  entries.push_back("bbb.5.ext");

  tie(collections, remainders) = sequence::assemble(entries);
  REQUIRE(remainders.size() == 0);
  REQUIRE(collections.size() == 2);

  sequence::Collection aaa = collections.at(0);
  sequence::Collection bbb = collections.at(1);

  // SECTION( "complete sequence" ) {
    cout << aaa.format() << endl;
    REQUIRE(aaa.format() == "aaa.[1:4].ext");
    REQUIRE(aaa.count() == 4);
  // }

  // SECTION( "incomplete sequence" ) {
    cout << bbb.format() << endl;
    REQUIRE(bbb.format() == "bbb.[1:2,5].ext");
    REQUIRE(bbb.count() == 3);
  // }
}

// --------------------------------------------------------------------------
// Parsing
TEST_CASE("parsing fails if not collection", "[parsing]") {
  sequence::Collection collection;
  std::string entry;

  entry = "my_file.ext";
  try {
    collection = sequence::parse(entry);
  } 
  catch (const sequence::parse_exception& e) {
    REQUIRE(true);
  }
  catch (...) {
    cout << "Unhandled exception" << endl;
    REQUIRE(false);
  }
}

TEST_CASE("parsing success", "[parsing]") {
  sequence::Collection collection;
  std::string entry;

  entry = "my_file.%04d.ext [1-10]";
  try {
    collection = sequence::parse(entry);
  } 
  catch (const sequence::parse_exception& e) {
    REQUIRE(false);
  }
  catch (...) {
    cout << "Unhandled exception" << endl;
    REQUIRE(false);
  }

  REQUIRE(true);
}