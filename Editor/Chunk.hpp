#ifndef OWE_CHUNK_HPP
#define OWE_CHUNK_HPP

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

#include "Layer.hpp"
#include "Tileset.hpp"
#include "CompressionUtils.hpp"

namespace owe
{

class Editor;

class Chunk : public sf::Transformable
{
    public:
        Chunk(Editor* parent);

        bool loadFromFile(std::string const& filename);
        bool saveToFile(std::string const& filename);

        void render(unsigned int line, unsigned int layer, sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect viewRect = sf::FloatRect(0,0,0,0)) const;

        void setPos(sf::Vector2i pos);
        sf::Vector2i getPos() const;
        sf::Vector2i getSize() const;
        sf::Vector2i getTileSize() const;
        sf::Vector2i getTexSize() const;
        bool isIsometric() const;

        sf::FloatRect getBounds() const;

        Layer* getLayer(unsigned int layer);
        void addLayer();
        void clearLayers();
        unsigned int getLayerCount() const;

        void setTileset(Tileset::Ptr tileset);
        Tileset::Ptr getTileset() const;

    protected:
        void uncompressLine(std::string& line);
        std::string compressLine(std::string const& line);

    protected:
        Editor* mParent;
        sf::Vector2i mPos;
        Tileset::Ptr mTileset;
        std::vector<Layer> mLayers;
};

} // owe

#endif // OWE_CHUNK_HPP
