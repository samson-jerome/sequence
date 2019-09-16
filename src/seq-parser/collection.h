#ifndef COLLECTION_H
#define COLLECTION_H

#include <string>
#include <iostream>
#include <vector>
#include <deque>

#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>

// enum pattern {}
using namespace fmt::literals;
using namespace std;

typedef struct Range{
    int start = 0;
    int end = 0;
    int step = 1;
    bool isSingleFrame=false;
} Range;


class Collection {
    // TODO 
    //  - implement iterator interface (Boost or std?)
    //  - add formatting support

    // std::string DEFAULT_FORMAT = "{head}{ranges}{tail}"
    
    std::vector<int> m_indexes;
    std::string m_head;
    std::string m_tail;

    std::vector<int> m_holes;
    std::deque<Range> m_ranges;

public:
    Collection();
    Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes);

    // std::string toString(const std::string &format=DEFAULT_FORMAT);
    // std::string format(const std::string &format=Collection::DEFAULT_FORMAT);
    int count();
    int first();
    int last();
    std::string format();
    void info();

    void setHead(std::string);
    std::string getHead() const;

    std::vector<std::string> getFileList() const;
protected:
    void _findHoles();
    // void _findRanges();
    void _separate();


};

#endif // COLLECTION_H