#include "Layer.hpp"
#include "Chunk.hpp"

namespace owe
{

Layer::Layer(Chunk* parent) : mParent(parent)
{
    #ifdef OW_DEBUG
    if (mParent == nullptr)
    {
        std::cout << "Layer: Parent is nullptr in ctor !" << std::endl;
    }
    #endif // OW_DEBUG

    if (mParent != nullptr)
    {
        for (unsigned int j = 0; j < static_cast<unsigned int >(mParent->getSize().y); j++)
        {
            mLines.push_back(sf::VertexArray());
            mLines.back().setPrimitiveType(sf::Quads);
        }
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
                sf::Vertex* quad = &mLines[y][x * 4];
                quad[0].texCoords = sf::Vector2f(texCoords.x, texCoords.y);
                quad[1].texCoords = sf::Vector2f(texCoords.x + mParent->getTexSize().x, texCoords.y);
                quad[2].texCoords = sf::Vector2f(texCoords.x + mParent->getTexSize().x, texCoords.y + mParent->getTexSize().y);
                quad[3].texCoords = sf::Vector2f(texCoords.x, texCoords.y + mParent->getTexSize().y);
            }
            else
            {
                #ifdef OW_DEBUG
                std::cout << "Layer: Cannot access to Tileset (setTileId)" << std::endl;
                #endif // OW_DEBUG
            }
        }
        else
        {
            #ifdef OW_DEBUG
            std::cout << "Layer: Incorrect value called" << std::endl;
            std::cout << "Layer: x = " << x << " & size = " << mParent->getSize().x << std::endl;
            std::cout << "Layer: y = " << y << " & size = " << mParent->getSize().y << std::endl;
            #endif // OW_DEBUG
        }
    }
}

int Layer::getTileId(int x, int y)
{
    if (mParent != nullptr)
    {
        if (x >= 0 && y >= 0 && x < mParent->getSize().x && y < mParent->getSize().y)
        {
            sf::Vertex* quad = &mLines[y][x * 4];
            if (mParent->getTileset() != nullptr)
            {
                return mParent->getTileset()->getId(quad[0].texCoords);
            }
            else
            {
                #ifdef OW_DEBUG
                std::cout << "Layer: Cannot access to Tileset (getTileId)" << std::endl;
                #endif // OW_DEBUG
            }
        }
        else
        {
            #ifdef OW_DEBUG
            std::cout << "Layer: Incorrect value called" << std::endl;
            std::cout << "Layer: x = " << x << " & size = " << mParent->getSize().x << std::endl;
            std::cout << "Layer: y = " << y << " & size = " << mParent->getSize().y << std::endl;
            #endif // OW_DEBUG
        }
    }
    return 0;
}

void Layer::render(unsigned int line, sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mParent != nullptr && line >= 0 && line < mLines.size())
    {
        if (mParent->getTileset() != nullptr)
        {
            if (mParent->getTileset()->getTexture() != nullptr)
            {
                states.texture = mParent->getTileset()->getTexture().get();
                target.draw(mLines[line],states);
            }
            else
            {
                #ifdef OW_DEBUG
                std::cout << "Layer: Cannot access to Texture" << std::endl;
                #endif // OW_DEBUG
            }
        }
        else
        {
            #ifdef OW_DEBUG
            std::cout << "Layer: Cannot access to Tileset (render)" << std::endl;
            #endif // OW_DEBUG
        }
    }
}

void Layer::update()
{
    if (mParent != nullptr)
    {
        for (unsigned int j = 0; j < static_cast<unsigned int>(mParent->getSize().y); j++)
        {
            mLines[j].resize(mParent->getSize().x * 4);
            for (unsigned int i = 0; i < static_cast<unsigned int>(mParent->getSize().x); i++)
            {
                sf::Vertex* quad = &mLines[j][i * 4];
                if (mParent->isIsometric())
                {
                    if (j%2 == 0)
                    {
                        quad[0].position = sf::Vector2f(i * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f);
                        quad[1].position = sf::Vector2f((i + 1) * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f);
                        quad[2].position = sf::Vector2f((i + 1) * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f + mParent->getTexSize().y);
                        quad[3].position = sf::Vector2f(i * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f + mParent->getTexSize().y);
                    }
                    else
                    {
                        quad[0].position = sf::Vector2f((i + 0.5) * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f);
                        quad[1].position = sf::Vector2f((i + 1.5) * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f);
                        quad[2].position = sf::Vector2f((i + 1.5) * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f + mParent->getTexSize().y);
                        quad[3].position = sf::Vector2f((i + 0.5) * mParent->getTileSize().x, j * mParent->getTileSize().y * 0.5f + mParent->getTexSize().y);
                    }
                }
                else
                {
                    quad[0].position = sf::Vector2f(i * mParent->getTileSize().x, j * mParent->getTileSize().y);
                    quad[1].position = sf::Vector2f((i + 1) * mParent->getTileSize().x, j * mParent->getTileSize().y);
                    quad[2].position = sf::Vector2f((i + 1) * mParent->getTileSize().x, (j + 1) * mParent->getTileSize().y);
                    quad[3].position = sf::Vector2f(i * mParent->getTileSize().x, (j + 1) * mParent->getTileSize().y);
                }
            }
        }
    }
}

} // owe
