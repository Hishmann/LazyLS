#include "file.h"


std::optional<std::vector<std::string>> contents_list(fs::path p) {

    fs::directory_entry de{p};

    if (!de.exists() || !de.is_directory()) {
        return std::nullopt;
    }
    
    std::vector<std::string> ret;

    for (auto const& dir_entry : std::filesystem::directory_iterator{p}) {
        ret.push_back(dir_entry.path().filename());
    }

    return ret;
}