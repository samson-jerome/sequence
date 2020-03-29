#ifndef COLLECTION_H
#define COLLECTION_H

#include <string>
#include <iostream>
#include <vector>
#include <deque>

#include <boost/algorithm/string/join.hpp>
#include "fmt/format.h"

// enum pattern {}
using namespace fmt::literals;
using namespace std;

namespace sequence {

    typedef struct Range{
        int start = 0;
        int end = 0;
        int step = 1;
        bool isSingleFrame=false;
    } Range;


    typedef struct CollectionFormats {
        string buf = "{head}[{ranges}]{tail}";
        string rv = "{head}{#}{tail} {ranges}";
        string percent = "{head}{%04d}{tail} {ranges}";

    } CollectionFormats;


    class Collection {
        // TODO 
        //  - implement iterator interface (Boost or std?)
        //  - add formatting support
        //  - add operators: + - <<
        //  - add cache format and holes and invalidation mecanism

        // std::string DEFAULT_FORMAT = "{head}{ranges}{tail}"
        
        std::vector<int> m_indexes;
        std::string m_head;
        std::string m_tail;
        int m_padding;

        std::vector<int> m_holes;
        std::deque<Range> m_ranges;

        // std::string m_cached_format;
        // std::string m_cached_holes;

    public:
        Collection();
        Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes);
        Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes, const int padding);

        // std::string toString(const std::string &format=DEFAULT_FORMAT);
        // std::string format(const std::string &format=Collection::DEFAULT_FORMAT);
        int count();
        int first();
        int last();
        void insert(int);

        std::string format();
        void info();

        std::string head() const;
        void setHead(std::string);

        std::string tail() const;
        void setTail(std::string);

        int padding() const;
        void setPadding(int);

        std::vector<std::string> getFileList() const;
    protected:
        void _findHoles();
        // void _findRanges();
        void _separate();

    };
}
#endif // COLLECTION_H