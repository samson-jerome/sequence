#include <regex> 

#include "collection.h"
#include "seqparser.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

int give_me_one() {
    Range r;
    cout << "Range: " << r.isSingleFrame << endl;

    Collection c;
    cout << "Collection: " << c.format() << endl;

    cout << "surprise !" << endl;
    return 1;
}

SequenceParser::SequenceParser() {
    cout << "SequenceParser" << endl;
}

// void SequenceParser::assemble(directory_iterator d, vector<Collection>* collections, vector<std::string>* remainders) {
//     cout << "SequenceParser::assemble (iterator)" << endl;
// }

// void SequenceParser::assemble(vector<string> entries, vector<Collection>* collections, vector<std::string>* remainders) {
//     cout << "SequenceParser::assemble (vector of entries)" << endl;
// }

std::tuple<vector<Collection>, vector<string>> SequenceParser::assemble(vector<string> entries) {
    cout << "tuple of collection and remainders > SequenceParser::assemble (vector of entries)" << endl;

    vector<Collection> collections;
    vector<string> remainders;

    // Filter hidden
    // Filter folders, files, symlink and special items

    // TODO probably better to avoid multimap here, see following ref
    // http://www.yosoygames.com.ar/wp/2014/05/the-sorted-vector-pattern-part-i/

    regex re("(-*\\d+)(\\D*)$"); 
    smatch match; 

    string head, tail, index, hash;
    set<string> keys;
    unordered_multimap<string, int> collections_ummap;
    typedef unordered_multimap<string, int>::iterator ummap_it;
    typedef set<string>::iterator keysIt;

    for (int i = 0; i < entries.size(); ++i) {
        
        // cout << "- entry: " << entries.at(i) << endl;    

        if (regex_search(entries.at(i), match, re) && match.size() > 1) {
            // cout << "   - matches: " << match.size();
            
            head = entries.at(i).substr(0, match.position(0));
            index = match.str(1);
            tail = match.str(2);
            hash = tail + "|" + head;  // convention to store unicity of a sequence
            keys.insert(hash); // should we tets existence here ?

            // Limit size of int, sequence can't have more than ~2B frame
            // as a global range
            collections_ummap.insert(make_pair(hash, stoi(index)));
            // if(i < 20){
            //     std::cout << (entries.at(i)) << " --> " \
            //         << (hash) << " / " \
            //         << (head) << " / " \
            //         << (index) << " / " \
            //         << (tail) << std::endl;
            // }
            // else{
            //     if(i == 21) std::cout << "..." << std::endl;
            // }            
        }
        else {
            remainders.push_back(entries.at(i));
            // cout << entries.at(i) << " --> remainders" << endl;
        }
    }

    cout << "Sequence unique keys:" << endl;
    vector<string> splits;
    vector<int> indexes;
    string delims = "|";
    for (keysIt key=keys.begin(); key!=keys.end(); ++key)
    {
        cout << *key;
        boost::split(splits, *key, boost::is_any_of(delims));
    

        // Collect indexes for current hash key i.e. current sequence
        indexes.clear();
        pair<ummap_it, ummap_it> result = collections_ummap.equal_range(*key);
        for (ummap_it it = result.first; it != result.second; it++) {
            indexes.push_back(it->second);
        }
        sort(indexes.begin(), indexes.end());

        // Identify hash entries with a single index to handle as a remainder instead of collection
        if (distance(result.first, result.second)==1) {
            // FIXME bug with zero padding indexes
            auto singleFile = splits.at(1) + std::to_string(indexes[0]) + splits.at(0);
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
