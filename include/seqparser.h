#ifndef SEQPARSER_H
#define SEQPARSER_H

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map> 
#include <regex>

#include <boost/filesystem.hpp>

#include "collection.h"

using namespace boost::filesystem;
using namespace std;

// #include <QStringList>
// #include <QList>
// #include <QMultiHash>
// #include <QHash>
// #include <QRegularExpression>
// #include <QRegularExpressionMatch>
// class directory_iterator;
// class Collection;

int give_me_one();

class SequenceParser {

public:
    SequenceParser();
    
    // void assemble(directory_iterator, std::vector<Collection>* collections, std::vector<std::string>* remainders);
    // void assemble(std::vector<std::string>, std::vector<Collection>* collections, std::vector<std::string>* remainders);
    static tuple<vector<Collection>, vector<string>> assemble(vector<string> entries);

    // void assemble(QStringList entries);
    // std::tuple<std::vector<Collection>, std::vector<std::string>> assemble(QStringList entries);
    // static std::tuple<QList<Collection>, QStringList> assemble(QStringList entries);

};

#endif // SEQPARSER_H
