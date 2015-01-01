#ifndef LAYER_HPP
#define LAYER_HPP

#include <iostream>
#include <vector>

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

        void render(unsigned int line, sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

    private:
        std::vector<sf::VertexArray> mLines;
        Chunk* mParent;
};

} // owi

#endif // LAYER_HPP
