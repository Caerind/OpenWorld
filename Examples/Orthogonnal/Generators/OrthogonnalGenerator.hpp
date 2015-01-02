#ifndef ORTHOGONNALGENERATOR_HPP
#define ORTHOGONNALGENERATOR_HPP

#include <cmath>
#include <ctime>

#include "ChunkGenerator.hpp"

class OrthogonnalGenerator : public ow::ChunkGenerator
{
    public:
        OrthogonnalGenerator();

        void createChunk(ow::Chunk& chunk, sf::Vector2i pos);
};

#endif // ORTHOGONNALGENERATOR_HPP
