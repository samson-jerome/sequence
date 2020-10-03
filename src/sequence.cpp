#include <regex> 

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
            indexes,
            padding
            );

            collections.push_back(currentCollection);
        }

    return make_tuple(collections, remainders);
}


// ^(\w.*)%(\d+)d(.*) \[(\d+)\-(\d+)\]$
// '{head}{padding}{tail} [{ranges}]'
// @todo parse against multiple patterns
Collection sequence::parse(string entry, string pattern) {

    //(^ (\D.*) % (\d + )d(.*) \[(\d + )\ - (\d + )\]$ | ^ (\D.*) % (\d + )d(.*)$)
    //std::regex re("^(\\D.*)%(\\d+)d(.*) \\[(\\d+)\\-(\\d+)\\]$");
    vector<string> regex_list;
    string head_pattern = "([^\\r\\n\\t\\f\\v \\(\\)\\{\\}\\[\\]] *)";
    string padding_pattern = "(%(\\d+)d|%d|(#+))";
    string tail_pattern = "(\S+) *";
    string global_range_pattern = "(\\[(\\d+)\\-(\\d+)\\])?";
    string ranges_pattern = "\\[(\\d+)\\-(\\d+)\\]";
    string holes_pattern = "\\[(\\d+)\\-(\\d+)\\]";
    //(? P<ranges>[\d, \ - ] + ) ?
    //regex_list.push_back("^(\\D.*)%(\\d+)d(.*) \\[(\\d+)\\-(\\d+)\\]$");
    //regex_list.push_back("^(\\D.*)(#*)(.*) \\[(\\d+)\\-(\\d+)\\]$");

    string regex_pattern = "^(\\D.*)%(\\d+)d(.*) \\[(\\d+)\\-(\\d+)\\]$";
    if (pattern != "") {
        // Build pattern from a set of allowed tokens: 
        // {head}{padding}{ext}{global_range}{ranges}{holes}
    }

    cout << "DBG: entry = " << entry << endl;
    cout << "DBG: pattern = " << pattern << endl;
    cout << "DBG: regex_pattern = " << regex_pattern << endl;
    std::regex re(regex_pattern);
    std::smatch match;
    string head, tail;
    int start, end, padding;

    if (regex_search(entry, match, re) && match.size() > 1) {
        // cout << "matches = " << match.size() << endl;
        // cout << "matches pos = ";
        // for (int i=0; i<match.size()-1; i++) cout << match.position(i) << ", ";
        // cout << match.position(match.size()) << endl;

        //head = entry.substr(match.position(1), match.position(2) - 1);
        head = match.str(1);
        padding = stoi(match.str(2));
        tail = match.str(3);
        start = stoi(match.str(4));
        end = stoi(match.str(5));

        vector<int> indexes;
        for (int i = start; i <= end; i++) {
            indexes.push_back(i);
        }

        Collection coll(head, tail, indexes, padding);
        return coll;
    }
    else {
        throw parse_exception("sequence pattern not matched");
    }
}