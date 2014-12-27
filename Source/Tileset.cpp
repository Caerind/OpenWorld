#include "Tileset.hpp"
#include "Map.hpp"

namespace owi
{

Tileset::Tileset(Map* map)
: mMap(map)
, mTexture(nullptr)
{
}

bool Tileset::loadFromFile(std::string const& filename)
{
    std::string temp;
    if (mMap != nullptr)
    {
        temp = mMap->getDirectory() + filename;
    }
    else
    {
        #ifdef DEBUG
        std::cout << "Tileset: isn't linked to a Map" << std::endl;
        #endif // DEBUG
        temp = filename;
    }
    std::ifstream file(temp);
    if (!file)
    {
        #ifdef DEBUG
        std::cout << "Tileset: Cannot read file : " << temp << std::endl;
        #endif // DEBUG
        return false;
    }
    mFilename = filename;
    std::string textureFilename;
    std::getline(file,textureFilename);
    file.close();
    mTexture = std::make_shared<sf::Texture>();
    if (mMap != nullptr)
    {
        textureFilename = mMap->getDirectory() + textureFilename;
    }
    if (!mTexture->loadFromFile(textureFilename))
    {
        #ifdef DEBUG
        std::cout << "Tileset: Cannot read Texture : " << textureFilename << std::endl;
        #endif // DEBUG
        return false;
    }
    return true;
}

std::shared_ptr<sf::Texture> Tileset::getTexture() const
{
    return mTexture;
}

sf::Vector2f Tileset::getTexCoords(int id) const
{
    if (mTexture != nullptr && id >= 0 && mMap != nullptr)
    {
        int x = (id%(mTexture->getSize().x/mMap->getTileSize().x)) * mMap->getTileSize().x;
        int y = (id/(mTexture->getSize().y/mMap->getTileSize().y)) * mMap->getTileSize().y;
        return sf::Vector2f(x,y);
    }
    else if (mMap == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Tileset: isn't linked to a Map" << std::endl;
        #endif // DEBUG
    }
    else if (mTexture == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Tileset: Cannot access to Texture" << std::endl;
        #endif // DEBUG
    }
    else if (id < 0)
    {
        #ifdef DEBUG
        std::cout << "Tileset: id required < 0 : " << id << std::endl;
        #endif // DEBUG
    }
    return sf::Vector2f(0,0);
}

int Tileset::getId(sf::Vector2f texCoords) const
{
    if (mTexture != nullptr && texCoords.x >= 0 && texCoords.y >= 0 && mMap != nullptr)
    {
        return texCoords.x / mMap->getTileSize().x + texCoords.y / mMap->getTileSize().y * mMap->getTileSize().x / mTexture->getSize().x;
    }
    else if (mMap == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Tileset: isn't linked to a Map" << std::endl;
        #endif // DEBUG
    }
    else if (mTexture == nullptr)
    {
        #ifdef DEBUG
        std::cout << "Tileset: Cannot access to Texture" << std::endl;
        #endif // DEBUG
    }
    else
    {
        #ifdef DEBUG
        std::cout << "Tileset: Uncorrect Texture Rect : " << texCoords.x << " " << texCoords.y << std::endl;
        #endif // DEBUG
    }
    return 0;
}

std::string Tileset::getFilename() const
{
    return mFilename;
}

}
