#ifndef OW_MAP_HPP
#define OW_MAP_HPP

#include <array>
#include <map>

#include <SFML/Graphics/Transformable.hpp>

#ifdef OW_INFO
#include <SFML/System/Clock.hpp>
#endif // OW_INFO

#include "MapSettings.hpp"
#include "Chunk.hpp"
#include "Tileset.hpp"
#include "ChunkGenerator.hpp"

namespace ow
{

class Map : public sf::Transformable
{
    public:
        enum Update
        {
            Nothing,
            TL,
            T,
            TR,
            L,
            R,
            BL,
            B,
            BR,
        };

    public:
        Map(MapSettings const& settings);

        sf::Vector2i getChunkSize() const;
        sf::Vector2i getTileSize() const;
        sf::Vector2i getTexSize() const;
        std::string getDirectory() const;
        bool isDataCompressed() const;
        bool isIsometric() const;

        Update update(sf::Vector2f position);

        void render(unsigned int layer, sf::RenderTarget& target) const;
        void render(unsigned int layer, sf::RenderTarget& target, sf::FloatRect viewRect) const;

        Tileset::Ptr getTileset(std::string const& filename);
        bool loadTileset(std::string const& filename);

        void initChunks(sf::Vector2i pos = sf::Vector2i(0,0));
        void loadChunks(sf::Vector2i pos);
        void requestChunk(sf::Vector2i pos);

    private:
        void createDirectory(std::string const& filename);

        void moveTL(sf::Vector2i pos);
        void moveT(sf::Vector2i pos);
        void moveTR(sf::Vector2i pos);
        void moveL(sf::Vector2i pos);
        void moveR(sf::Vector2i pos);
        void moveBL(sf::Vector2i pos);
        void moveB(sf::Vector2i pos);
        void moveBR(sf::Vector2i pos);

        unsigned int getMaxLayer() const;

    private:
        MapSettings mSettings;
        std::array<std::array<Chunk,3>,3> mChunks;
        std::map<std::string,Tileset::Ptr> mTilesets;
};

} // ow

#endif // OW_MAP_HPP
