#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <regex>
#include <filesystem>

#include "ZipFile.h"
#include "ZipArchive.h"

namespace fs = std::filesystem;

std::vector<std::string> find_in_archive(
    fs::path const & archivepath,
    std::string_view pattern)
{
    std::vector<std::string> results;

    if (fs::exists(archivepath))
    {
        try
        {
            auto archive = ZipFile::Open(archivepath.string());

            for (size_t i = 0; i< archive->GetEntriesCount(); ++i)
            {
                auto entry = archive->GetEntry(i);
                if (entry)
                {
                    if (!entry->IsDirectory())
                    {
                        auto name = entry->GetName();
                        if (std::regex_match(name, std::regex{ pattern.data() }))
                        {
                            results.push_back(entry->GetFullName());
                        }
                    }
                }
            }
        }
        catch(std::exception const & e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    return results;
}

int main()
{
    auto results = find_in_archive("sample79.zip", R"(^.*\.jpg$)");

    for( auto const & name : results)
    {
        std::cout << name << std::endl;
    }

    /*
    std::string archivepath;
    std::cout << "Archive path:";
    std::cin >> archivepath;

    std::string pattern;
    std::cout << "Pattern:";
    std::cin >> pattern;

    std::cout << "Results:" << std::endl;

    for (auto const & name : find_in_archive(archivepath, pattern))
    {
        std::cout << name << std::endl;
    }*/
}