#include "MapSettings.hpp"

namespace ow
{

MapSettings::MapSettings()
{
    directory = "";
    chunkSize = sf::Vector2i(0,0);
    tileSize = sf::Vector2i(0,0);
    texSize = sf::Vector2i(0,0);
    isometric = false;
    onlineMode = false;
    isServer = false;
    generator = nullptr;
    compressedData = false;
}

} // ow
