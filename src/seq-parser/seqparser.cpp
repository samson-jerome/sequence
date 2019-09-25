#include <regex> 

#include "collection.h"
#include "seqparser.h"

using namespace std;
using namespace boost::filesystem;

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

    // Handle negative frames
    // QRegularExpression re("(?<index>-*\\d+)(?<tail>\\D+)$");
    // QRegularExpressionMatch match;

    // const regex re("(?<index>-*\\d+)(?<tail>\\D+)$"); 
    const regex re("(-*\\d+?)(\\D+?)$"); 
    smatch match; 
  
    // QMultiHash<QString, int> collectHash;
    // QString head, tail, index, hash;

    for (int i = 0; i < entries.size()-100; ++i) {

        cout << "- entry: " << entries.at(i) << endl;
    

        if (regex_match(entries.at(i), match, re)) {
            cout << entries.at(i) << endl;
            for (size_t i = 0; i < match.size(); ++i) {
                ssub_match sub_match = match[i];
                string piece = sub_match.str();
                cout << "  submatch " << i << ": " << piece << endl;
            }   
        }   
    }

    //     // match = re.match(entries.at(i));
    //     if (match.hasMatch()) {
    //         // std::cout << "Has match" << std::endl;
    //         head = entries.at(i).left(match.capturedStart("index"));
    //         index = match.captured("index");
    //         tail = match.captured("tail");
    //         hash = tail + "|" + head;  // convention to store unicity of a sequence
    
    //         // Limit size of int, sequence can't have more than ~2B frame
    //         // as a global range
    //         collectHash.insert(hash, index.toInt());

    //         // if(i < 20){
    //         //     std::cout << qPrintable(entries.at(i)) << " --> " \
    //         //         << qPrintable(hash) << " / " \
    //         //         << qPrintable(head) << " / " \
    //         //         << qPrintable(index) << " / " \
    //         //         << qPrintable(tail) << std::endl;
    //         // }
    //         // else{
    //         //     if(i == 21) std::cout << "..." << std::endl;
    //         // }
    //     }
    //     else {
    //         remainders.append(entries.at(i));
    //         // std::cout << qPrintable(entries.at(i)) << " --> remainders" << std::endl;
    //     }
    // }

    // QStringList keys = collectHash.uniqueKeys();
    // for (QStringList::Iterator it = keys.begin(); it != keys.end(); it++)
    // {
    //     QStringList splits = it->split('|');
    //     QList<int> indexes = collectHash.values(*it);

    //     // Identify hash entries with a single index to handle as a remainder instead of collection
    //     if (indexes.count()==1) {
    //         auto singleFile = splits.at(1) + QString::number(indexes[0]) + splits.at(0);
    //         remainders.append(singleFile);
    //         continue;
    //     }

    //     // Create the collection for the current hash key
    //     qSort(indexes);
    //     Collection currentCollection( 
    //         splits.at(1).toLocal8Bit().constData(), 
    //         splits.at(0).toLocal8Bit().constData(), 
    //         indexes.toVector().toStdVector()
    //     );

    //     collections.append(currentCollection);
    // } 

    // return std::make_tuple(collections, remainders);
    // // return std::make_tuple(collections.toVector().toStdVector(), remainders.toVector().toStdVector());


    return make_tuple(collections, remainders);
}
// void FileParser::assemble(QStringList entries, std::vector<Collection>* collections, std::vector<std::string>* remainders) {
// }

// std::tuple<QList<Collection>, QStringList> 
//     FileParser::assemble(QStringList entries) {

//     // Filter hidden
//     // Filter folders, files, symlink and special items

//     // Handle negative frames
//     QRegularExpression re("(?<index>-*\\d+)(?<tail>\\D+)$");
//     QRegularExpressionMatch match;

//     QList<Collection> collections;
//     QStringList remainders;

//     QMultiHash<QString, int> collectHash;
//     QString head, tail, index, hash;

//     for (int i = 0; i < entries.size(); ++i) {
//         match = re.match(entries.at(i));
//         if (match.hasMatch()) {
//             // std::cout << "Has match" << std::endl;
//             head = entries.at(i).left(match.capturedStart("index"));
//             index = match.captured("index");
//             tail = match.captured("tail");
//             hash = tail + "|" + head;  // convention to store unicity of a sequence
    
//             // Limit size of int, sequence can't have more than ~2B frame
//             // as a global range
//             collectHash.insert(hash, index.toInt());

//             // if(i < 20){
//             //     std::cout << qPrintable(entries.at(i)) << " --> " \
//             //         << qPrintable(hash) << " / " \
//             //         << qPrintable(head) << " / " \
//             //         << qPrintable(index) << " / " \
//             //         << qPrintable(tail) << std::endl;
//             // }
//             // else{
//             //     if(i == 21) std::cout << "..." << std::endl;
//             // }
//         }
//         else {
//             remainders.append(entries.at(i));
//             // std::cout << qPrintable(entries.at(i)) << " --> remainders" << std::endl;
//         }
//     }

//     QStringList keys = collectHash.uniqueKeys();
//     for (QStringList::Iterator it = keys.begin(); it != keys.end(); it++)
//     {
//         QStringList splits = it->split('|');
//         QList<int> indexes = collectHash.values(*it);

//         // Identify hash entries with a single index to handle as a remainder instead of collection
//         if (indexes.count()==1) {
//             auto singleFile = splits.at(1) + QString::number(indexes[0]) + splits.at(0);
//             remainders.append(singleFile);
//             continue;
//         }

//         // Create the collection for the current hash key
//         qSort(indexes);
//         Collection currentCollection( 
//             splits.at(1).toLocal8Bit().constData(), 
//             splits.at(0).toLocal8Bit().constData(), 
//             indexes.toVector().toStdVector()
//         );

//         collections.append(currentCollection);
//     } 

//     return std::make_tuple(collections, remainders);
//     // return std::make_tuple(collections.toVector().toStdVector(), remainders.toVector().toStdVector());
// }
