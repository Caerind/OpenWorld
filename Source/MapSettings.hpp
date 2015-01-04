#ifndef OW_MAPSETTINGS_HPP
#define OW_MAPSETTINGS_HPP

#include <string>
#include <fstream>

#include <SFML/System/Vector2.hpp>

#include "ChunkGenerator.hpp"

namespace ow
{

struct MapSettings
{
    MapSettings();

    bool loadFromFile(std::string const& directory);
    bool saveToFile();

    std::string directory;
    sf::Vector2i chunkSize;
    sf::Vector2i tileSize;
    sf::Vector2i texSize;
    bool isometric;

    bool onlineMode;

    // Online Mode Properties



    // Offline Mode Properties
    ChunkGenerator* generator;
    bool compressedData;
};

} // ow

#endif // OW_MAPSETTINGS_HPP
