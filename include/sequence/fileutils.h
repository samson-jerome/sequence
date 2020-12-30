#ifndef FILEUTILS_H
#define FILEUTILS_H
#pragma once

#include <filesystem>

#include "sequence.h"

using std::string;
using std::pair;
// using std::filesystem;

namespace sequence {
    namespace fileutils {
        std::pair<Collection, bool> collection_from_file(string filepath, string pattern = "");
    }
}
#endif // FILEUTILS_H
