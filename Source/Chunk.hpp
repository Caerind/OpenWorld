#ifndef OW_CHUNK_HPP
#define OW_CHUNK_HPP

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

#include "Layer.hpp"
#include "Tileset.hpp"
#include "CompressionUtils.hpp"

namespace ow
{

class Map;

class Chunk : public sf::Transformable
{
    public:
        Chunk(Map* parent);

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
        Map* mParent;
        sf::Vector2i mPos;
        Tileset::Ptr mTileset;
        std::vector<Layer> mLayers;
};

} // ow

#endif // OW_CHUNK_HPP
