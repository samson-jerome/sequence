#include "catch.hpp"

#include <map>
#include <string>

#include "sequence.h"
#include "collection.h"


using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;
using std::vector;

using sequence::Collection;
using sequence::assemble;
using sequence::parse;

struct MockCollection{
    string head, tail, format, full_format;
    int count, padding, first, last;
//    vector<int> indices, holes;
};

MockCollection get_mock(string head, string tail, string format, 
    string full_format, int count, int padding, int first, int last) 
{
    MockCollection result;
    result.head = head;
    result.tail = tail;
    result.format = format;
    result.full_format = full_format;

    result.count = count;
    result.padding = padding;
    result.first = first;
    result.last = last;

    return result;
}

vector<string> test_collection(Collection c, MockCollection mock) {

    string full_format = "{head} {tail} {global_range} {ranges} {padding} {holes}";
    vector<string> result;

    cout << "TESTING " << c.format() << endl;

    if( c.count() != mock.count)
        result.push_back("Error in collection::count = " + std::to_string(c.count()));
    if(c.head() != mock.head)
        result.push_back("Error in collection::head = " + c.head());
    if(c.tail() != mock.tail)
        result.push_back("Error in collection::tail = " + c.tail());
    if(c.padding() != mock.padding)
        result.push_back("Error in collection::padding = " + std::to_string(c.padding()));
    if(c.first() != mock.first)
        result.push_back("Error in collection::first = " + std::to_string(c.first()));
    if(c.last() != mock.last)
        result.push_back("Error in collection::last = " + std::to_string(c.last()));
    if(c.format() != mock.format)
        result.push_back("Error in collection::format = "  + c.format());
    if(c.format(full_format) != mock.full_format)
        result.push_back("Error in collection::full_format = " + c.format(full_format));

    for(string err : result) { cout << err << endl; }
    return result;
}


// --------------------------------------------------------------------------
// Check remainders
TEST_CASE("constructor", "[creation]") {
    MockCollection mock;
    vector<string> res;

    res = test_collection(
        Collection("head.", ".tail", 1, 5),
        get_mock("head.", ".tail", "head.[1:5].tail", "head. .tail 1:5 1:5 0 ",
            5, 0, 1, 5)
    );
    CHECK(res.size() == 0);


    res = test_collection(
        Collection("head.", ".tail", 1, 5, 4),
        get_mock("head.", ".tail", "head.[0001:0005].tail", "head. .tail 0001:0005 0001:0005 4 ",
            5, 4, 1, 5)
    );
    CHECK(res.size() == 0);


    Collection edited = Collection("head.", ".tail", {10, 11, 12, 13});
    res = test_collection(
        edited,
        get_mock("head.", ".tail", "head.[10:13].tail", "head. .tail 10:13 10:13 0 ",
            4, 0, 10, 13)
    );
    CHECK(res.size() == 0);

    edited.remove(10);
    res = test_collection(
        edited,
        get_mock("head.", ".tail", "head.[11:13].tail", "head. .tail 11:13 11:13 0 ",
            3, 0, 11, 13)
    );
    CHECK(res.size() == 0);

    edited.add(10);
    res = test_collection(
        edited,
        get_mock("head.", ".tail", "head.[10:13].tail", "head. .tail 10:13 10:13 0 ",
            4, 0, 10, 13)
    );
    CHECK(res.size() == 0);

    edited.add({1, 2, 3, 4, 5, 6});
    res = test_collection(
        edited,
        get_mock("head.", ".tail", "head.[1:6,10:13].tail", "head. .tail 1:13 1:6,10:13 0 7,8,9",
            10, 0, 1, 13)
    );
    CHECK(res.size() == 0);

    edited.remove({5, 6, 7, 8, 9, 10 ,11});
    res = test_collection(
        edited,
        get_mock("head.", ".tail", "head.[1:4,12:13].tail", "head. .tail 1:13 1:4,12:13 0 5,6,7,8,9,10,11",
            6, 0, 1, 13)
    );
    CHECK(res.size() == 0);

    // @audit how to handle clear and empty collection?
    // edited.clear();
    // res = test_collection(
    //     edited,
    //     get_mock("head.", ".tail", "head.[].tail", "head. .tail   0 ",
    //         0, 0, 0, 0)
    // );
    // CHECK(res.size() == 0);

    CHECK(edited.lastItem() == "head.13.tail");

    vector<int> frames = {1,2,3,4,12,13};
    CHECK(edited.frames() == frames);

    vector<int> holes = {5,6,7,8,9,10,11};
    CHECK(edited.holes() == holes);


    // -------------------------------------------------------------------------
    //  Large collection
    Collection large = Collection("head.", ".tail", 1001, 1950);
    res = test_collection(
        large,
        get_mock("head.", ".tail", "head.[1001:1950].tail", "head. .tail 1001:1950 1001:1950 0 ",
            950, 0, 1001, 1950)
    );
    CHECK(res.size() == 0);

    // -------------------------------------------------------------------------
    //  Negative collection
    Collection negative = Collection("head.", ".tail", -15, -5);
    res = test_collection(
        negative,
        get_mock("head.", ".tail", "head.[-15:-5].tail", "head. .tail -15:-5 -15:-5 0 ",
            11, 0, -15, -5)
    );
    CHECK(res.size() == 0);

    // // @todo add copy const test
    Collection copy = large;
    res = test_collection(
        large,
        get_mock("head.", ".tail", "head.[1001:1950].tail", "head. .tail 1001:1950 1001:1950 0 ",
            950, 0, 1001, 1950)
    );
    CHECK(res.size() == 0);
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

  cout << aaa.format() << endl;
  REQUIRE(aaa.format() == "aaa.[1:4].ext");
  REQUIRE(aaa.count() == 4);
  REQUIRE(aaa.head() == "aaa.");
  REQUIRE(aaa.tail() == ".ext");

  cout << bbb.format() << endl;
  REQUIRE(bbb.format() == "bbb.[001:002,005].ext");
  REQUIRE(bbb.count() == 3);
  REQUIRE(bbb.padding() == 3);
  REQUIRE(bbb.head() == "bbb.");
  REQUIRE(bbb.tail() == ".ext");
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

  entry = "my_file.%4d.ext [11-20]";
  try {
    Collection collection = sequence::parse(entry);
    cout << "result is: " << collection.format() << endl;

    REQUIRE(collection.count() == 10);
    REQUIRE(collection.first() == 11);
    REQUIRE(collection.last() == 20);
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