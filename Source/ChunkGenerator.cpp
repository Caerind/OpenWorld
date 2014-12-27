#include "ChunkGenerator.hpp"
#include "Map.hpp"

namespace owi
{

ChunkGenerator::ChunkGenerator()
{
    mDefaultTileset = "out.tileset";
}

void ChunkGenerator::setMap(Map* map)
{
    mMap = map;
}

void ChunkGenerator::setDefaultTileset(std::string const& filename)
{
    mDefaultTileset = filename;
}

void ChunkGenerator::createChunk(Chunk& chunk, sf::Vector2i pos)
{
    if (mMap != nullptr)
    {
        chunk.setTileset(mMap->getTileset(mDefaultTileset));
        chunk.setPos(pos);
        for (unsigned int i = 0; i < 32; i++)
        {
            for (unsigned int j = 0; j < 32; j++)
            {
                chunk.getLayer(0)->setTileId(i,j,1);
            }
        }
        chunk.saveToFile(mMap->getDirectory() + std::to_string(pos.x) + "_" + std::to_string(pos.y) + ".chunk");
    }
    else
    {
        #ifdef DEBUG
        std::cout << "ChunkGenerator: hasn't any Map" << std::endl;
        #endif // DEBUG
    }
}

}
