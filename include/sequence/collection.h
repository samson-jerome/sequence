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
#include <set>

#include "fmt/format.h"

//#include "sequence.h"

using namespace fmt::literals;
using std::cout;
using std::endl;

namespace sequence {

    class out_of_range_error : public std::runtime_error {
    public:
        out_of_range_error(const std::string msg) : std::runtime_error(msg) {};
    };


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
        
        std::set<int> m_indices;    //< set of numerical indexes
        std::string m_head = "";    //< head part of the collection
        std::string m_tail = "";    //< tail part of the collection
        int m_padding = 0;          //< size of zero-padded indexes
        //std::string m_frame_separator{ sequence::frame_separator };   // @audit verify that we're passing the value and not the address of the original literal
        //std::string m_step_separator{ step_separator };
        //std::string m_range_separator{ range_separator  };
        std::string m_frame_separator = ":";
        std::string m_step_separator = "x";
        std::string m_range_separator = ",";
        std::vector<int> m_holes;
        std::deque<Range> m_ranges;

        // std::string m_cached_format;
        // std::string m_cached_holes;

    public:
        Collection();
        Collection(const sequence::Collection &c);
        //Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes);
        Collection(const std::string& head, const std::string& tail, const std::set<int>& indexes);
        Collection(const std::string& head, const std::string& tail, const int start, const int end);
        //Collection(const std::string &head,const std::string &tail, const std::vector<int> &indexes, const int padding);
        Collection(const std::string& head, const std::string& tail, const std::set<int>& indexes, const int padding);
        Collection(const std::string& head, const std::string& tail, const int start, const int end, const int padding);

        // =====================================================================
        // Items accessors
        std::string firstItem();
        std::string lastItem();
        void addItem(std::string item);
        void addItem(std::vector<std::string> items_list);
        void removeItem(std::string item);
        void removeItem(std::vector<std::string> items_list);

        std::pair<std::string, bool> getItem(int frame) const;
        std::pair<std::string, bool> getNthItem(int index) const;

        // =====================================================================
        // Frame accessors
        int first();
        int last();
        void add(int frame);
        void add(std::vector<int> frames_list);
        void remove(int frame);
        void remove(std::vector<int> frames_list);

        // =====================================================================
        // Global accessors
        std::vector<std::string> items() const;
        std::vector<int> frames() const;
        std::vector<int> holes() const;
        int count();
        void clear();
        // std::string toString(const std::string &format=DEFAULT_FORMAT);
        // std::string format(const std::string &format=Collection::DEFAULT_FORMAT);

        /**
         * \brief Format description
         */
        // std::string format();
        std::string format(std::string pattern = "{head}[{ranges}]{tail}");
        std::string getFrame(int frame);
        void info();
        // void print(string format="");
        // void print(CollectionFormats format=Format.natural);
        // void print(stream, format=CollectionFormats.default);

        // =====================================================================
        // Structure accessors
        std::string head() const;
        void setHead(std::string);

        std::string tail() const;
        void setTail(std::string);

        int padding() const;
        void setPadding(int);

    protected:
        void _findHoles();
        void _separate(int minimum_items=1);

    };
}
#endif // COLLECTION_H