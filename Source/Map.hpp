#ifndef MAP_HPP
#define MAP_HPP

#include <array>
#include <map>

#include <SFML/Graphics/Transformable.hpp>

#include "Chunk.hpp"
#include "Tileset.hpp"
#include "ChunkGenerator.hpp"

namespace owi
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
        Map(std::string const& directory, sf::Vector2i chunkSize, sf::Vector2i tileSize = sf::Vector2i(64,32), sf::Vector2i texSize = sf::Vector2i(64,64), ChunkGenerator* generator = nullptr);

        sf::Vector2i getChunkSize() const;
        sf::Vector2i getTileSize() const;
        sf::Vector2i getTexSize() const;
        std::string getDirectory() const;

        Update update(sf::Vector2f position);

        void render(unsigned int layer, sf::RenderTarget& target) const;
        void render(unsigned int layer, sf::RenderTarget& target, sf::FloatRect viewRect) const;

        Tileset::Ptr getTileset(std::string const& filename);
        bool loadTileset(std::string const& filename);

        void initChunks(sf::Vector2i pos = sf::Vector2i(0,0));
        void loadChunks(sf::Vector2i pos);

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
        sf::Vector2i mChunkSize;
        sf::Vector2i mTileSize;
        sf::Vector2i mTexSize;
        std::string mDirectory;
        ChunkGenerator* mGenerator;
        std::array<std::array<Chunk,3>,3> mChunks;
        std::map<std::string,Tileset::Ptr> mTilesets;
};

}

#endif // MAP_HPP
