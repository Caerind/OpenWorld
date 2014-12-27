#ifndef LAYER_HPP
#define LAYER_HPP

#include <iostream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace owi
{

class Chunk;

class Layer
{
    public:
        Layer(Chunk* parent);

        void setTileId(int x, int y, int id);
        int getTileId(int x, int y);

        void render(sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

    private:
        sf::VertexArray mTiles;
        Chunk* mParent;
};

}

#endif // LAYER_HPP
