#include "OrthogonnalGenerator.hpp"
#include "Map.hpp"

OrthogonnalGenerator::OrthogonnalGenerator() : ChunkGenerator(std::string("out.tileset"))
{
    srand(time(NULL));
}

void OrthogonnalGenerator::createChunk(ow::Chunk& chunk, sf::Vector2i pos)
{
    if (mMap != nullptr)
    {
        chunk.setTileset(mMap->getTileset(mDefaultTileset));
        chunk.setPos(pos);
        chunk.clearLayers();
        chunk.addLayer();
        chunk.addLayer();
        chunk.addLayer();
        for (unsigned int i = 0; i < 32; i++)
        {
            for (unsigned int j = 0; j < 32; j++)
            {
                chunk.getLayer(0)->setTileId(i,j,rand() %4 +1);
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
