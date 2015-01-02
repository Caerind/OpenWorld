#include "IsometricGenerator.hpp"
#include "Map.hpp"

IsometricGenerator::IsometricGenerator() : ChunkGenerator(std::string("out.tileset"))
{
    srand(time(NULL));
}

void IsometricGenerator::createChunk(ow::Chunk& chunk, sf::Vector2i pos)
{
    if (mMap != nullptr)
    {
        chunk.setTileset(mMap->getTileset(mDefaultTileset));
        chunk.setPos(pos);
        chunk.clearLayers();
        chunk.addLayer();
        chunk.addLayer();
        for (unsigned int i = 0; i < static_cast<unsigned int>(chunk.getSize().x); i++)
        {
            for (unsigned int j = 0; j < static_cast<unsigned int>(chunk.getSize().y); j++)
            {
                chunk.getLayer(0)->setTileId(i,j,8);
                if (rand() % 10 == 1)
                {
                    chunk.getLayer(1)->setTileId(i,j,rand()%19);
                }
            }
        }
        chunk.saveToFile(mMap->getDirectory() + std::to_string(pos.x) + "_" + std::to_string(pos.y) + ".chunk");
    }
    else
    {
        #ifdef OW_DEBUG
        std::cout << "ChunkGenerator: hasn't any Map" << std::endl;
        #endif // OW_DEBUG
    }
}
