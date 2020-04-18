#pragma once
/***
 * \file
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
     * \brief A contiguous section of frames.
     */
    typedef struct Range{
        int start = 0;
        int end = 0;
        int step = 1;
        bool isSingleFrame=false;
    } Range;

    /**
     * \brief Predefined string sequence formatting
     */
    typedef struct CollectionFormats {
        std::string defaut = "{head}[{ranges}]{tail}";
        std::string rv = "{head}{#}{tail} {ranges}";
        std::string clique = "{head}{#}{tail} {ranges}";
        std::string percent = "{head}{%d}{tail} {ranges}";

    } CollectionFormats;

    namespace Format {
        enum Format {natural, dash, clique, percent};
    };

    /**
     * \class Collection
     * 
     * \brief 
     * A class holding information about a collection of names with a numerical 
     * token
     * 
     * Each element of the collection consist of the head, a numerical index and
     * a tail.
     */
    class Collection {
        // @todo 
        //  - implement iterator interface (std?)
        //  - add formatting support
        //  - add operators: + - <<
        //  - add insertion/removal
        //  - add cache format and holes and invalidation mecanism
        //  - support variable padding

        // std::string DEFAULT_FORMAT = "{head}{ranges}{tail}"
        
        std::vector<int> m_indexes; //< vector of numerical indexes
        std::string m_head;         //< head part of the collection
        std::string m_tail;         //< tail part of the collection
        int m_padding;              //< size of zero-padded indexes

        std::vector<int> m_holes;
        std::deque<Range> m_ranges;

        // std::string m_cached_format;
        // std::string m_cached_holes;

    public:
        //Collection();
        Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes);
        Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes, const int padding);

        // std::string toString(const std::string &format=DEFAULT_FORMAT);
        // std::string format(const std::string &format=Collection::DEFAULT_FORMAT);
        int count();
        int first();
        int last();
        void insert(int frame);
		void insert(std::vector<int> frames_list);
		void remove(int frame);
		void remove(std::vector<int> frames_list);
		void update(int frame);
		void update(std::vector<int> frames_list);

        std::string format();
        void info();
        // void print(string format="");
        // void print(CollectionFormats format=Format.natural);
        // void print(stream, format=CollectionFormats.default);
        

        std::string head() const;
        void setHead(std::string);

        std::string tail() const;
        void setTail(std::string);

        int padding() const;
        void setPadding(int);


        std::vector<std::string> getItems() const;
        std::string getItem(int index) const;

    protected:
        void _findHoles();
        // void _findRanges();
        void _separate();

    };
}
#endif // COLLECTION_H