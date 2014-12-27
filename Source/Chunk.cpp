#include "Chunk.hpp"
#include "Map.hpp"

namespace owi
{

Chunk::Chunk(Map* parent)
: mParent(parent)
, mPos(sf::Vector2i(0,0))
, mTileset(nullptr)
, mLayers({Layer(this),Layer(this),Layer(this)})
{
    if (mParent == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Chunk: Haven't any parent" << std::endl;
        #endif // DEBUG
    }
}

bool Chunk::loadFromFile(std::string const& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        #ifdef DEBUG
        std::cout << "Chunk: Cannot read file : " << filename << std::endl;
        #endif // DEBUG
        return false;
    }

    if (mParent == nullptr)
    {
        return false;
    }

    std::string line;

    // Read Pos
    std::getline(file,line);
    mPos.x = std::stoi(line.substr(0,line.find(":")));
    mPos.y = std::stoi(line.substr(line.find(":")+1,line.size()));

    sf::Transformable::setPosition(sf::Vector2f(mPos.x * mParent->getTileSize().x * mParent->getChunkSize().x, mPos.y * mParent->getTileSize().y * mParent->getChunkSize().y));
    for (unsigned int k = 0; k < 3; k++)
    {
        mLayers[k].update();
    }

    // Read Tileset
    std::getline(file,line);
    mTileset = mParent->getTileset(line);
    if (mTileset == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Chunk: Don't have any Tileset : " << line << std::endl;
        #endif // DEBUG
    }

    for (unsigned int k = 0; k < 3; k++)
    {
        for (int j = 0; j < mParent->getChunkSize().y; j++)
        {
            std::getline(file,line);
            std::stringstream ss(line);
            std::string temp;
            for (int i = 0; i < mParent->getChunkSize().x; i++)
            {
                std::getline(ss,temp,',');
                mLayers[k].setTileId(i,j,stoi(temp));
            }
        }
    }
    file.close();
    return true;
}

bool Chunk::saveToFile(std::string const& filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        #ifdef DEBUG
        std::cout << "Chunk: Cannot write file : " << filename << std::endl;
        #endif // DEBUG
        return false;
    }

    if (mParent == nullptr)
    {
        return false;
    }

    file << mPos.x << ":" << mPos.y << std::endl;
    if (mTileset == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Chunk: Cannot access Tileset name" << std::endl;
        file << "--ERROR-- Please remplace this line with a correct Tileset file" << std::endl;
        #endif // DEBUG
    }
    else
    {
        file << mTileset->getFilename() << std::endl;
    }

    for (unsigned int k = 0; k < 3; k++)
    {
        for (int j = 0; j < mParent->getChunkSize().y; j++)
        {
            for (int i = 0; i < mParent->getChunkSize().x; i++)
            {
                file << mLayers[k].getTileId(i,j) << ",";
            }
            file << std::endl;
        }
    }
    file.close();
    return true;
}

void Chunk::render(unsigned int layer, sf::RenderTarget& target, sf::RenderStates states) const
{
    if (layer >= 0 && layer < 3)
    {
        states.transform *= getTransform();
        mLayers[layer].render(target,states);
    }
    else
    {
        #ifdef DEBUG
        std::cout << "Chunk: Uncorrect Layer : " << layer << std::endl;
        #endif // DEBUG
    }
}

void Chunk::render(unsigned int layer, sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect viewRect) const
{
    if (getBounds().intersects(viewRect))
    {
        render(layer,target,states);
    }
}

void Chunk::setPos(sf::Vector2i pos)
{
    mPos = pos;
    if (mParent != nullptr)
        sf::Transformable::setPosition(sf::Vector2f(mPos.x * mParent->getTileSize().x * mParent->getChunkSize().x, mPos.y * mParent->getTileSize().y * mParent->getChunkSize().y));
}

sf::Vector2i Chunk::getPos() const
{
    return mPos;
}

sf::Vector2i Chunk::getSize() const
{
    if (mParent != nullptr)
        return mParent->getChunkSize();
    return sf::Vector2i(0,0);
}

sf::Vector2i Chunk::getTileSize() const
{
    if (mParent != nullptr)
        return mParent->getTileSize();
    return sf::Vector2i(0,0);
}

sf::FloatRect Chunk::getBounds() const
{
    if (mParent != nullptr)
        return sf::FloatRect(getPosition(),sf::Vector2f(mParent->getChunkSize().x * mParent->getTileSize().x, mParent->getChunkSize().y * mParent->getTileSize().y));
    return sf::FloatRect(getPosition(),sf::Vector2f(0,0));
}

Layer* Chunk::getLayer(unsigned int layer)
{
    if (layer >= 0 && layer < 3)
    {
        return &mLayers[layer];
    }
    else
    {
        #ifdef DEBUG
        std::cout << "Chunk: Uncorrect Layer : " << layer << std::endl;
        #endif // DEBUG
    }
    return nullptr;
}

unsigned int Chunk::getLayerCount() const
{
    return mLayers.size();
}

void Chunk::setTileset(Tileset::Ptr tileset)
{
    mTileset = tileset;
}

Tileset::Ptr Chunk::getTileset() const
{
    return mTileset;
}

}
