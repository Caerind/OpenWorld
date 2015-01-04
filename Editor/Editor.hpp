#ifndef OWE_EDITOR_HPP
#define OWE_EDITOR_HPP

#include <map>

#include <SFML/Graphics.hpp>

#include "Chunk.hpp"
#include "Tileset.hpp"

namespace owe
{

class Editor
{
    public:
        Editor();

        void run();
        void handleEvent();
        void update(sf::Time dt);
        void render();

        void stop();

        sf::Vector2i toIsoPos(sf::Vector2f pos);
        sf::Vector2i toOrthoPos(sf::Vector2f pos);

        Chunk getActualChunk(sf::Vector2f pos);

        std::string getDirectory() const;
        sf::Vector2i getChunkSize() const;
        sf::Vector2i getTileSize() const;
        sf::Vector2i getTexSize() const;
        bool isIsometric() const;
        bool isDataCompressed() const;

        Tileset::Ptr getTileset(std::string const& name);
        bool loadTileset(std::string const& filename);

    protected:
        unsigned int getMaxLayer() const;

    protected:
        bool mInitialized;

        sf::RenderWindow mWindow;

        float mMapSpeed;
        sf::FloatRect mMapRect;
        sf::View mMapView;

        float mTilesetSpeed;
        sf::FloatRect mTilesetRect;
        sf::View mTilesetView;

        sf::Font mFont;

        sf::Sprite mSprite;

        sf::RectangleShape mButtonNew;
        sf::RectangleShape mButtonOpen;
        sf::RectangleShape mButtonSave;
        sf::RectangleShape mButtonLayerP;
        sf::RectangleShape mButtonLayerM;

        sf::Text mTextNew;
        sf::Text mTextOpen;
        sf::Text mTextSave;
        sf::Text mTextLayerP;
        sf::Text mTextLayerM;

        unsigned int mActualId;
        unsigned int mActualLayer;

        std::string mDirectory;
        sf::Vector2i mChunkSize;
        sf::Vector2i mTileSize;
        sf::Vector2i mTexSize;
        bool mIsometric;
        bool mCompressed;

        std::map<std::string,Tileset::Ptr> mTilesets;

        std::array<std::array<Chunk,3>,3> mChunks;

};

} // owe

#endif // OWE_EDITOR_HPP
