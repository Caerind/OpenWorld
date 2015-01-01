#include "Map.hpp"

namespace owi
{

Map::Map(std::string const& directory, sf::Vector2i chunkSize, sf::Vector2i tileSize, sf::Vector2i texSize, ChunkGenerator* generator)
: mChunkSize(chunkSize)
, mTileSize(tileSize)
, mTexSize(texSize)
, mDirectory(directory)
, mGenerator(generator)
, mChunks({{ {Chunk(this),Chunk(this),Chunk(this)}
, {Chunk(this),Chunk(this),Chunk(this)}
, {Chunk(this),Chunk(this),Chunk(this)} }})
{
    if (mGenerator == nullptr)
    {
        mGenerator = new ChunkGenerator();
    }
    mGenerator->setMap(this);
    createDirectory(directory);
    initChunks();
}

sf::Vector2i Map::getChunkSize() const
{
    return mChunkSize;
}

sf::Vector2i Map::getTileSize() const
{
    return mTileSize;
}

sf::Vector2i Map::getTexSize() const
{
    return mTexSize;
}

std::string Map::getDirectory() const
{
    return mDirectory;
}

Map::Update Map::update(sf::Vector2f position)
{
    auto x = position.x / (mChunkSize.x * mTileSize.x);
    auto y = position.y / (mChunkSize.y * mTileSize.y);
    sf::Vector2i pos = sf::Vector2i(x,y);
    if (x < 0) pos.x--;
    if (y < 0) pos.y--;

    if (mChunks[1][1].getPos() != pos)
    {
        /*if (mChunks[0][0].getPos() == pos)
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
        */

        loadChunks(pos);

        /*
        }
        */
    }
    return Update::Nothing;
}

void Map::render(unsigned int layer, sf::RenderTarget& target) const
{
    sf::RenderStates states;
    states.transform *= getTransform();
    unsigned int maxLayer = getMaxLayer();

    for (unsigned int h = 0; h < maxLayer; h++)
    {
        for (unsigned int j = 0; j < static_cast<unsigned int>(3 * mChunkSize.y); j++)
        {
            for (unsigned int i = 0; i < 3; i++)
            {
                mChunks[i][j/mChunkSize.y].render(j%mChunkSize.y,h,target,states);
            }
        }
    }
}

void Map::render(unsigned int layer, sf::RenderTarget& target, sf::FloatRect viewRect) const
{
    sf::RenderStates states;
    states.transform *= getTransform();
    unsigned int maxLayer = getMaxLayer();

    for (unsigned int h = 0; h < maxLayer; h++)
    {
        for (unsigned int j = 0; j < static_cast<unsigned int>(3 * mChunkSize.y); j++)
        {
            for (unsigned int i = 0; i < 3; i++)
            {
                if (mChunks[i][j/mChunkSize.y].getBounds().intersects(viewRect))
                {
                    mChunks[i][j/mChunkSize.y].render(j%mChunkSize.y,h,target,states,viewRect);
                }
            }
        }
    }
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
        #ifdef DEBUG
        std::cout << "Map: Cant load texture : " << filename << std::endl;
        #endif // DEBUG
        return false;
    }
    return true;
}

void Map::initChunks(sf::Vector2i pos)
{
    for (int j = -1; j < 2; j++)
    {
        for (int i = -1; i < 2; i++)
        {
            if (!mChunks[i+1][j+1].loadFromFile(mDirectory + std::to_string(pos.x+i) + "_" + std::to_string(pos.y+j) + ".chunk"))
            {
                mGenerator->createChunk(mChunks[i+1][j+1],sf::Vector2i(pos.x+i,pos.y+j));
            }
        }
    }
}

void Map::loadChunks(sf::Vector2i pos)
{
    for (int j = -1; j < 2; j++)
    {
        for (int i = -1; i < 2; i++)
        {
            mChunks[i+1][j+1].saveToFile(mDirectory + std::to_string(mChunks[i+1][j+1].getPos().x) + "_" + std::to_string(mChunks[i+1][j+1].getPos().y) + ".chunk");
            if (!mChunks[i+1][j+1].loadFromFile(mDirectory + std::to_string(pos.x+i) + "_" + std::to_string(pos.y+j) + ".chunk"))
            {
                mGenerator->createChunk(mChunks[i+1][j+1],sf::Vector2i(pos.x+i,pos.y+j));
            }
        }
    }
}

void Map::createDirectory(std::string const& filename)
{
    // Need to be cross-platform
    // And only create if the directory dont exist
}

void Map::moveTL(sf::Vector2i pos)
{
    loadChunks(pos);
}

void Map::moveT(sf::Vector2i pos)
{
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

}
