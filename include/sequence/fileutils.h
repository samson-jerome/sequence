#ifndef FILEUTILS_H
#define FILEUTILS_H
#pragma once

#include "collection.h"

using std::string;

namespace sequence {
    namespace fileutils {
        Collection collection_from_file(string filepath, string pattern = "");
    }
}
#endif // FILEUTILS_H
