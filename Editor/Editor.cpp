#include "Editor.hpp"

#include <SFML/Graphics.hpp>

Editor::Editor()
{
    mWindow.create(sf::VideoMode(1200,600),"Map Editor");

    mMapSpeed = 600.f;
    mMapRect = sf::FloatRect(0,0,800,600);
    mMapView.reset(mMapRect);
    mMapView.setViewport(sf::FloatRect(0,0,0.75,1));

    mTilesetSpeed = 800.f;
    mTilesetRect = sf::FloatRect(800,0,400,600);
    mTilesetView.reset(mTilesetRect);
    mTilesetView.setViewport(sf::FloatRect(0.75,0,0.25,1));

    mFont.loadFromFile("");


}

void Editor::run()
{
    sf::Clock clock;
    while (mWindow.isOpen())
    {
        handleEvent();
        update(clock.restart());
        render();
    }
}

void Editor::handleEvent()
{
    sf::IntRect rButtonNew = static_cast<sf::IntRect>(mButtonNew.getGlobalBounds());
    sf::IntRect rButtonOpen = static_cast<sf::IntRect>(mButtonOpen.getGlobalBounds());
    sf::IntRect rButtonSave = static_cast<sf::IntRect>(mButtonSave.getGlobalBounds());
    sf::IntRect rButtonLayerP = static_cast<sf::IntRect>(mButtonLayerP.getGlobalBounds());
    sf::IntRect rButtonLayerM = static_cast<sf::IntRect>(mButtonLayerM.getGlobalBounds());

    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);

    sf::Event event;
    if (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            stop();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if (rButtonNew.contains(mousePos))
            {

            }
            else if (rButtonOpen.contains(mousePos))
            {

            }
            else if (rButtonSave.contains(mousePos))
            {

            }
            else if (rButtonLayerP.contains(mousePos))
            {

            }
            else if (rButtonLayerM.contains(mousePos))
            {

            }
            else if (static_cast<sf::IntRect>(mMapRect).contains(mousePos))
            {

            }
            else if (static_cast<sf::IntRect>(mTilesetRect).contains(mousePos))
            {

            }
        }
    }
}

void Editor::update(sf::Time dt)
{
    sf::Vector2f movementMap;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        movementMap.y -= mMapSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        movementMap.x -= mMapSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movementMap.y += mMapSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movementMap.x += mMapSpeed;
    movementMap.x *= dt.asSeconds();
    movementMap.y *= dt.asSeconds();
    mMapView.move(movementMap);

    sf::Vector2f movementTileset;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        movementTileset.y -= mTilesetSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        movementTileset.x -= mTilesetSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        movementTileset.y += mTilesetSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        movementTileset.x += mTilesetSpeed;
    movementTileset.x *= dt.asSeconds();
    movementTileset.y *= dt.asSeconds();
    mTilesetView.move(movementTileset);
}

void Editor::render()
{
    mWindow.clear(sf::Color(128,128,128));

    mWindow.setView(mMapView);
    /*for (unsigned int i = 0; i < mChunks.size(); i++)
    {
        mChunks[i].render(mWindow,mMapView);
    }*/

    mWindow.setView(mTilesetView);

    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(400,600));
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(sf::Vector2f(200,300));
    shape.setPosition(mTilesetView.getCenter());
    mWindow.draw(shape);

    //sf::Sprite texture;
    //texture.setTexture();
    //mWindow.draw(texture);

    mWindow.display();
}

void Editor::stop()
{


    mWindow.close();
}
