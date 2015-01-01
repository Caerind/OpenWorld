#include "MyFirstChunkGenerator.hpp"
#include "Map.hpp"

MyFirstChunkGenerator::MyFirstChunkGenerator() : ChunkGenerator()
{
    srand(time(NULL));
}

void MyFirstChunkGenerator::createChunk(owi::Chunk& chunk, sf::Vector2i pos)
{
    if (mMap != nullptr)
    {
        chunk.setTileset(mMap->getTileset(mDefaultTileset));
        chunk.setPos(pos);
        chunk.addLayer();
        chunk.addLayer();
        for (unsigned int i = 0; i < static_cast<unsigned int>(chunk.getSize().x); i++)
        {
            for (unsigned int j = 0; j < static_cast<unsigned int>(chunk.getSize().y); j++)
            {
                chunk.getLayer(0)->setTileId(i,j,rand() %4 +1);
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
