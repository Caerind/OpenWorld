#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <SFML/Graphics.hpp>

class Editor
{
    public:
        Editor();

        void run();
        void handleEvent();
        void update(sf::Time dt);
        void render();

        void stop();

    protected:
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

        //std::vector<Chunk> mChunks;
};

#endif // EDITOR_HPP
