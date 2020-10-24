#include "catch.hpp"

#include <map>
#include <string>

#include "sequence/sequence.h"
#include "sequence/collection.h"


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
    string head="", tail="", format="", full_format="";
    int count=0, padding=0, first=0, last=0;
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

vector<string> test_collection(Collection c, MockCollection mock, bool quiet=false) {

    string full_format = "{head} {tail} {global_range} {ranges} {padding} {holes}";
    vector<string> result;

    if(!quiet) {
        cout << "TESTING " << c.format() << endl;
    }

    if(c.head() != mock.head)
        result.push_back("Error in collection::head: \"" + c.head() + "\" <> \"" + mock.head + "\"");
    if(c.tail() != mock.tail)
        result.push_back("Error in collection::tail: \"" + c.tail() + "\" <> \"" + mock.tail + "\"");

    if( c.count() != mock.count)
        result.push_back("Error in collection::count: \"" \
            + std::to_string(c.count()) + "\" <> \"" \
            + std::to_string(mock.count) + "\"");
    if(c.padding() != mock.padding)
        result.push_back("Error in collection::padding: \"" \
            + std::to_string(c.padding()) + "\" <> \"" \
            + std::to_string(mock.padding) + "\"");
    if(c.first() != mock.first)
        result.push_back("Error in collection::first: \"" \
            + std::to_string(c.first()) + "\" <> \"" \
            + std::to_string(mock.first) + "\"");
    if(c.last() != mock.last)
        result.push_back("Error in collection::last: \"" \
            + std::to_string(c.last()) + "\" <> \"" \
            + std::to_string(mock.last) + "\"");

    if(c.format() != mock.format)
        result.push_back("Error in collection::format: \"" + c.format() + "\" <> \"" + mock.format + "\"");
    if(c.format(full_format) != mock.full_format)
        result.push_back("Error in collection::full_format: \"" + c.format(full_format) + "\" <> \"" + mock.full_format + "\"");

    if(!quiet) {
        for(string err : result) { cout << err << endl; }
    }
    return result;
}


vector<string> test_parser(string input_sequence, MockCollection mock, bool quiet=false) {
    vector<string> result;

    if(!quiet) {
        cout << "PARSING " << input_sequence << endl;
    }

    Collection c = sequence::parse(input_sequence);
    result = test_collection(c, mock, true);
    if(!quiet) {
        for(string err : result) { cout << err << endl; }
    }
    return result;
}

// --------------------------------------------------------------------------
// Check remainders
TEST_CASE("constructor", "creation") {
   MockCollection mock;
   vector<string> res;

   // Note: ATM a collection must at least have one frame.
   Collection empty = Collection();
   empty.add(1);

   res = test_collection(
       empty,
       get_mock("", "", "[1]", "  1:1 1 0 ",
           1, 0, 1, 1)
   );
   CHECK(res.size() == 0);

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

   auto c = Collection("head.", ".tail", 1, 5, 4);
   CHECK(c.getFrame(4) == "head.0004.tail");
   //CHECK(c.getFrame(90));

   //res = test_collection(
   //    Collection("head.", ".tail", -1, 5, 4),
   //    get_mock("head.", ".tail", "head.[-0001:0005].tail", "head. .tail -0001:0005 -0001:0005 4 ",
   //        7, 4, -1, 5)
   //);
   //CHECK(res.size() == 0);


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

   CHECK(edited.getFrame(10) == "head.10.tail");

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
TEST_CASE("assembling single files", "assemble") {
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

//// --------------------------------------------------------------------------
//// Check collections
//TEST_CASE("assembling collections", "[assemble]") {
//  vector<sequence::Collection> collections;
//  vector<std::string> remainders;
//  vector<std::string> entries;
//
//  entries.push_back("aaa.1.ext");
//  entries.push_back("aaa.2.ext");
//  entries.push_back("aaa.3.ext");
//  entries.push_back("aaa.4.ext");
//  entries.push_back("bbb.001.ext");
//  entries.push_back("bbb.002.ext");
//  entries.push_back("bbb.005.ext");
//
//  tie(collections, remainders) = sequence::assemble(entries);
//  REQUIRE(remainders.size() == 0);
//  REQUIRE(collections.size() == 2);
//
//  sequence::Collection aaa = collections.at(0);
//  sequence::Collection bbb = collections.at(1);
//
//  cout << aaa.format() << endl;
//  REQUIRE(aaa.format() == "aaa.[1:4].ext");
//  REQUIRE(aaa.count() == 4);
//  REQUIRE(aaa.head() == "aaa.");
//  REQUIRE(aaa.tail() == ".ext");
//
//  cout << bbb.format() << endl;
//  REQUIRE(bbb.format() == "bbb.[001:002,005].ext");
//  REQUIRE(bbb.count() == 3);
//  REQUIRE(bbb.padding() == 3);
//  REQUIRE(bbb.head() == "bbb.");
//  REQUIRE(bbb.tail() == ".ext");
//}

// --------------------------------------------------------------------------
// Parsing
//TEST_CASE("parsing fails if not collection", "[parsing]") {
//  std::string entry;
//
//  entry = "my_file.ext";
//  try {
//      Collection collection = sequence::parse(entry);
//  } 
//  catch (const sequence::parse_exception& e) {
//    REQUIRE(true);
//  }
//  catch (...) {
//    cout << "Unhandled exception" << endl;
//    REQUIRE(false);
//  }
//}

TEST_CASE("Parsing successs with various correct formats", "[parsing]"){
    MockCollection mock;
    vector<string> res;

    res = test_parser(
        "head.%d.tail [0:9]",
        get_mock("head.", ".tail", "head.[0:9].tail", "head. .tail 0:9 0:9 0 ",
            10, 0, 0, 9)
    );
    CHECK(res.size() == 0);

    // --------------------------------------------------------
    // Paddings
    res = test_parser(
        "head.%04d.tail [1:100]",
        get_mock("head.", ".tail", "head.[0001:0100].tail", "head. .tail 0001:0100 0001:0100 4 ",
            100, 4, 1, 100)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%d.tail [1:100]",
        get_mock("head.", ".tail", "head.[1:100].tail", "head. .tail 1:100 1:100 0 ",
            100, 0, 1, 100)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.####.tail [1:10]",
        get_mock("head.", ".tail", "head.[0001:0010].tail", "head. .tail 0001:0010 0001:0010 4 ",
            10, 4, 1, 10)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%1d.tail [1:10]",
        get_mock("head.", ".tail", "head.[1:10].tail", "head. .tail 1:10 1:10 1 ",
            10, 1, 1, 10)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head%d.tail [1:10]",
        get_mock("head", ".tail", "head[1:10].tail", "head .tail 1:10 1:10 0 ",
            10, 0, 1, 10)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%dtail [1:10]",
        get_mock("head.", "tail", "head.[1:10]tail", "head. tail 1:10 1:10 0 ",
            10, 0, 1, 10)
    );
    CHECK(res.size() == 0);

    // --------------------------------------------------------
    // Incomplete sequence
    res = test_parser(
        "head.%d.tail [1:10,20:30,40:50]",
        get_mock("head.", ".tail", "head.[1:10,20:30,40:50].tail", "head. .tail 1:50 1:10,20:30,40:50 0 11,12,13,14,15,16,17,18,19,31,32,33,34,35,36,37,38,39",
            32, 0, 1, 50)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%d.tail [1, 4,5]",
        get_mock("head.", ".tail", "head.[1:4x3,5].tail", "head. .tail 1:5 1:4x3,5 0 2,3",
            3, 0, 1, 5)
    );
    CHECK(res.size() == 0);

    // --------------------------------------------------------
    // Incomplete sequences with regular steps
    res = test_parser(
        "head.%d.tail [1 ,3, 5]",
        get_mock("head.", ".tail", "head.[1:5x2].tail", "head. .tail 1:5 1:5x2 0 2,4",
            3, 0, 1, 5)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%d.tail [10,15,20,25]",
        get_mock("head.", ".tail", "head.[10:25x5].tail", "head. .tail 10:25 10:25x5 0 \
11,12,13,14,16,17,18,19,21,22,23,24",
            4, 0, 10, 25)
    );
    CHECK(res.size() == 0);

    // --------------------------------------------------------
    // Parsing with a step
//    res = test_parser(
//        "head.%d.tail [10:25x2]",
//        get_mock("head.", ".tail", "head.[10:25x5].tail", "head. .tail 10:25 10:25x2 0 \
//11,12,13,14,16,17,18,19,21,22,23,24",
//        4, 0, 10, 25)
//    );
//    CHECK(res.size() == 0);

    // --------------------------------------------------------
    // Negative ranges
    res = test_parser(
        "head.%d.tail [-2:5]",
        get_mock("head.", ".tail", "head.[-2:5].tail", "head. .tail -2:5 -2:5 0 ",
            8, 0, -2, 5)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%d.tail [-5:-1]",
        get_mock("head.", ".tail", "head.[-5:-1].tail", "head. .tail -5:-1 -5:-1 0 ",
            5, 0, -5, -1)
    );
    CHECK(res.size() == 0);

    res = test_parser(
        "head.%d.tail [-5, -3, -1]",
        get_mock("head.", ".tail", "head.[-5:-1x2].tail", "head. .tail -5:-1 -5:-1x2 0 -4,-2",
            3, 0, -5, -1)
    );
    CHECK(res.size() == 0);

    //res = test_parser(
    //    "head.###.tail [-2:2]",
    //    get_mock("head.", ".tail", "head.[-002:002].tail", "head. .tail -002:002 -002:002 3 ",
    //        5, 3, -2, 2)
    //);
    //CHECK(res.size() == 0);


    // REQUIRE(sequence::parse("frame.%d.ext [1-10,20-30,40-50]").format() == "frame.[1:10,20:30,40:50].ext");
    // REQUIRE(sequence::parse("frame.%d.ext [10,20,30,40,50]").format() == "frame.[10:50x10].ext");
    // REQUIRE(sequence::parse("frame.%d.ext [10,25,32,41]").format() == "frame.[10,25,32,41].ext");
    // REQUIRE(sequence::parse("frame.%d.ext [1-10,25,41]").format() == "frame.[1:10,25,41].ext");

    // REQUIRE(sequence::parse("frame%d [1-10]").format() == "frame[1:10]");
    // REQUIRE(sequence::parse("%d.ext [1-10]").format() == "[1:10].ext");

    // cout << endl;

}

// TEST_CASE("parsing success", "[parsing]") {
//   std::string entry;

//   entry = "my_file.%04d.ext [1-10]";
//   try {
//     Collection collection = sequence::parse(entry);
//     cout << "result is: " << collection.format() << endl;
//     REQUIRE(collection.count() == 10);
//     REQUIRE(collection.first() == 1);
//     REQUIRE(collection.last() == 10);
//     REQUIRE(collection.head() == "my_file.");
//     REQUIRE(collection.tail() == ".ext");
//   } 
//   catch (const sequence::parse_exception& e) {
//     REQUIRE(false);
//   }
//   catch (...) {
//     cout << "Unhandled exception" << endl;
//     REQUIRE(false);
//   }

//   entry = "my_file.%4d.ext [11-20]";
//   try {
//     Collection collection = sequence::parse(entry);
//     cout << "result is: " << collection.format() << endl;

//     REQUIRE(collection.count() == 10);
//     REQUIRE(collection.first() == 11);
//     REQUIRE(collection.last() == 20);
//     REQUIRE(collection.head() == "my_file.");
//     REQUIRE(collection.tail() == ".ext");
//   } 
//   catch (const sequence::parse_exception& e) {
//     REQUIRE(false);
//   }
//   catch (...) {
//     cout << "Unhandled exception" << endl;
//     REQUIRE(false);
//   }


//   REQUIRE(true);
// }

//TEST_CASE("parsing failures on various wrong collections", "[parsing]") {
//  std::string entry;
//
//  entry = "my_file.%04.ext [1-10]";
//  try {
//    Collection collection = sequence::parse(entry);
//  } 
//  catch (const sequence::parse_exception& e) {
//    REQUIRE(true);
//    cout << "fail to parse: " << entry << endl;
//  }
//  catch (...) {
//    cout << "Unhandled exception" << endl;
//    REQUIRE(false);
//  }
//
//  REQUIRE(true);
//}