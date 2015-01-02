#ifndef ISOMETRICGENERATOR_HPP
#define ISOMETRICGENERATOR_HPP

#include <cmath>
#include <ctime>

#include "ChunkGenerator.hpp"

class IsometricGenerator : public ow::ChunkGenerator
{
    public:
        IsometricGenerator();

        void createChunk(ow::Chunk& chunk, sf::Vector2i pos);
};

#endif // ISOMETRICGENERATOR_HPP
