#include <regex> 
#include <sstream>

#include "sequence/collection.h"
#include "sequence/sequence.h"

using std::string;
using std::cout;
using std::endl;
using std::set;
using std::vector;
using std::unordered_multimap;
using std::pair;

using namespace sequence;

/**
Splits a string using a given delimiter
Returns a vector of strings
*/
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


/**
Parses a list of string to identify sequences with numerical indices.
Returns a tuple with all collections found and the remainder singles entrie.
*/
std::tuple<vector<Collection>, vector<std::string>> sequence::assemble(vector<std::string> entries, int minimum_items) {

    vector<Collection> collections;
    vector<string> remainders;

    // TODO probably better to avoid multimap here, see ref:
    // http://www.yosoygames.com.ar/wp/2014/05/the-sorted-vector-pattern-part-i/

    std::regex re("(-*\\d+)(\\D*)$");
    std::smatch match;

    string head, tail, str_index, hash;
    set<string> keys;
    unordered_multimap<string, string> collections_ummap;
    typedef unordered_multimap<string, string>::iterator ummap_it;
    typedef set<string>::iterator keysIt;

    for (int i = 0; i < entries.size(); ++i) {

        // cout << "- entry: " << entries.at(i) << endl;    

        if (regex_search(entries.at(i), match, re) && match.size() > 1) {
            // cout << "   - matches: " << match.size();

            head = entries.at(i).substr(0, match.position(0));
            str_index = match.str(1);
            tail = match.str(2);
            hash = tail + "|" + head;  // convention to store unicity of a sequence
            keys.insert(hash); // should we tets existence here ?

            // Limit size of int, sequence can't have more than ~2B frame
            // as a global range
            collections_ummap.insert(make_pair(hash, str_index));
        }
        else {
            remainders.push_back(entries.at(i));
        }
    }

    string hash_head="", hash_tail="";
    vector<int> indexes;
    vector<string> str_indexes;
    int padding;
    std::size_t pos;
    string delims = "|";
    for (keysIt key = keys.begin(); key != keys.end(); ++key)
    {
        pos = (*key).find(delims);
        hash_tail = (*key).substr(0, pos);
        hash_head = (*key).substr(pos + 1);

        // Collect indexes for current hash key i.e. current sequence
        padding = 0;
        indexes.clear();
        str_indexes.clear();
        pair<ummap_it, ummap_it> result = collections_ummap.equal_range(*key);
        for (ummap_it it = result.first; it != result.second; it++) {
            str_indexes.push_back(it->second);
            indexes.push_back(stoi(it->second));

            if (it->second[0] == '0' && padding < it->second.size())
                padding = it->second.size();
        }

        // TODO insert in correct pos instead of brute force sorting
        // TODO use pairs to store int and str if different or 
        // if varying padding?
        sort(indexes.begin(), indexes.end());
        sort(str_indexes.begin(), str_indexes.end());

        // Identify hash entries with a single index to handle as a remainder instead of collection
        // TODO use minimum_items to qualify if this is a remainder or collection
        if (distance(result.first, result.second) == 1) {
            auto singleFile = hash_head + result.first->second + hash_tail;
            remainders.push_back(singleFile);
            continue;
        }

        // Create the collection for the current hash key
        Collection currentCollection(
            hash_head,
            hash_tail,
            std::set<int>(indexes.begin(), indexes.end()),
            padding
            );

            collections.push_back(currentCollection);
        }

    return make_tuple(collections, remainders);
}


// ^(\w.*)%(\d+)d(.*) \[(\d+)\-(\d+)\]$
// '{head}{padding}{tail} [{ranges}]'
// @todo parse against multiple patterns > tricky as regex cannot get named groups, would bring a lot of conditionnal
// @todo support parsing when step <> 1 i.e. "1:100x2"

// Currently supporting:
// frame.%04d.exr [1050-1080]
// frame.###.exr [1050-1080]
// frame.%d.exr [1050-1080]
// frame.%d.exr[1050-1080]
// frame.[1050- 1080].exr
// frame.%03d.exr[1050-1080, 1100, 1105-1110]
// frame.%03d.exr[1050, 1080, 1090]
Collection sequence::parse(string entry, string pattern) {

    // @audit how can we use the constants in sequence.h into the regex
    string head_pattern = "([^\\r\\n\\t\\f\\v \\(\\)\\{\\}\\[\\]#]*)";
    string padding_pattern = "(%(\\d+)d|(%d)|(#+))";
    string tail_pattern = "(\\S+) *";
    string ranges_pattern = "\\[([\\d\\, \\ \\-\\:]+)?\\]";

    string regex_pattern = fmt::format("^{0}{1}{2}{3}$", head_pattern, padding_pattern, tail_pattern, ranges_pattern);
    if (pattern != "") {
        // Build pattern from a set of allowed tokens: 
        // {head}{padding}{ext}{global_range}{ranges}{holes}
    }

    // cout << "DBG: entry = " << entry << endl;
    // cout << "DBG: pattern = " << pattern << endl;
    // cout << "DBG: regex_pattern = " << regex_pattern << endl;
    std::regex re(regex_pattern);
    std::smatch match;

    if (regex_search(entry, match, re) && match.size() > 1) {
        string head, tail, range;
        int num_padding, start, end;
        string d_padding, no_padding, hash_padding, ranges;
        set<int> raw_indexes;
        vector<string> ranges_list;

        head = match.str(1);
        d_padding = match.str(3);       // the padding value as an int: "%4d" > 4
        no_padding = match.str(4);      // the string %d if found
        hash_padding = match.str(5);    // the hash padded string if found
        tail = match.str(6);
        ranges = match.str(7);          // a list of ranges if found: n-m,x-z...

        // cout << "DBG: head = " << head << endl;
        // cout << "DBG: padding = " << d_padding << endl;
        // cout << "DBG: no_padding = " << no_padding << endl;
        // cout << "DBG: hash_padding = " << hash_padding << endl;
        // cout << "DBG: tail = " << tail << endl;
        // cout << "DBG: ranges = " << ranges << endl;

        if(d_padding != "") {
            num_padding = stoi(d_padding);
        }
        else if (hash_padding != "") {
            num_padding = hash_padding.size();
        }
        else {
            num_padding = 0;
        }

        if(ranges != "") {
            // ranges can be: 
            //    1-5,10,20-25...
            //    1,2,5...
            //    1-5
            ranges_list = split(ranges, sequence::range_separator);
            for (auto r : ranges_list) {
                try {
                    // We will assume the ranges are valid (correct numbers, no extra delim)
                    // otherwise parse_exception is thrown

                    size_t delim_found = r.find(sequence::frame_separator);

                    // Is this a range or single frame
                    if (delim_found != string::npos) {

                        // Is there any other delimiter left in the range
                        //if (r.find(sequence::frame_separator, delim_found + 1) != string::npos)
                        //    throw parse_exception("invalid range syntax");

                        // Range is valid, loop to add all numbers
                        int start = stoi(r.substr(0, delim_found));
                        int end = stoi(r.substr(delim_found + 1));
                        for (int i = start; i <= end; i++) {
                            raw_indexes.insert(i);
                        }
                    }
                    else {
                        // this is a single frame
                        raw_indexes.insert(stoi(r));
                    }
                }
                catch (...) {
                    throw parse_exception(fmt::format("invalid range syntax: {}", r));
                }
            }
        }

        Collection coll(head, tail, raw_indexes, num_padding);
        return coll;
    }
    else {
        throw parse_exception("sequence pattern not matched");
    }
}