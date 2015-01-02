#ifndef MAPSETTINGS_HPP
#define MAPSETTINGS_HPP

#include <string>

#include <SFML/System/Vector2.hpp>

#include "ChunkGenerator.hpp"

namespace owi
{

struct MapSettings
{
    MapSettings();

    std::string directory;
    sf::Vector2i chunkSize;
    sf::Vector2i tileSize;
    sf::Vector2i texSize;

    bool onlineMode;

    // Online Mode Properties
    bool isServer;



    // Offline Mode Properties
    ChunkGenerator* generator;
    bool compressedData;
};

} // owi

#endif // MAPSETTINGS_HPP
