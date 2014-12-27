#include "Layer.hpp"
#include "Chunk.hpp"

namespace owi
{

Layer::Layer(Chunk* parent) : mParent(parent)
{
    #ifdef DEBUG
    if (mParent == nullptr)
    {
        std::cout << "Layer: Parent is nullptr in ctor !" << std::endl;
    }
    #endif // DEBUG

    if (mParent != nullptr)
    {
        mTiles.setPrimitiveType(sf::Quads);
        update();
    }
}

void Layer::setTileId(int x, int y, int id)
{
    if (mParent != nullptr)
    {
        if (x >= 0 && y >= 0 && x < mParent->getSize().x && y < mParent->getSize().y)
        {
            if (mParent->getTileset() != nullptr)
            {
                sf::Vector2f texCoords = mParent->getTileset()->getTexCoords(id);
                sf::Vertex* quad = &mTiles[(x + y * mParent->getSize().x) * 4];
                quad[0].texCoords = sf::Vector2f(texCoords.x, texCoords.y);
                quad[1].texCoords = sf::Vector2f(texCoords.x + mParent->getTileSize().x, texCoords.y);
                quad[2].texCoords = sf::Vector2f(texCoords.x + mParent->getTileSize().x, texCoords.y + mParent->getTileSize().y);
                quad[3].texCoords = sf::Vector2f(texCoords.x, texCoords.y + mParent->getTileSize().y);
            }
            else
            {
                #ifdef DEBUG
                std::cout << "Layer: Cannot access to Tileset (setTileId)" << std::endl;
                #endif // DEBUG
            }
        }
        else
        {
            #ifdef DEBUG
            std::cout << "Layer: Incorrect value called" << std::endl;
            std::cout << "Layer: x = " << x << " & size = " << mParent->getSize().x << std::endl;
            std::cout << "Layer: y = " << y << " & size = " << mParent->getSize().y << std::endl;
            #endif // DEBUG
        }
    }
}

int Layer::getTileId(int x, int y)
{
    if (mParent != nullptr)
    {
        if (x >= 0 && y >= 0 && x < mParent->getSize().x && y < mParent->getSize().y)
        {
            sf::Vertex* quad = &mTiles[(x + y * mParent->getSize().x) * 4];
            if (mParent->getTileset() != nullptr)
            {
                return mParent->getTileset()->getId(quad[0].texCoords);
            }
            else
            {
                #ifdef DEBUG
                std::cout << "Layer: Cannot access to Tileset (getTileId)" << std::endl;
                #endif // DEBUG
            }
        }
        else
        {
            #ifdef DEBUG
            std::cout << "Layer: Incorrect value called" << std::endl;
            std::cout << "Layer: x = " << x << " & size = " << mParent->getSize().x << std::endl;
            std::cout << "Layer: y = " << y << " & size = " << mParent->getSize().y << std::endl;
            #endif // DEBUG
        }
    }
    return 0;
}

void Layer::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mParent != nullptr)
    {
        if (mParent->getTileset() != nullptr)
        {
            if (mParent->getTileset()->getTexture() != nullptr)
            {
                states.texture = mParent->getTileset()->getTexture().get();
                target.draw(mTiles,states);
            }
            else
            {
                #ifdef DEBUG
                std::cout << "Layer: Cannot access to Texture" << std::endl;
                #endif // DEBUG
            }
        }
        else
        {
            #ifdef DEBUG
            std::cout << "Layer: Cannot access to Tileset (render)" << std::endl;
            #endif // DEBUG
        }
    }
}

void Layer::update()
{
    if (mParent != nullptr)
    {
        mTiles.resize(mParent->getSize().x * mParent->getSize().y * 4);
        for (int j = 0; j < mParent->getSize().y; j++)
        {
            for (int i = 0; i < mParent->getSize().x; i++)
            {
                sf::Vertex* quad = &mTiles[(i + j * mParent->getSize().x) * 4];
                quad[0].position = sf::Vector2f(i * mParent->getTileSize().x, j * mParent->getTileSize().y);
                quad[1].position = sf::Vector2f((i + 1) * mParent->getTileSize().x, j * mParent->getTileSize().y);
                quad[2].position = sf::Vector2f((i + 1) * mParent->getTileSize().x, (j + 1) * mParent->getTileSize().y);
                quad[3].position = sf::Vector2f(i * mParent->getTileSize().x, (j + 1) * mParent->getTileSize().y);
            }
        }
    }
}

}
