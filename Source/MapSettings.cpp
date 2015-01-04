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
    generator = nullptr;
    compressedData = false;
}

bool MapSettings::loadFromFile(std::string const& directory)
{
    std::ifstream file(directory + ".settings");
    if (!file)
        return false;

    file << CompressionUtils::compressLine(directory) << std::endl;
    file << CompressionUtils::compressLine(std::stoi(chunkSize.x)) << std::endl;
    file << CompressionUtils::compressLine(std::stoi(chunkSize.y)) << std::endl;
    file << CompressionUtils::compressLine(std::stoi(tileSize.x)) << std::endl;
    file << CompressionUtils::compressLine(std::stoi(tileSize.y)) << std::endl;
    file << CompressionUtils::compressLine(std::stoi(texSize.x)) << std::endl;
    file << CompressionUtils::compressLine(std::stoi(texSize.y)) << std::endl;
    if (isometric)
        file << CompressionUtils::compressLine("true") << std::endl;
    else
        file << CompressionUtils::compressLine("false") << std::endl;
    if (compressedData)
        file << CompressionUtils::compressLine("true") << std::endl;
    else
        file << CompressionUtils::compressLine("false") << std::endl;
    if (onlineMode)
    {
        file << CompressionUtils::compressLine("true") << std::endl;

    }
    else
        file << CompressionUtils::compressLine("false") << std::endl;
    file.close();
    return true;
}

bool MapSettings::saveToFile()
{
    std::ofstream file(directory + ".settings");
    if (!file)
        return false;

    std::string line;
    unsigned int lineCount = 0;
    while (std::getline(file,line))
    {
        CompressionUtils::uncompressLine(line);
        switch (lineCount)
        {
            case 0: directory = line; break;
            case 1: chunkSize.x = std::stoi(line); break;
            case 2: chunkSize.y = std::stoi(line); break;
            case 3: tileSize.x = std::stoi(line); break;
            case 4: tileSize.y = std::stoi(line); break;
            case 5: texSize.x = std::stoi(line); break;
            case 6: texSize.y = std::stoi(line); break;
            case 7: isometric = (line == "true") ? true : false; break;
            case 8: compressedData = (line == "true") ? true : false; break;
            case 9: onlineMode = (line == "true") ? true : false; break;
            default: break;
        }
        lineCount++;
    }
    file.close();
    return true;
}

} // ow
