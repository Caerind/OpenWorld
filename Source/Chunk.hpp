#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Layer.hpp"
#include "Tileset.hpp"

namespace owi
{

class Map;

class Chunk : public sf::Transformable
{
    public:
        Chunk(Map* parent);

        bool loadFromFile(std::string const& filename);
        bool saveToFile(std::string const& filename);

        void render(unsigned int layer, sf::RenderTarget& target, sf::RenderStates states) const;
        void render(unsigned int layer, sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect viewRect) const;

        void setPos(sf::Vector2i pos);
        sf::Vector2i getPos() const;
        sf::Vector2i getSize() const;
        sf::Vector2i getTileSize() const;

        sf::FloatRect getBounds() const;

        Layer* getLayer(unsigned int layer);
        unsigned int getLayerCount() const;

        void setTileset(Tileset::Ptr tileset);
        Tileset::Ptr getTileset() const;

    protected:
        Map* mParent;
        sf::Vector2i mPos;
        Tileset::Ptr mTileset;
        std::array<Layer,3> mLayers;
};

}

#endif // CHUNK_HPP
