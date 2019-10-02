#include <iostream>
#include "seqparser.h"
#include <boost/filesystem.hpp>

#include "collection.h"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{

    vector<Collection> collections;
    vector<string> remainders;

    // std::string folder = "/home/jerome/workspace/dev/samples/sequences/test";
    // std::string folder = "/home/jerome/workspace/dev/samples/sequences/candlesmoke-exr";
    // QDir mydir(QString::fromStdString(folder));
    // filesystem.path mypath()
    path p("/home/jer/workspace/dev/samples/sequences/test");
    cout << "given path: " << p << endl;

    try {
        if(exists(p) && is_directory(p)) {
            vector<string> entries;

            for (auto&& x : directory_iterator(p))
                entries.push_back(x.path().filename().string()); 

            sort(entries.begin(), entries.end());
            tie(collections, remainders) = SequenceParser::assemble(entries);

            // cout << "Found collections: " << collections.size() << endl;
            // cout << "Found remainders: " << remainders.size() << endl;
        }
    }
    catch (const filesystem_error& ex) {
        cout << ex.what() << '\n';
    }

    // auto[collections, remainders] = FileParser::assemble(mydir.entryList(QDir::Filter::Files));
    // // QStringList l = {
    // //     // "aaa.10.ext", "aaa.11.ext", "aaa.12.ext", "aaa.13.ext",
    // //     // "bbb.10.ext", "bbb.12.ext", "bbb.14.ext", "bbb.16.ext",
    // //     "ccc.01.ext", "ccc.02.ext", "ccc.03.ext", "ccc.06.ext",
    // //     "ccc.08.ext", "ccc.10.ext", "ccc.12.ext", "ccc.14.ext", "ccc.25.ext", "ccc.28.ext",
    // //     "yyy.10.ext"
    // // };
    // // auto[collections, remainders] = FileParser::assemble(l);

    cout << endl << "Found collections: " << collections.size() << endl;
    for(Collection i : collections) {
        cout << "------------------------" << endl;
        cout << i.format() << endl;
        i.info();
    }

    cout << endl << "Found remainders: " << remainders.size() << endl;
    for(string s : remainders)
        cout << s << endl;

    return 0;
}