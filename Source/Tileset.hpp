#ifndef TILESET_HPP
#define TILESET_HPP

#include <fstream>
#include <iostream>
#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace owi
{

class Map;

class Tileset
{
    public:
        typedef std::shared_ptr<Tileset> Ptr;

    public:
        Tileset(Map* map);

        bool loadFromFile(std::string const& filename);

        std::shared_ptr<sf::Texture> getTexture() const;

        sf::Vector2f getTexCoords(int id) const;
        int getId(sf::Vector2f texCoords) const;

        std::string getFilename() const;

    private:
        Map* mMap;
        std::shared_ptr<sf::Texture> mTexture;
        std::string mFilename;
};

}

#endif // TILESET_HPP
