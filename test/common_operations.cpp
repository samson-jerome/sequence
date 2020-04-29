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
        result.push_back("Error in collection::count");
    if(c.head() != mock.head)
        result.push_back("Error in collection::head");
    if(c.tail() != mock.tail)
        result.push_back("Error in collection::tail");
    if(c.padding() != mock.padding)
        result.push_back("Error in collection::padding");
    if(c.first() != mock.first)
        result.push_back("Error in collection::first");
    if(c.last() != mock.last)
        result.push_back("Error in collection::last");
    if(c.format() != mock.format)
        result.push_back("Error in collection::format");
    if(c.format(full_format) != mock.full_format)
        result.push_back("Error in collection::full_format = " + c.format(full_format));

    return result;
}
// --------------------------------------------------------------------------
// Check remainders
TEST_CASE("constructor", "[creation]") {
    MockCollection mock;
    vector<string> res;

    res = test_collection(
        Collection("head.", ".tail", 1, 3),
        get_mock("head.", ".tail", "head.[1:3].tail", "head. .tail 1:3 1:3 0 ",
            3, 0, 1, 3)
    );
    for(string err : res) { cout << err << endl; }
    CHECK(res.size() == 0);


    // REQUIRE(collection1.count() == 3);
    // REQUIRE(collection1.head() == "head.");
    // REQUIRE(collection1.tail() == ".tail");
    // REQUIRE(collection1.padding() == 0);
    // REQUIRE(collection1.first() == 1);
    // REQUIRE(collection1.last() == 3);
    // REQUIRE(collection1.format() == "head.[1:3].tail");
    // cout << collection1.format("{head}[{global_range}]{tail}") << endl;

    res = test_collection(
        Collection("head.", ".tail", 1, 5),
        get_mock("head.", ".tail", "head.[1:5].tail", "head. .tail 1:5 1:5 0 ",
            5, 0, 1, 5)
    );
    for(string err : res) { cout << err << endl; }
    CHECK(res.size() == 0);

    // Collection collection2 = Collection("head.", ".tail", 1, 5);
    // REQUIRE(collection2.count() == 5);
    // REQUIRE(collection2.head() == "head.");
    // REQUIRE(collection2.tail() == ".tail");
    // REQUIRE(collection2.padding() == 0);
    // REQUIRE(collection2.first() == 1);
    // REQUIRE(collection2.last() == 5);
    // cout << collection2.format() << endl;

    // Collection collection3 = Collection("head.", ".tail", 1, 6, 4);
    // REQUIRE(collection3.count() == 6);
    // REQUIRE(collection3.head() == "head.");
    // REQUIRE(collection3.tail() == ".tail");
    // REQUIRE(collection3.padding() == 4);
    // REQUIRE(collection3.first() == 1);
    // REQUIRE(collection3.last() == 6);
    // REQUIRE(collection3.firstItem() == "head.0001.tail");
    // REQUIRE(collection3.lastItem() == "head.0006.tail");

    // collection3.add(7);
    // REQUIRE(collection3.lastItem() == "head.0007.tail");
    
    // std::vector<int> frames= {10, 11, 12};
    // collection3.add(frames);
    // REQUIRE(collection3.format() == "head.[0001:0007,0010:0012].tail");

    // collection3.remove(8);
    // REQUIRE(collection3.format() == "head.[0001:0007,0010:0012].tail");

    // collection3.remove(7);
    // REQUIRE(collection3.format() == "head.[0001:0006,0010:0012].tail");

    // collection3.remove(frames);
    // REQUIRE(collection3.format() == "head.[0001:0006].tail");

    // // collection3.addItem("head.0007.tail");
    // // REQUIRE(collection3.lastItem() == "head.0007.tail");

    // // REQUIRE(collection3.getFrames() == );
    // // REQUIRE(collection3.getHoles() == );
    
    // cout << collection3.format() << endl;

    // Collection collection4 = Collection("head.", ".tail", { 5, 6, 7 }, 4);
    // REQUIRE(collection4.count() == 3);
    // REQUIRE(collection4.head() == "head.");
    // REQUIRE(collection4.tail() == ".tail");
    // REQUIRE(collection4.padding() == 4);
    // REQUIRE(collection4.first() == 5);
    // REQUIRE(collection4.last() == 7);
    // cout << collection4.format() << endl;

    // Collection collection5 = Collection("head.", ".tail", {1050, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058 ,1059 ,1060 ,1061 ,1062 ,1063 ,1064 ,1065 ,1066 ,1067 ,1068 ,1069 ,1070 ,1071 ,1072 ,1073 ,1074 ,1075 ,1076 ,1077, 1078, 1079, 1080});
    // REQUIRE(collection5.count() == 31);
    // REQUIRE(collection5.head() == "head.");
    // REQUIRE(collection5.tail() == ".tail");
    // REQUIRE(collection5.padding() == 0);
    // REQUIRE(collection5.first() == 1050);
    // REQUIRE(collection5.last() == 1080);
    // cout << collection5.format() << endl;

    // // @todo add copy const test
    // Collection copy = collection5;
    // REQUIRE(copy.count() == 31);
    // REQUIRE(copy.head() == "head.");
    // REQUIRE(copy.tail() == ".tail");
    // REQUIRE(copy.padding() == 0);
    // REQUIRE(copy.first() == 1050);
    // REQUIRE(copy.last() == 1080);
    // cout << copy.format() << endl;

    // auto item = copy.getItem(1050);
    // REQUIRE(item.first == "head.1050.tail");
    // REQUIRE(item.second == true);

    // item = copy.getItem(1051);
    // REQUIRE(item.first == "head.1051.tail");
    // REQUIRE(item.second == true);
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