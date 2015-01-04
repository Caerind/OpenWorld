#include "Editor.hpp"

namespace owe
{

Editor::Editor()
: mChunks({{ {Chunk(this),Chunk(this),Chunk(this)}
, {Chunk(this),Chunk(this),Chunk(this)}
, {Chunk(this),Chunk(this),Chunk(this)} }})
{
    mInitialized = false;
    mOverlay = false;

    mActualId = 0;
    mActualLayer = 0;

    mWindow.create(sf::VideoMode(1200,600),"Map Editor");

    mMapSpeed = 600;
    mMapRect = sf::FloatRect(0,0,800,600);
    mMapView.reset(mMapRect);
    mMapView.setViewport(sf::FloatRect(0,0,0.75,1));

    mTilesetSpeed = 800;
    mTilesetRect = sf::FloatRect(800,0,400,600);
    mTilesetView.reset(mTilesetRect);
    mTilesetView.setViewport(sf::FloatRect(0.75,0,0.25,1));

    mFont.loadFromFile("Sansation.ttf");

    mButtonNew.setSize(sf::Vector2f(200,50));
    mButtonNew.setFillColor(sf::Color::Green);
    mButtonNew.setPosition(0,550);
    mTextNew.setString("New");
    mTextNew.setFont(mFont);
    mTextNew.setPosition(25,555);

    mButtonOpen.setSize(sf::Vector2f(200,50));
    mButtonOpen.setFillColor(sf::Color::Green);
    mButtonOpen.setPosition(250,550);
    mTextOpen.setString("Open");
    mTextOpen.setFont(mFont);
    mTextOpen.setPosition(275,555);

    mButtonSave.setSize(sf::Vector2f(200,50));
    mButtonSave.setFillColor(sf::Color::Green);
    mButtonSave.setPosition(500,550);
    mTextSave.setString("Save");
    mTextSave.setFont(mFont);
    mTextSave.setPosition(525,555);

    mButtonLayerP.setSize(sf::Vector2f(200,50));
    mButtonLayerP.setFillColor(sf::Color::Green);
    mButtonLayerP.setPosition(750,550);
    mTextLayerP.setString("Layer +");
    mTextLayerP.setFont(mFont);
    mTextLayerP.setPosition(775,555);

    mButtonLayerM.setSize(sf::Vector2f(200,50));
    mButtonLayerM.setFillColor(sf::Color::Green);
    mButtonLayerM.setPosition(1000,550);
    mTextLayerM.setString("Layer -");
    mTextLayerM.setFont(mFont);
    mTextLayerM.setPosition(1025,555);
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
    if (!mOverlay && mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            stop();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if (rButtonNew.contains(mousePos))
            {
                openOverlay();

                if (mInitialized)
                {
                    // On sauvegarde
                }

                std::cout << "Enter the directory name (Need to exist) : ";
                std::cin >> mDirectory;

                std::string tilesetName;
                std::cout << "Default Tileset Name : ";
                std::cin >> tilesetName;
                loadTileset(mDirectory + tilesetName);
                if (mTileset != nullptr)
                    mSprite.setTexture(*mTileset->getTexture().get());

                std::cout << "Chunk Size X : ";
                std::cin >> mChunkSize.x;
                std::cout << "Chunk Size Y : ";
                std::cin >> mChunkSize.y;

                std::cout << "Tile Size X : ";
                std::cin >> mTileSize.x;
                std::cout << "Tile Size Y : ";
                std::cin >> mTileSize.y;

                std::cout << "Tex Size X : ";
                std::cin >> mTexSize.x;
                std::cout << "Tex Size Y : ";
                std::cin >> mTexSize.y;

                std::string c;
                std::cout << "Isometric ? : (Y/N)";
                std::cin >> c;
                if (c == "Y")
                    mIsometric = true;
                else
                    mIsometric = false;

                std::string c;
                std::cout << "Compressed ? : (Y/N)";
                std::cin >> c;
                if (c == "Y")
                    mCompressed = true;
                else
                    mCompressed = false;

                mInitialized = true;

                closeOverlay();
            }
            else if (rButtonOpen.contains(mousePos))
            {
                openOverlay();

                if (mInitialized)
                {
                    // On sauvegarde
                }

                // On ouvre la map en fonction des fichiers settings

                mInitialized = true;

                closeOverlay();
            }
            else if (rButtonSave.contains(mousePos))
            {
                if (mInitialized)
                {
                    for (int j = -1; j < 2; j++)
                    {
                        for (int i = -1; i < 2; i++)
                        {
                            mChunks[i+1][j+1].saveToFile(mDirectory + std::to_string(mChunks[i+1][j+1].getPos().x) + "_" + std::to_string(mChunks[i+1][j+1].getPos().y) + ".chunk");
                        }
                    }
                }
            }
            else if (rButtonLayerP.contains(mousePos))
            {
                mActualLayer++;
                std::cout << "New Layer : " << mActualLayer << std::endl;
            }
            else if (rButtonLayerM.contains(mousePos))
            {
                if (mActualLayer > 0)
                    mActualLayer--;
                std::cout << "New Layer : " << mActualLayer << std::endl;
            }
            else if (static_cast<sf::IntRect>(mMapRect).contains(mousePos))
            {
                // Il faudra prevoir le fait qu'on puisse cliquer sur différents layers et donc "bouger" la souris
                if (mInitialized)
                {
                    sf::Vector2f mouse = mWindow.mapPixelToCoords(mousePos,mMapView);
                    sf::Vector2i pos;
                    if (mIsometric)
                        pos = toIsoPos(mouse);
                    else
                        pos = toOrthoPos(mouse);
                    getActualChunk(mouse).getLayer(mActualLayer)->setTileId(pos.x,pos.y,mActualId);
                }
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



    float x = mMapView.getCenter().x / (mChunkSize.x * mTileSize.x);
    float y;
    if (isIsometric())
        y = mMapView.getCenter().y / (mChunkSize.y * mTileSize.y * 0.5f);
    else
        y = mMapView.getCenter().y / (mChunkSize.y * mTileSize.y);
    sf::Vector2i pos = sf::Vector2i(x,y);
    if (x < 0) pos.x--;
    if (y < 0) pos.y--;

    if (mInitialized)
    {
        if (mChunks[1][1].getPos() != pos)
        {
            for (int j = -1; j < 2; j++)
            {
                for (int i = -1; i < 2; i++)
                {
                    mChunks[i+1][j+1].saveToFile(mDirectory + std::to_string(mChunks[i+1][j+1].getPos().x) + "_" + std::to_string(mChunks[i+1][j+1].getPos().y) + ".chunk");
                    if (!mChunks[i+1][j+1].loadFromFile(mDirectory + std::to_string(pos.x+i) + "_" + std::to_string(pos.y+j) + ".chunk"))
                    {
                        mChunks[i+1][j+1].setTileset(mTileset);
                        mChunks[i+1][j+1].setPos(sf::Vector2i(pos.x+i,pos.y+j));
                        mChunks[i+1][j+1].clearLayers();
                        mChunks[i+1][j+1].addLayer();
                    }
                }
            }
        }
    }
    else
    {
        pos.x = 0;
        pos.y = 0;
        for (int j = -1; j < 2; j++)
        {
            for (int i = -1; i < 2; i++)
            {
                mChunks[i+1][j+1].setTileset(mTileset);
                mChunks[i+1][j+1].setPos(sf::Vector2i(pos.x+i,pos.y+j));
                mChunks[i+1][j+1].clearLayers();
            }
        }
    }
}

void Editor::render()
{
    mWindow.clear(sf::Color(128,128,128));

    mWindow.setView(mMapView);

    if (mInitialized)
    {
        sf::RenderStates states;
        unsigned int maxLayer = getMaxLayer();
        sf::Transform layerOffset;

        if (isIsometric())
            layerOffset.translate(0,-mTexSize.y+mTileSize.y);

        for (unsigned int h = 0; h < maxLayer; h++)
        {
            for (unsigned int j = 0; j < static_cast<unsigned int>(3 * mChunkSize.y); j++)
            {
                for (unsigned int i = 0; i < 3; i++)
                {
                    mChunks[i][j/mChunkSize.y].render(j%mChunkSize.y,h,mWindow,states);
                }
            }
            states.transform *= layerOffset;
        }
    }

    mWindow.setView(mTilesetView);

    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(400,600));
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(sf::Vector2f(200,300));
    shape.setPosition(mTilesetView.getCenter());
    mWindow.draw(shape);

    mWindow.draw(mSprite);

    mWindow.setView(mWindow.getDefaultView());

    mWindow.draw(mButtonNew);
    mWindow.draw(mTextNew);
    mWindow.draw(mButtonOpen);
    mWindow.draw(mTextOpen);
    mWindow.draw(mButtonSave);
    mWindow.draw(mTextSave);
    mWindow.draw(mButtonLayerP);
    mWindow.draw(mTextLayerP);
    mWindow.draw(mButtonLayerM);
    mWindow.draw(mTextLayerM);

    if (mOverlay)
    {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(800,400));
        overlay.setPosition(200,100);
        overlay.setFillColor(sf::Color(76,76,76));

        sf::Text text("Follow Console Instructions",mFont);
        text.setPosition(350,250);

        mWindow.draw(overlay);
        mWindow.draw(text);
    }

    mWindow.display();
}

void Editor::stop()
{
    if (mInitialized)
        for (unsigned int j = 0; j < 3; j++)
            for (unsigned int i = 0; i < 3; i++)
                mChunks[i][j].saveToFile(mDirectory + std::to_string(mChunks[i][j].getPos().x) + "_" + std::to_string(mChunks[i][j].getPos().y) + ".chunk");
    mWindow.close();
}

sf::Vector2i Editor::toIsoPos(sf::Vector2f pos)
{
    return sf::Vector2i(0,0);
}

sf::Vector2i Editor::toOrthoPos(sf::Vector2f pos)
{
    return sf::Vector2i(0,0);
}

Chunk Editor::getActualChunk(sf::Vector2f pos)
{
    if (mInitialized)
        for (unsigned int j = 0; j < 3; j++)
            for (unsigned int i = 0; i < 3; i++)
                if (mChunks[i][j].getBounds().contains(pos))
                    return mChunks[i][j];
    return mChunks[1][1];
}

std::string Editor::getDirectory() const
{
    return mDirectory;
}

sf::Vector2i Editor::getChunkSize() const
{
    return mChunkSize;
}

sf::Vector2i Editor::getTileSize() const
{
    return mTileSize;
}

sf::Vector2i Editor::getTexSize() const
{
    return mTexSize;
}

bool Editor::isIsometric() const
{
    return mIsometric;
}

bool Editor::isDataCompressed() const
{
    return mCompressed;
}

Tileset::Ptr Editor::getTileset(std::string const& filename)
{
    if (mTileset->getFilename() == filename)
    {
        return mTileset;
    }
    else
    {
        loadTileset(filename);
        return mTileset;
    }
}

bool Editor::loadTileset(std::string const& filename)
{
    mTileset = std::make_shared<Tileset>(this);
    if (!mTileset->loadFromFile(filename))
    {
        #ifdef OW_DEBUG
        std::cout << "Map: Cant load texture : " << filename << std::endl;
        #endif // OW_DEBUG
        return false;
    }
    return true;
}

void Editor::openOverlay()
{
    mOverlay = true;
    render();
}

void Editor::closeOverlay()
{
    mOverlay = false;
    std::cout << "Get Back To The Editor" << std::endl;
}

unsigned int Editor::getMaxLayer() const
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            ret = std::max(ret,mChunks[i][j].getLayerCount());
        }
    }
    return ret;
}

} // owe
