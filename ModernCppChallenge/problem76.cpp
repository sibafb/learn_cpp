#include <iostream>
#include <string_view>
#include <fstream>

#include "json.hpp"
#include "movies.h"

using json = nlohmann::json;

movie_list deserialize(std::string const & filepath)
{
    movie_list movies;

    std::ifstream ifile(filepath.c_str());
    if (ifile.is_open())
    {
        json jdata;
        
        try
        {
            ifile >> jdata;

            if(jdata.is_object())
            {
                for (auto const & element : jdata.at("movies"))
                {
                    movie m;
                    m.id = element.at("id").get<unsigned int>();
                    m.title = element.at("title").get<std::string>();
                    m.year = element.at("year").get<unsigned int>();
                    m.length = element.at("length").get<unsigned int>();

                    for(auto const & role : element.at("cast"))
                    {
                        m.cast.push_back(casting_role{
                            role.at("star").get<std::string>(),
                            role.at("name").get<std::string>() });
                    }

                    for (auto const & director : element.at("directors"))
                    {
                        m.directors.push_back(director);
                    }

                    for (auto const & writer : element.at("writers"))
                    {
                        m.writers.push_back(writer);
                    }

                    movies.push_back(std::move(m));
                }
            }
        }
        catch (std::exception const & ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    return movies;
}

int main()
{
    auto movies = deserialize("movies.json");

    assert(movies.size() == 2);
    assert(movies[0].title == "The Matrix");
    assert(movies[1].title == "Forrest Gump");
}