// #include "catch.hpp"

// #include <map>
// #include <string>

// #include "sequence/sequence.h"
// #include "sequence/collection.h"

// #include "tests_utils.h"

// using std::cout;
// using std::endl;
// using std::string;
// using std::map;
// using std::pair;
// using std::vector;

// struct MockCollection;

// MockCollection get_mock(string head, string tail, string format, 
//     string full_format, int count, int padding, int first, int last) 
// {
//     MockCollection result;
//     result.head = head;
//     result.tail = tail;
//     result.format = format;
//     result.full_format = full_format;

//     result.count = count;
//     result.padding = padding;
//     result.first = first;
//     result.last = last;

//     return result;
// }

// vector<string> test_collection(Collection c, MockCollection mock, bool quiet=false) {

//     string full_format = "{head} {tail} {global_range} {ranges} {padding} {holes}";
//     vector<string> result;

//     if(!quiet) {
//         cout << "TESTING " << c.format() << endl;
//     }

//     if(c.head() != mock.head)
//         result.push_back("Error in collection::head: \"" + c.head() + "\" <> \"" + mock.head + "\"");
//     if(c.tail() != mock.tail)
//         result.push_back("Error in collection::tail: \"" + c.tail() + "\" <> \"" + mock.tail + "\"");

//     if( c.count() != mock.count)
//         result.push_back("Error in collection::count: \"" \
//             + std::to_string(c.count()) + "\" <> \"" \
//             + std::to_string(mock.count) + "\"");
//     if(c.padding() != mock.padding)
//         result.push_back("Error in collection::padding: \"" \
//             + std::to_string(c.padding()) + "\" <> \"" \
//             + std::to_string(mock.padding) + "\"");
//     if(c.first() != mock.first)
//         result.push_back("Error in collection::first: \"" \
//             + std::to_string(c.first()) + "\" <> \"" \
//             + std::to_string(mock.first) + "\"");
//     if(c.last() != mock.last)
//         result.push_back("Error in collection::last: \"" \
//             + std::to_string(c.last()) + "\" <> \"" \
//             + std::to_string(mock.last) + "\"");

//     if(c.format() != mock.format)
//         result.push_back("Error in collection::format: \"" + c.format() + "\" <> \"" + mock.format + "\"");
//     if(c.format(full_format) != mock.full_format)
//         result.push_back("Error in collection::full_format: \"" + c.format(full_format) + "\" <> \"" + mock.full_format + "\"");

//     if(!quiet) {
//         for(string err : result) { cout << err << endl; }
//     }
//     return result;
// }


// vector<string> test_parser(string input_sequence, MockCollection mock, bool quiet=false) {
//     vector<string> result;

//     if(!quiet) {
//         cout << "PARSING " << input_sequence << endl;
//     }

//     Collection c = sequence::parse(input_sequence);
//     result = test_collection(c, mock, true);
//     if(!quiet) {
//         for(string err : result) { cout << err << endl; }
//     }
//     return result;
// }
