#ifndef OW_LAYER_HPP
#define OW_LAYER_HPP

#include <iostream>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace ow
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

} // ow

#endif // OW_LAYER_HPP
