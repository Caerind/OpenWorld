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

    clock.restart();

    owi::Map map("Data/",sf::Vector2i(32,64),sf::Vector2i(64,32),sf::Vector2i(64,64),new MyFirstChunkGenerator());

    std::cout << clock.restart().asSeconds() << std::endl;

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
        sf::Vector2f mvt = movement(dt,800);
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
