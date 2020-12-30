#include <regex> 
#include <cassert>

#include "sequence/fileutils.h"
#include <filesystem>

namespace sequence {
    namespace fileutils {

        /**
        */
        std::pair<Collection, bool> collection_from_file(std::string filepath, std::string pattern) {
            std::pair<Collection, bool> result;

            std::vector<std::string> matching_files_list;
            auto path = std::filesystem::path(filepath);
            auto folder = std::filesystem::canonical(path.parent_path());
            std::string file = path.filename().string();

            std::cout << fmt::format("Collecting from file: {} > {}\n", filepath, path.string());
            std::cout << fmt::format("Folder: {}, file: {}\n", folder.string(), file);
            std::regex re("(-*\\d+)(\\D*)$");
            std::smatch match;
            std::string head, str_index, tail;
            // std::cout << regex_replace (file, re, "*$2");

            if (regex_search(file, match, re) && match.size() > 1) {
                // std::cout << "   - matches: " << match.size();

                head = file.substr(0, match.position(0));
                str_index = match.str(1);
                tail = match.str(2);
                // std::cout << "   - head: " << head;
                // std::cout << "   - str_index: " << str_index;
                // std::cout << "   - tail: " << tail;
            }
            // else{

            // }
            std::cout << "filtered regex: " << fmt::format("^{0}(\\d+){1}$", head, tail) << std::endl;
            std::regex filtered_regex(fmt::format("^{0}(\\d+){1}$", head, tail));
            std::string f;
            for (const auto curr_file : std::filesystem::directory_iterator(folder)) {
                f = curr_file.path().filename().string();
                if (regex_search(f, match, filtered_regex)) {
                    // std::cout << "   - matches: " << match.size() << " for: ";
                    matching_files_list.push_back((folder / curr_file.path()).string());
                }
                // std::cout << f << endl;
            }

            // for (auto p : matching_files_list) {
            //     std::cout << p << endl;
            // }

            std::vector<Collection> collections;
            std::vector<string> remainders;
            tie(collections, remainders) = assemble(matching_files_list);

            assert(collections.size() < 2);
            result.first = collections.at(0);
            result.second = true; 
            return result;
        }
    }
}