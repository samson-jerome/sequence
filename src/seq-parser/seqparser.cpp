#include <regex> 

#include "collection.h"
#include "seqparser.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

using namespace sequence;

int sequence::give_me_one() {
    Range r;
    cout << "Range: " << r.isSingleFrame << endl;

    Collection c;
    cout << "Collection: " << c.format() << endl;

    cout << "surprise !" << endl;
    return 1;
}


std::tuple<vector<Collection>, vector<string>> sequence::assemble(vector<string> entries) {

    vector<Collection> collections;
    vector<string> remainders;

    // Filter hidden
    // Filter folders, files, symlink and special items

    // TODO probably better to avoid multimap here, see ref:
    // http://www.yosoygames.com.ar/wp/2014/05/the-sorted-vector-pattern-part-i/

    regex re("(-*\\d+)(\\D*)$"); 
    smatch match; 

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

    vector<string> splits;
    vector<int> indexes;
    vector<string> str_indexes;
    string delims = "|";
    for (keysIt key=keys.begin(); key!=keys.end(); ++key)
    {
        boost::split(splits, *key, boost::is_any_of(delims));

        // Collect indexes for current hash key i.e. current sequence
        indexes.clear();
        str_indexes.clear();
        pair<ummap_it, ummap_it> result = collections_ummap.equal_range(*key);
        for (ummap_it it = result.first; it != result.second; it++) {
            str_indexes.push_back(it->second);
            indexes.push_back(stoi(it->second));
        }
        sort(indexes.begin(), indexes.end());
        sort(str_indexes.begin(), str_indexes.end());
        // TODO get padding

        // Identify hash entries with a single index to handle as a remainder instead of collection
        if (distance(result.first, result.second)==1) {
            auto singleFile = splits.at(1) + result.first->second + splits.at(0);
            remainders.push_back(singleFile);
            continue;
        }

        // Create the collection for the current hash key
        Collection currentCollection( 
            splits.at(1), 
            splits.at(0), 
            indexes
        );

        collections.push_back(currentCollection);
    }

    return make_tuple(collections, remainders);
}
