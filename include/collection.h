/***
 *
 */
#ifndef COLLECTION_H
#define COLLECTION_H

#include <string>
#include <iostream>
#include <vector>
#include <deque>

#include "fmt/format.h"

using namespace fmt::literals;

namespace sequence {

    /**
     * A contigious section of frames.
     */
    typedef struct Range{
        int start = 0;
        int end = 0;
        int step = 1;
        bool isSingleFrame=false;
    } Range;

    // Predefined string sequence formatting
    typedef struct CollectionFormats {
        std::string buf = "{head}[{ranges}]{tail}";
        std::string rv = "{head}{#}{tail} {ranges}";
        std::string percent = "{head}{%04d}{tail} {ranges}";

    } CollectionFormats;

    /**
     * A class holding information about a collection of names (usually file names) with a numerical token
     * Each element of the collection consist of the head, a numerical index and a tail.
     */
    class Collection {
        // TODO 
        //  - implement iterator interface (std?)
        //  - add formatting support
        //  - add operators: + - <<
        //  - add insertion/removal
        //  - add cache format and holes and invalidation mecanism
        //  - support variable padding

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