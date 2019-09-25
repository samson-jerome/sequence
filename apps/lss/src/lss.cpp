#include <iostream>
#include "seqparser.h"
#include <boost/filesystem.hpp>

#include "collection.h"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{
    std::cout << "Yo" << std::endl; 
    int res = give_me_one();
    std::cout << "res = " << res << std::endl; 

    SequenceParser s;

    // std::string folder = "/home/jerome/workspace/dev/samples/sequences/test";
    // std::string folder = "/home/jerome/workspace/dev/samples/sequences/candlesmoke-exr";
    // QDir mydir(QString::fromStdString(folder));
    // filesystem.path mypath()
    path p("/home/jerome/workspace/dev/samples/sequences/candlesmoke-exr");
    cout << "given path: " << p << endl;
    // try
    // {
    //     if (exists(p))
    //     {
    //         if (is_regular_file(p))
    //             cout << p << " size is " << file_size(p) << '\n';

    //         else if (is_directory(p))
    //         {
    //             cout << p << " is a directory containing:\n";

    //             for (directory_entry& x : directory_iterator(p))
    //                 cout << "    " << x.path() << '\n'; 
    //         }
    //         else
    //             cout << p << " exists, but is not a regular file or directory\n";
    //     }
    //     else
    //         cout << p << " does not exist\n";
    // }

    // catch (const filesystem_error& ex)
    // {
    //     cout << ex.what() << '\n';
    // }

    try {
        if(exists(p) && is_directory(p)) {
            // vector<path> entries;
            vector<string> entries;

            // for (auto&& x : directory_iterator(p))
            //     entries.push_back(x.path()); 

            // std::sort(entries.begin(), entries.end());
            
            // for (auto&& x : entries)
            //     v.push_back(x.filename().string());

            for (auto&& x : directory_iterator(p))
                entries.push_back(x.path().filename().string()); 

            std::sort(entries.begin(), entries.end());

            // for (auto&& i : entries)
            //     cout << "    " << i << '\n'; 

            // for (directory_entry& x : directory_iterator(p))
            //     cout << "    " << x.path() << '\n'; 

            vector<Collection> collections;
            vector<string> remainders;

            tie(collections, remainders) = SequenceParser::assemble(entries);

            cout << "Found collections: " << collections.size() << endl;
            cout << "Found remainders: " << remainders.size() << endl;

            // auto[collections, remainders] = SequenceParser::assemble(entries);
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

    // std::cout << "Found collections: " << collections.count() << std::endl;

    // for(Collection i : collections) {
    //     // std::cout << "------------------------" << std::endl;
    //     i.info();
    //     std::cout << "--" << std::endl;
    //     std::cout << i.format() << std::endl;
    // }

    // std::cout << "Found remainders: " << remainders.count() << std::endl;
    // for(QString s : remainders) {
    //     std::cout << qPrintable(s) << std::endl;
    // }

    return 0;
}