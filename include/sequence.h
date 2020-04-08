#ifndef SEQUENCE_H
#define SEQUENCE_H
#pragma once

#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map> 
#include <regex>

//#include <boost/filesystem.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/split.hpp>

#include "collection.h"

//using namespace boost::filesystem;
using std::vector;
using std::string;
using std::tuple;

namespace sequence {
    // TODO:
    // - allow additional regex pattern
    // - set minimum items arg to match or not single number sequences
    // - add parse method to construct a collection from a string pattern
    // - tests

    tuple<vector<Collection>, vector<string>> assemble(vector<string> entries, int minimum_items=1);
    // bool assemble(vector<string>, vector<Collection>* collections, vector<string>* remainders);

    Collection parse(string value);
    // Collection parse(string value, string pattern);

    class parse_exception : public std::runtime_error {
    public:
        parse_exception(const string msg) : std::runtime_error(msg) {};
    };
}

#endif // SEQUENCE_H
