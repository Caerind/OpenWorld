#ifndef OW_CHUNKGENERATOR_HPP
#define OW_CHUNKGENERATOR_HPP

#include <iostream>
#include <string>

#include "Chunk.hpp"

namespace ow
{

class Map;

class ChunkGenerator
{
    public:
        ChunkGenerator();
        ChunkGenerator(std::string const& filename);

        void setMap(Map* map);
        void setDefaultTileset(std::string const& filename);

        virtual void createChunk(Chunk& chunk, sf::Vector2i pos);

    protected:
        Map* mMap;
        std::string mDefaultTileset;
};

} // ow

#endif // OW_CHUNKGENERATOR_HPP
