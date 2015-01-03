#include "Chunk.hpp"
#include "Map.hpp"

namespace ow
{

Chunk::Chunk(Map* parent)
: mParent(parent)
, mPos(sf::Vector2i(0,0))
, mTileset(nullptr)
, mLayers()
{
    if (mParent == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Chunk: Haven't any parent" << std::endl;
        #endif // OW_DEBUG
    }
}

bool Chunk::loadFromFile(std::string const& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        #ifdef OW_DEBUG
        std::cout << "Chunk: Cannot read file : " << filename << std::endl;
        #endif // OW_DEBUG
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

    sf::Transformable::setPosition(sf::Vector2f(mPos.x * mParent->getTileSize().x * mParent->getChunkSize().x, mPos.y * mParent->getTileSize().y * mParent->getChunkSize().y * 0.5f));

    // Read Tileset
    std::getline(file,line);
    mTileset = mParent->getTileset(line);
    if (mTileset == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Chunk: Don't have any Tileset : " << line << std::endl;
        #endif // OW_DEBUG
    }

    // Read Layer Count
    std::getline(file,line);
    unsigned int layerCount = std::stoi(line);
    mLayers.clear();
    for (unsigned int k = 0; k < layerCount; k++)
    {
        addLayer();
        mLayers.back().update();
    }

    for (unsigned int k = 0; k < layerCount; k++)
    {
        for (int j = 0; j < mParent->getChunkSize().y; j++)
        {
            std::getline(file,line);
            if (mParent->isDataCompressed())
                uncompressLine(line);
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
        #ifdef OW_DEBUG
        std::cout << "Chunk: Cannot write file : " << filename << std::endl;
        #endif // OW_DEBUG
        return false;
    }

    if (mParent == nullptr)
    {
        return false;
    }

    // Write Pos
    file << mPos.x << ":" << mPos.y << std::endl;

    // Write Tileset
    if (mTileset == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Chunk: Cannot access Tileset name" << std::endl;
        file << "--ERROR-- Please remplace this line with a correct Tileset file" << std::endl;
        #endif // OW_DEBUG
    }
    else
    {
        file << mTileset->getFilename() << std::endl;
    }

    // Wrie Layer Count
    file << getLayerCount() << std::endl;

    for (unsigned int k = 0; k < getLayerCount(); k++)
    {
        for (int j = 0; j < mParent->getChunkSize().y; j++)
        {
            std::string lineIds;
            for (int i = 0; i < mParent->getChunkSize().x; i++)
            {
                if (mParent->isDataCompressed())
                {
                    lineIds += std::string(std::to_string(mLayers[k].getTileId(i,j)));
                    lineIds += ",";
                }
                else
                {
                    file << mLayers[k].getTileId(i,j) << ",";
                }
            }

            if (mParent->isDataCompressed())
                file << compressLine(lineIds);

            file << std::endl;
        }
    }
    file.close();
    return true;
}

void Chunk::render(unsigned int line, unsigned int layer, sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect viewRect) const
{
    if (layer >= 0 && layer < getLayerCount())
    {
        states.transform *= getTransform();

        if (viewRect != sf::FloatRect(0,0,0,0))
        {
            /*

            Need to work on this,

            I don't know why this isn't working....





            sf::FloatRect rect;

            rect.left = getPos().x * getSize().x * getTileSize().x;

            rect.top = getPos().y * getSize().y * getTileSize().y;
            rect.top += line * getTileSize().y;

            rect.width = getSize().x * getTileSize().x;

            rect.height = getTexSize().y;

            if (isIsometric())
            {
                if (line%2==1)
                {
                    rect.left += 0.5f * getTileSize().x;
                    rect.top += 0.5f * getTileSize().y;
                }
                //rect.height = getTexSize().y;

                // add layer height modification
                //rect.top -=  * layer;
            }

            if (viewRect.intersects(rect))
                */mLayers[layer].render(line,target,states);
        }
        else
        {
            mLayers[layer].render(line,target,states);
        }
    }
    else if (layer < 0)
    {
        #ifdef OW_DEBUG
        std::cout << "Chunk: Uncorrect Layer : " << layer << std::endl;
        #endif // OW_DEBUG
    }
}

void Chunk::setPos(sf::Vector2i pos)
{
    mPos = pos;
    if (mParent != nullptr)
        sf::Transformable::setPosition(sf::Vector2f(mPos.x * mParent->getTileSize().x * mParent->getChunkSize().x, mPos.y * mParent->getTileSize().y * mParent->getChunkSize().y * 0.5f));
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

sf::Vector2i Chunk::getTexSize() const
{
    if (mParent != nullptr)
        return mParent->getTexSize();
    return sf::Vector2i(0,0);
}

bool Chunk::isIsometric() const
{
    if (mParent != nullptr)
        return mParent->isIsometric();
    return false;
}

sf::FloatRect Chunk::getBounds() const
{
    if (mParent != nullptr)
    {
        if (mParent->getChunkSize().y%2 == 0)
        {
            return sf::FloatRect(getPosition(),sf::Vector2f(mParent->getChunkSize().x * mParent->getTileSize().x + 0.5f * mParent->getTileSize().x, mParent->getChunkSize().y * mParent->getTileSize().y * 0.5f + 0.5f * mParent->getTileSize().y + mParent->getTexSize().y - mParent->getTileSize().y));
        }
        else
        {
            return sf::FloatRect(getPosition(),sf::Vector2f(mParent->getChunkSize().x * mParent->getTileSize().x + 0.5f * mParent->getTileSize().x, mParent->getChunkSize().y * mParent->getTileSize().y * 0.5f + mParent->getTexSize().y - mParent->getTileSize().y));
        }
    }
    return sf::FloatRect(getPosition(),sf::Vector2f(0,0));
}

Layer* Chunk::getLayer(unsigned int layer)
{
    if (layer >= 0 && layer < getLayerCount())
    {
        return &mLayers[layer];
    }
    else
    {
        #ifdef OW_DEBUG
        std::cout << "Chunk: Uncorrect Layer : " << layer << std::endl;
        #endif // OW_DEBUG
    }
    return nullptr;
}

void Chunk::addLayer()
{
    mLayers.push_back(Layer(this));
}

void Chunk::clearLayers()
{
    mLayers.clear();
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

void Chunk::uncompressLine(std::string& line)
{
    std::string tempLine = line;
    if (!CompressionUtils::base64_decode(tempLine))
    {
        return;
    }
    if (!CompressionUtils::decompressString(tempLine))
    {
        return;
    }
    line = tempLine;
}

std::string Chunk::compressLine(std::string const& line)
{
    std::string tempLine = line;
    CompressionUtils::compressString(tempLine);
    CompressionUtils::base64_encode(tempLine);
    return tempLine;
}

} // ow
