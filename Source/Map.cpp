#include "Map.hpp"

namespace ow
{

Map::Map(MapSettings const& settings)
: mSettings(settings)
, mChunks({{ {Chunk(this),Chunk(this),Chunk(this)}
, {Chunk(this),Chunk(this),Chunk(this)}
, {Chunk(this),Chunk(this),Chunk(this)} }})
{
    if (mSettings.generator == nullptr)
    {
        mSettings.generator = new ChunkGenerator();
    }
    mSettings.generator->setMap(this);
    createDirectory(mSettings.directory);
    loadChunks(sf::Vector2i(0,0),false);
}

sf::Vector2i Map::getChunkSize() const
{
    return mSettings.chunkSize;
}

sf::Vector2i Map::getTileSize() const
{
    return mSettings.tileSize;
}

sf::Vector2i Map::getTexSize() const
{
    return mSettings.texSize;
}

std::string Map::getDirectory() const
{
    return mSettings.directory;
}

bool Map::isDataCompressed() const
{
    return mSettings.compressedData;
}

bool Map::isIsometric() const
{
    return mSettings.isometric;
}

Map::Update Map::update(sf::Vector2f position)
{
    auto x = position.x / (mSettings.chunkSize.x * mSettings.tileSize.x);
    auto y = position.y / (mSettings.chunkSize.y * mSettings.tileSize.y * 0.5f);
    sf::Vector2i pos = sf::Vector2i(x,y);
    if (x < 0) pos.x--;
    if (y < 0) pos.y--;

    if (mChunks[1][1].getPos() != pos)
    {
        if (mChunks[0][0].getPos() == pos)
        {
            moveTL(pos);
            return Update::TL;
        }
        else if (mChunks[1][0].getPos() == pos)
        {
            moveT(pos);
            return Update::T;
        }
        else if (mChunks[2][0].getPos() == pos)
        {
            moveTR(pos);
            return Update::TR;
        }
        else if (mChunks[0][1].getPos() == pos)
        {
            moveL(pos);
            return Update::L;
        }
        else if (mChunks[2][1].getPos() == pos)
        {
            moveR(pos);
            return Update::R;
        }
        else if (mChunks[0][2].getPos() == pos)
        {
            moveBL(pos);
            return Update::BL;
        }
        else if (mChunks[1][2].getPos() == pos)
        {
            moveB(pos);
            return Update::B;
        }
        else if (mChunks[2][2].getPos() == pos)
        {
            moveBR(pos);
            return Update::BR;
        }
        else
        {
            loadChunks(pos);
            return Update::Unknown;
        }
    }
    return Update::Nothing;
}

void Map::render(unsigned int layer, sf::RenderTarget& target, sf::FloatRect viewRect) const
{
    #ifdef OW_INFO
    sf::Clock clock;
    #endif // OW_INFO

    sf::RenderStates states;
    states.transform *= getTransform();
    unsigned int maxLayer = getMaxLayer();
    sf::Transform layerOffset;

    if (isIsometric())
        layerOffset.translate(0,-mSettings.texSize.y+mSettings.tileSize.y);

    for (unsigned int h = 0; h < maxLayer; h++)
    {
        for (unsigned int j = 0; j < static_cast<unsigned int>(3 * mSettings.chunkSize.y); j++)
        {
            for (unsigned int i = 0; i < 3; i++)
            {
                if ((viewRect != sf::FloatRect(0,0,0,0) && mChunks[i][j/mSettings.chunkSize.y].getBounds().intersects(viewRect)) || viewRect == sf::FloatRect(0,0,0,0))
                {
                    mChunks[i][j/mSettings.chunkSize.y].render(j%mSettings.chunkSize.y,h,target,states,viewRect);
                }
            }
        }
        states.transform *= layerOffset;
        if (isIsometric() && viewRect != sf::FloatRect(0,0,0,0))
        {
            viewRect.height += mSettings.texSize.y;
        }
    }

    #ifdef OW_INFO
    std::cout << "Map: Drawn in : " << clock.restart().asSeconds() << " s" << std::endl;
    #endif // OW_INFO
}

Tileset::Ptr Map::getTileset(std::string const& filename)
{
    if (mTilesets.find(filename) != mTilesets.end())
    {
        return mTilesets[filename];
    }
    else
    {
        loadTileset(filename);
        return mTilesets[filename];
    }
}

bool Map::loadTileset(std::string const& filename)
{
    mTilesets[filename] = std::make_shared<Tileset>(this);
    if (!mTilesets[filename]->loadFromFile(filename))
    {
        #ifdef OW_DEBUG
        std::cout << "Map: Cant load texture : " << filename << std::endl;
        #endif // OW_DEBUG
        return false;
    }
    return true;
}

void Map::loadChunks(sf::Vector2i pos, bool saveFirst)
{
    #ifdef OW_INFO
    sf::Clock clock;
    #endif // OW_INFO
    for (int j = -1; j < 2; j++)
    {
        for (int i = -1; i < 2; i++)
        {
            if (mSettings.onlineMode)
            {
                requestChunk(sf::Vector2i(pos.x+i,pos.y+j));
            }
            else
            {
                if (saveFirst)
                {
                    mChunks[i+1][j+1].saveToFile(mSettings.directory + std::to_string(mChunks[i+1][j+1].getPos().x) + "_" + std::to_string(mChunks[i+1][j+1].getPos().y) + ".chunk");
                }
                if (!mChunks[i+1][j+1].loadFromFile(mSettings.directory + std::to_string(pos.x+i) + "_" + std::to_string(pos.y+j) + ".chunk"))
                {
                    mSettings.generator->createChunk(mChunks[i+1][j+1],sf::Vector2i(pos.x+i,pos.y+j));
                }
            }
        }
    }
    #ifdef OW_INFO
    std::cout << "Map: Chunks initialized in : " << clock.restart().asSeconds() << " s" << std::endl;
    #endif // OW_INFO
}

void Map::requestChunk(sf::Vector2i pos)
{

}

void Map::createDirectory(std::string const& filename)
{
    // Need to be cross-platform
    // And only create if the directory dont exist
}

void Map::moveTL(sf::Vector2i pos)
{
    /*

    Need to work on this,

    Actually there is a problem with rendering during the frame on which you move all chunks
    And I don't know why...

    if (!mSettings.onlineMode)
    {
        mChunks[0][0].saveToFile(mSettings.directory + std::to_string(mChunks[0][0].getPos().x) + "_" + std::to_string(mChunks[0][0].getPos().y) + ".chunk");
        mChunks[1][0].saveToFile(mSettings.directory + std::to_string(mChunks[1][0].getPos().x) + "_" + std::to_string(mChunks[1][0].getPos().y) + ".chunk");
        mChunks[2][0].saveToFile(mSettings.directory + std::to_string(mChunks[2][0].getPos().x) + "_" + std::to_string(mChunks[2][0].getPos().y) + ".chunk");
        mChunks[0][1].saveToFile(mSettings.directory + std::to_string(mChunks[0][1].getPos().x) + "_" + std::to_string(mChunks[0][1].getPos().y) + ".chunk");
        mChunks[0][2].saveToFile(mSettings.directory + std::to_string(mChunks[0][2].getPos().x) + "_" + std::to_string(mChunks[0][2].getPos().y) + ".chunk");
    }

    mChunks[0][0] = mChunks[1][1];
    mChunks[1][0] = mChunks[2][1];
    mChunks[0][1] = mChunks[1][2];
    mChunks[1][1] = mChunks[2][2];

    if (!mSettings.onlineMode)
    {
        if(!mChunks[2][0].loadFromFile(mSettings.directory + std::to_string(pos.x+1) + "_" + std::to_string(pos.y-1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[2][0],sf::Vector2i(pos.x+1,pos.y-1));
        }
        if(!mChunks[2][1].loadFromFile(mSettings.directory + std::to_string(pos.x+1) + "_" + std::to_string(pos.y) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[2][1],sf::Vector2i(pos.x+1,pos.y));
        }
        if(!mChunks[2][2].loadFromFile(mSettings.directory + std::to_string(pos.x+1) + "_" + std::to_string(pos.y+1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[2][2],sf::Vector2i(pos.x+1,pos.y+1));
        }
        if(!mChunks[1][2].loadFromFile(mSettings.directory + std::to_string(pos.x) + "_" + std::to_string(pos.y+1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[1][2],sf::Vector2i(pos.x,pos.y+1));
        }
        if(!mChunks[0][2].loadFromFile(mSettings.directory + std::to_string(pos.x-1) + "_" + std::to_string(pos.y+1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[0][2],sf::Vector2i(pos.x-1,pos.y+1));
        }
    }
    else
    {

    }
    */

    loadChunks(pos);
}

void Map::moveT(sf::Vector2i pos)
{
    /*

    Need to work on this,

    Actually there is a problem with rendering during the frame on which you move all chunks
    And I don't know why...

    if (!mSettings.onlineMode)
    {
        mChunks[0][0].saveToFile(mSettings.directory + std::to_string(mChunks[0][0].getPos().x) + "_" + std::to_string(mChunks[0][0].getPos().y) + ".chunk");
        mChunks[1][0].saveToFile(mSettings.directory + std::to_string(mChunks[1][0].getPos().x) + "_" + std::to_string(mChunks[1][0].getPos().y) + ".chunk");
        mChunks[2][0].saveToFile(mSettings.directory + std::to_string(mChunks[2][0].getPos().x) + "_" + std::to_string(mChunks[2][0].getPos().y) + ".chunk");
    }

    mChunks[0][0] = mChunks[0][1];
    mChunks[1][0] = mChunks[1][1];
    mChunks[2][0] = mChunks[2][1];
    mChunks[0][1] = mChunks[0][2];
    mChunks[1][1] = mChunks[1][2];
    mChunks[2][1] = mChunks[2][2];

    if (!mSettings.onlineMode)
    {
        if(!mChunks[0][2].loadFromFile(mSettings.directory + std::to_string(pos.x-1) + "_" + std::to_string(pos.y+1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[0][2],sf::Vector2i(pos.x-1,pos.y+1));
        }
        if(!mChunks[1][2].loadFromFile(mSettings.directory + std::to_string(pos.x) + "_" + std::to_string(pos.y+1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[1][2],sf::Vector2i(pos.x,pos.y+1));
        }
        if(!mChunks[2][2].loadFromFile(mSettings.directory + std::to_string(pos.x+1) + "_" + std::to_string(pos.y+1) + ".chunk"))
        {
            mSettings.generator->createChunk(mChunks[2][2],sf::Vector2i(pos.x+1,pos.y+1));
        }
    }
    else
    {

    }
    */

    loadChunks(pos);
}

void Map::moveTR(sf::Vector2i pos)
{
    loadChunks(pos);
}

void Map::moveL(sf::Vector2i pos)
{
    loadChunks(pos);
}

void Map::moveR(sf::Vector2i pos)
{
    loadChunks(pos);
}

void Map::moveBL(sf::Vector2i pos)
{
    loadChunks(pos);
}

void Map::moveB(sf::Vector2i pos)
{
    loadChunks(pos);
}

void Map::moveBR(sf::Vector2i pos)
{
    loadChunks(pos);
}

unsigned int Map::getMaxLayer() const
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            ret = std::max(ret,mChunks[i][j].getLayerCount());
        }
    }
    return ret;
}

} // ow
