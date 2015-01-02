#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"
#include <fstream>

#include "../Generators/MyFirstChunkGenerator.hpp"

void handleEvent(sf::RenderWindow& window);
sf::Vector2f movement(sf::Time dt, float speed);
void updateView(sf::Vector2f playerPosition, sf::View& view, sf::FloatRect& viewRect);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    sf::Clock clock;
    sf::View view = window.getView();
    sf::FloatRect viewRect;

    owi::MapSettings settings;
    settings.directory = "Data/";
    settings.chunkSize = sf::Vector2i(32,64);
    settings.tileSize = sf::Vector2i(64,32);
    settings.texSize = sf::Vector2i(64,64);
    settings.onlineMode = false;
    settings.isServer = false;
    settings.generator = new MyFirstChunkGenerator();
    settings.compressedData = false;

    owi::Map map(settings);

    while (window.isOpen())
    {
        //
        // Event
        //
        handleEvent(window);

        //
        // Update
        //
        sf::Time dt = clock.restart();
        sf::Vector2f mvt = movement(dt,200);
        view.move(mvt);

        map.update(view.getCenter());

        updateView(view.getCenter(),view,viewRect);

        //
        // Render
        //
        window.clear();
        window.setView(view);
        map.render(0,window,viewRect);
        window.display();
    }

    return EXIT_SUCCESS;
}









void handleEvent(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}









sf::Vector2f movement(sf::Time dt, float speed)
{
    sf::Vector2f movement;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        movement.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        movement.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += speed;
    }
    movement.x *= dt.asSeconds();
    movement.y *= dt.asSeconds();
    return movement;
}





void updateView(sf::Vector2f playerPosition, sf::View& view, sf::FloatRect& viewRect)
{
    view.setCenter(playerPosition);
    viewRect = sf::FloatRect(view.getCenter()-view.getSize()*0.5f,view.getSize());
}
