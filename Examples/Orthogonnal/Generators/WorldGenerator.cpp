#include "WorldGenerator.hpp"
#include "Map.hpp"

WorldGenerator::WorldGenerator() : ChunkGenerator(std::string("out.tileset"))
{
    srand(time(NULL));
    mWaterId = 133;
    mSandId = 1;
    mGrassId = 1;
    mPercentGrassOverWater = 20;
    mPercentFromNeighboors = 100;
    mPercentDefault = 0;
    mPercentBonus = 40;
}

void WorldGenerator::createChunk(ow::Chunk& chunk, sf::Vector2i pos)
{
    if (mMap == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "ChunkGenerator: hasn't any Map" << std::endl;
        #endif // OW_DEBUG
        return;
    }

    chunk.setTileset(mMap->getTileset(mDefaultTileset));
    chunk.setPos(pos);
    chunk.clearLayers();
    chunk.addLayer();
    fill(chunk,mWaterId,0);

    std::vector<sf::Vector2i> transformed;
    std::vector<sf::Vector2i> checkList;
    unsigned int tileLimit = chunk.getSize().x * chunk.getSize().y * mPercentGrassOverWater / 100;
    bool neighboorsHandled = neighboorsChunks(pos,checkList,transformed,chunk);
    bool transformingDone = false;

    while (!transformingDone)
    {
        // if neighboors handled or no neighboors
        if (neighboorsHandled)
        {
            sf::Vector2i firstTile;
            firstTile.x = rand() % chunk.getSize().x;
            firstTile.y = rand() % chunk.getSize().y;
            transformed.push_back(firstTile);
            checkList.push_back(firstTile);
            chunk.getLayer(0)->setTileId(firstTile.x,firstTile.y,mSandId);
        }
        while (!checkList.empty())
        {
            sf::Vector2i parentTile = checkList[0];
            if (parentTile.x > 0) // left child
                handleChild(sf::Vector2i(parentTile.x-1,parentTile.y),checkList,transformed,chunk);
            if (parentTile.y > 0) // upper child
                handleChild(sf::Vector2i(parentTile.x,parentTile.y-1),checkList,transformed,chunk);
            if (parentTile.x < chunk.getSize().x -1) // right child
                handleChild(sf::Vector2i(parentTile.x+1,parentTile.y),checkList,transformed,chunk);
            if (parentTile.y < chunk.getSize().y -1) // lower child
                handleChild(sf::Vector2i(parentTile.x,parentTile.y+1),checkList,transformed,chunk);

            // End of the loop tasks & checks
            checkList.erase(checkList.begin());
            if (neighboorsHandled && transformed.size() >= tileLimit)
            {
                checkList.clear();
                transformingDone = true;
            }
        }
        if (!neighboorsHandled)
        {
            neighboorsHandled = true;
        }
    }

    for (unsigned int i = 0; i < transformed.size(); i++)
    {
        sf::Vector2i tile = transformed[i];
        if (((tile.x > 0 && chunk.getLayer(0)->getTileId(tile.x-1,tile.y) != mWaterId)
        && (tile.y > 0 && chunk.getLayer(0)->getTileId(tile.x,tile.y-1) != mWaterId)
        && (tile.x < chunk.getSize().x-1 && chunk.getLayer(0)->getTileId(tile.x+1,tile.y) != mWaterId)
        && (tile.y < chunk.getSize().y-1 && chunk.getLayer(0)->getTileId(tile.x,tile.y+1) != mWaterId))
        || tile.x == 0 || tile.y == 0
        || tile.x == chunk.getSize().x -1
        || tile.y == chunk.getSize().y -1)
        {
            chunk.getLayer(0)->setTileId(tile.x,tile.y,mGrassId);
        }
    }
    chunk.saveToFile(mMap->getDirectory() + std::to_string(pos.x) + "_" + std::to_string(pos.y) + ".chunk");
}

void WorldGenerator::fill(ow::Chunk& chunk, unsigned int tileId, unsigned int layer)
{
    if (layer >= 0 && layer < chunk.getLayerCount())
        for (unsigned int i = 0; i < static_cast<unsigned int>(chunk.getSize().x); i++)
            for (unsigned int j = 0; j < static_cast<unsigned int>(chunk.getSize().y); j++)
                chunk.getLayer(layer)->setTileId(i,j,tileId);
}

bool WorldGenerator::neighboorsChunks(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList,std::vector<sf::Vector2i>& transformed,ow::Chunk& chunk)
{
    bool neighboorsHandled = true;
    if (testUpperChunk(pos,checkList))
        neighboorsHandled = false;
    if (testLeftChunk(pos,checkList))
        neighboorsHandled = false;
    if (testRightChunk(pos,checkList))
        neighboorsHandled = false;
    if (testLowerChunk(pos,checkList))
        neighboorsHandled = false;
    if (!neighboorsHandled)
    {
        for (unsigned int i = 0; i < checkList.size(); i++)
        {
            transformed.push_back(checkList[i]);
            chunk.getLayer(0)->setTileId(checkList[i].x,checkList[i].y,mSandId);
        }
    }
    return neighboorsHandled;
}

bool WorldGenerator::testUpperChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList)
{
    bool needHandling = false;
    ow::Chunk chunk(mMap);
    if (chunk.loadFromFile(mMap->getDirectory() + std::to_string(pos.x) + "_" + std::to_string(pos.y-1) + ".chunk"))
    {
        for (int i = 0; i < chunk.getSize().x;i++)
        {
            if (chunk.getLayer(0)->getTileId(i,chunk.getSize().y-1) != mWaterId
            && rand() % 100 < mPercentFromNeighboors)
            {
                needHandling = true;
                checkList.push_back(sf::Vector2i(i,0));
            }
        }
    }
    return needHandling;
}

bool WorldGenerator::testLeftChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList)
{
    bool needHandling = false;
    ow::Chunk chunk(mMap);
    if (chunk.loadFromFile(mMap->getDirectory() + std::to_string(pos.x-1) + "_" + std::to_string(pos.y) + ".chunk"))
    {
        for (int i = 0; i < chunk.getSize().y;i++)
        {
            if (chunk.getLayer(0)->getTileId(chunk.getSize().x-1,i) != mWaterId
            && rand() % 100 < mPercentFromNeighboors)
            {
                needHandling = true;
                checkList.push_back(sf::Vector2i(0,i));
            }
        }
    }
    return needHandling;
}

bool WorldGenerator::testRightChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList)
{
    bool needHandling = false;
    ow::Chunk chunk(mMap);
    if (chunk.loadFromFile(mMap->getDirectory() + std::to_string(pos.x+1) + "_" + std::to_string(pos.y) + ".chunk"))
    {
        for (int i = 0; i < chunk.getSize().y;i++)
        {
            if (chunk.getLayer(0)->getTileId(0,i) != mWaterId
            && rand() % 100 < mPercentFromNeighboors)
            {
                needHandling = true;
                checkList.push_back(sf::Vector2i(chunk.getSize().x-1,i));
            }
        }
    }
    return needHandling;
}

bool WorldGenerator::testLowerChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList)
{
    bool needHandling = false;
    ow::Chunk chunk(mMap);
    if (chunk.loadFromFile(mMap->getDirectory() + std::to_string(pos.x) + "_" + std::to_string(pos.y+1) + ".chunk"))
    {
        for (int i = 0; i < chunk.getSize().x;i++)
        {
            if (chunk.getLayer(0)->getTileId(i,0) != mWaterId
            && rand() % 100 < mPercentFromNeighboors)
            {
                needHandling = true;
                checkList.push_back(sf::Vector2i(i,chunk.getSize().y-1));
            }
        }
    }
    return needHandling;
}

void WorldGenerator::handleChild(sf::Vector2i childTile, std::vector<sf::Vector2i>& checkList, std::vector<sf::Vector2i>& transformed, ow::Chunk& chunk)
{
    for (unsigned int i = 0; i < transformed.size(); i++)
        if (childTile == transformed[i])
            return;

    int percent = mPercentDefault;
    if (childTile.x > 0 && chunk.getLayer(0)->getTileId(childTile.x-1,childTile.y) != mWaterId)
        percent += mPercentBonus;
    if (childTile.y > 0 && chunk.getLayer(0)->getTileId(childTile.x,childTile.y-1) != mWaterId)
        percent += mPercentBonus;
    if (childTile.x < chunk.getSize().x-1 && chunk.getLayer(0)->getTileId(childTile.x+1,childTile.y) != mWaterId)
        percent += mPercentBonus;
    if (childTile.y < chunk.getSize().y-1 && chunk.getLayer(0)->getTileId(childTile.x,childTile.y+1) != mWaterId)
        percent += mPercentBonus;
    if (rand()%100 < percent)
    {
        transformed.push_back(childTile);
        checkList.push_back(childTile);
        chunk.getLayer(0)->setTileId(childTile.x,childTile.y,mSandId);
    }
}

