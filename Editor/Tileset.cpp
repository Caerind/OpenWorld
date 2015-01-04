#include "Tileset.hpp"
#include "Editor.hpp"

namespace owe
{

Tileset::Tileset(Editor* editor)
: mEditor(editor)
, mTexture(nullptr)
{
}

bool Tileset::loadFromFile(std::string const& filename)
{
    std::string temp;
    if (mEditor != nullptr)
    {
        temp = mEditor->getDirectory() + filename;
    }
    else
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: isn't linked to a Map" << std::endl;
        #endif // OW_DEBUG
        temp = filename;
    }
    std::ifstream file(temp);
    if (!file)
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: Cannot read file : " << temp << std::endl;
        #endif // OW_DEBUG
        return false;
    }
    mFilename = filename;
    std::string textureFilename;
    std::getline(file,textureFilename);
    file.close();
    mTexture = std::make_shared<sf::Texture>();
    if (mEditor != nullptr)
    {
        textureFilename = mEditor->getDirectory() + textureFilename;
    }
    if (!mTexture->loadFromFile(textureFilename))
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: Cannot read Texture : " << textureFilename << std::endl;
        #endif // OW_DEBUG
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
    if (mTexture != nullptr && id >= 0 && mEditor != nullptr)
    {
        int x = (id%(mTexture->getSize().x/mEditor->getTexSize().x)) * mEditor->getTexSize().x;
        int y = (id/(mTexture->getSize().x/mEditor->getTexSize().x)) * mEditor->getTexSize().y;
        return sf::Vector2f(x,y);
    }
    else if (mEditor == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: isn't linked to a Map" << std::endl;
        #endif // OW_DEBUG
    }
    else if (mTexture == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: Cannot access to Texture" << std::endl;
        #endif // OW_DEBUG
    }
    else if (id < 0)
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: id required < 0 : " << id << std::endl;
        #endif // OW_DEBUG
    }
    return sf::Vector2f(0,0);
}

int Tileset::getId(sf::Vector2f texCoords) const
{
    if (mTexture != nullptr && texCoords.x >= 0 && texCoords.y >= 0 && mEditor != nullptr)
    {
        int x = texCoords.x / mEditor->getTexSize().x;
        int y = texCoords.y / mEditor->getTexSize().y;
        int tilePerLine = mTexture->getSize().x / mEditor->getTexSize().x;
        int id = x + y * tilePerLine;
        return id;
    }
    else if (mEditor == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: isn't linked to a Map" << std::endl;
        #endif // OW_DEBUG
    }
    else if (mTexture == nullptr)
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: Cannot access to Texture" << std::endl;
        #endif // OW_DEBUG
    }
    else
    {
        #ifdef OW_DEBUG
        std::cout << "Tileset: Uncorrect Texture Rect : " << texCoords.x << " " << texCoords.y << std::endl;
        #endif // OW_DEBUG
    }
    return 0;
}

std::string Tileset::getFilename() const
{
    return mFilename;
}

} // owe
