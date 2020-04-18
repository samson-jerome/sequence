#include "catch.hpp"

#include "sequence.h"
#include "collection.h"

using std::cout;
using std::endl;

using sequence::Collection;
using sequence::assemble;
using sequence::parse;

// --------------------------------------------------------------------------
// Check remainders
TEST_CASE("constructor", "[creation]") {

	Collection collection = Collection("head.", ".tail", {1, 2, 3});
	REQUIRE(collection.count() == 3);
	REQUIRE(collection.head() == "head.");
	REQUIRE(collection.tail() == ".tail");
	REQUIRE(collection.first() == 1);
	REQUIRE(collection.last() == 3);

    collection = Collection("head.", ".tail", { 1, 2, 3 }, 4);
    REQUIRE(collection.count() == 3);
    REQUIRE(collection.head() == "head.");
    REQUIRE(collection.tail() == ".tail");
    REQUIRE(collection.padding() == 4);
    REQUIRE(collection.first() == 1);
    REQUIRE(collection.last() == 3);

    REQUIRE(collection.getItem(1) == "head.0001.tail");
    REQUIRE(collection.getItem(2) == "head.0002.tail");
    REQUIRE(collection.getItem(3) == "head.0003.tail");
}

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
    REQUIRE(remainders.at(0) == "one");
    REQUIRE(remainders.at(1) == "two");
    REQUIRE(remainders.at(2) == "three");
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
  entries.push_back("bbb.001.ext");
  entries.push_back("bbb.002.ext");
  entries.push_back("bbb.005.ext");

  tie(collections, remainders) = sequence::assemble(entries);
  REQUIRE(remainders.size() == 0);
  REQUIRE(collections.size() == 2);

  sequence::Collection aaa = collections.at(0);
  sequence::Collection bbb = collections.at(1);

  // SECTION( "complete sequence" ) {
    cout << aaa.format() << " - padding: " << aaa.padding() <<endl;
    REQUIRE(aaa.format() == "aaa.[1:4].ext");
    REQUIRE(aaa.count() == 4);
    REQUIRE(aaa.head() == "aaa.");
    REQUIRE(aaa.tail() == ".ext");
  // }

  // SECTION( "incomplete sequence" ) {
    cout << bbb.format() << " - padding: " << bbb.padding() <<endl;
    REQUIRE(bbb.format() == "bbb.[001:002,005].ext");
    REQUIRE(bbb.count() == 3);
    REQUIRE(bbb.padding() == 3);
    REQUIRE(bbb.head() == "bbb.");
    REQUIRE(bbb.tail() == ".ext");
  // }
}

// --------------------------------------------------------------------------
// Parsing
TEST_CASE("parsing fails if not collection", "[parsing]") {
  std::string entry;

  entry = "my_file.ext";
  try {
	  Collection collection = sequence::parse(entry);
  } 
  catch (const sequence::parse_exception& e) {
    REQUIRE(true);
  }
  catch (...) {
    cout << "Unhandled exception" << endl;
    REQUIRE(false);
  }
}
TEST_CASE("Parsing successs with various correct formats"){
  std::vector<std::string> entries;

  REQUIRE(sequence::parse("frame.%04d.exr [1050-1080]").format() == "frame.[1050:1080].exr");
  REQUIRE(sequence::parse("frame%03d.tga [001-100]").format() == "frame[001:100].tga");

  cout << endl;

}

TEST_CASE("parsing success", "[parsing]") {
  std::string entry;

  entry = "my_file.%04d.ext [1-10]";
  try {
    Collection collection = sequence::parse(entry);
    cout << "result is: " << collection.format() << endl;
    REQUIRE(collection.count() == 10);
    REQUIRE(collection.first() == 1);
    REQUIRE(collection.last() == 10);
    REQUIRE(collection.head() == "my_file.");
    REQUIRE(collection.tail() == ".ext");
  } 
  catch (const sequence::parse_exception& e) {
    REQUIRE(false);
  }
  catch (...) {
    cout << "Unhandled exception" << endl;
    REQUIRE(false);
  }

  entry = "my_file.%4d.ext [1-10]";
  try {
    Collection collection = sequence::parse(entry);
    cout << "result is: " << collection.format() << endl;

    REQUIRE(collection.count() == 10);
    REQUIRE(collection.first() == 1);
    REQUIRE(collection.last() == 10);
    REQUIRE(collection.head() == "my_file.");
    REQUIRE(collection.tail() == ".ext");
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

TEST_CASE("parsing failures on various wrong collections", "[parsing]") {
  std::string entry;

  entry = "my_file.%04.ext [1-10]";
  try {
	Collection collection = sequence::parse(entry);
  } 
  catch (const sequence::parse_exception& e) {
    REQUIRE(true);
    cout << "fail to parse: " << entry << endl;
  }
  catch (...) {
    cout << "Unhandled exception" << endl;
    REQUIRE(false);
  }

  REQUIRE(true);
}