#ifndef WORLDGENERATOR_HPP
#define WORLDGENERATOR_HPP

#include <cmath>
#include <ctime>
#include <vector>

#include "ChunkGenerator.hpp"

class WorldGenerator : public ow::ChunkGenerator
{
    public:
        WorldGenerator();

        void createChunk(ow::Chunk& chunk, sf::Vector2i pos);

        void fill(ow::Chunk& chunk, unsigned int tileId, unsigned int layer = 0);

        bool neighboorsChunks(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList, std::vector<sf::Vector2i>& transformed, ow::Chunk& chunk);
        bool testUpperChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList);
        bool testLeftChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList);
        bool testRightChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList);
        bool testLowerChunk(sf::Vector2i pos, std::vector<sf::Vector2i>& checkList);

        void handleChild(sf::Vector2i childTile, std::vector<sf::Vector2i>& checkList, std::vector<sf::Vector2i>& transformed, ow::Chunk& chunk);

    protected:
        int mWaterId;
        int mSandId;
        int mGrassId;
        int mPercentGrassOverWater;
        int mPercentFromNeighboors;
        int mPercentDefault;
        int mPercentBonus;
};

#endif // WORLDGENERATOR_HPP
