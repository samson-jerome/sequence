#include <iostream>
#include "seqparser.h"
#include <boost/filesystem.hpp>

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
    // fs::path full_path("/home/jerome/workspace/dev/samples/sequences/candlesmoke-exr");
    // cout << "path: " << full_path << endl;


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