#ifndef SEQUENCE_H
#define SEQUENCE_H
#pragma once

#define SEQUENCE_VER_MAJOR 0
#define SEQUENCE_VER_MINOR 2
#define SEQUENCE_VER_PATCH 1

#define SEQUENCE_VERSION (SEQUENCE_VER_MAJOR * 10000 + SEQUENCE_VER_MINOR * 100 + SEQUENCE_VER_PATCH)

#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map> 
#include <regex>

#include "collection.h"

using std::vector;
using std::string;
using std::tuple;

namespace sequence {

    constexpr char frame_separator = ':';
    constexpr char step_separator = 'x';
    constexpr char range_separator = ',';

    // TODO:
    // - allow additional regex pattern
    // - set minimum items arg to match or not single number sequences
    class parse_exception : public std::runtime_error {
    public:
        parse_exception(const string msg) : std::runtime_error(msg) {};
    };

    tuple<vector<Collection>, vector<string>> assemble(vector<string> entries, int minimum_items=1);
    // bool assemble(vector<string>, vector<Collection>* collections, vector<string>* remainders);

    Collection parse(string entry, string pattern = "");
}
#endif // SEQUENCE_H