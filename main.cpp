#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <sstream>

using namespace sf;

const int edgeUp = 0, edgeDown = 560, edgeRight = 860, edgeLeft = 0;
const std::string play1 = "Points Player 1: ", play2 = "Points Player 2: ";
int state = 1, box1X = 700, box1Y = 280, box2X = 200, box2Y = 280, pointsP1 = 0, pointsP2 = 0;
bool keyUp = false, keyDown = false, keyRight = false, keyLeft = false;
bool keyA = false, keyS = false, keyD = false, keyW = false;
std::stringstream sst;
Texture menu, background, box1, box2, gameOver, playerWin1, playerWin2, target;
Music music;
SoundBuffer overlaps;
Text pp1, pp2;
Font font;

void setTargetPosition(Sprite * t){
    srand(time(NULL));

    int randX = rand();
    int randY = rand();

    randX = rand()%853;
    randY = rand()%553;

    while(randX!=t->getPosition().x && randY!=t->getPosition().y){
        randX = rand()%853;
        randY = rand()%553;
    }

    t->setPosition(randX, randY);
}

void intersection(Sprite b1, Sprite b2, Sprite * t, Sound * s, Text * f1, Text * f2){
    RectangleShape rt, rb1, rb2;
    Vector2<float> v1(t->getPosition().x, t->getPosition().y);
    Vector2<float> v2(b1.getPosition().x, b1.getPosition().y);
    Vector2<float> v3(b2.getPosition().x, b2.getPosition().y);
    Vector2<float> v4(40, 40);
    Vector2<float> v5(47, 47);

    rt.setPosition(v1);
    rt.setSize(v5);
    rb1.setPosition(v2);
    rb1.setSize(v4);
    rb2.setPosition(v3);
    rb2.setSize(v4);

    sst.str(std::string());

    if(rt.getGlobalBounds().intersects(rb1.getGlobalBounds())){
        setTargetPosition(t);
        s->play();
        sst << play1 << pointsP1;
        f1->setString(sst.str());
        pointsP1++;
    }
    else if(rt.getGlobalBounds().intersects(rb2.getGlobalBounds())){
        setTargetPosition(t);
        s->play();
        sst << play2 << pointsP2;
        f2->setString(sst.str());
        pointsP2++;
    }
}

void win(Sprite * go, Sprite * win1, Sprite * win2){
    if(pointsP1>=20){
        state = 3;
        go = win1;
    }
    else if(pointsP2>=20){
        state = 3;
        go = win2;
    }
}

int main()
{
    RenderWindow app(VideoMode(900, 600), "Box Game");

    app.setKeyRepeatEnabled(true);

    menu.loadFromFile("bin/menu.png");
    background.loadFromFile("bin/back.jpg");
    box1.loadFromFile("bin/box2.png");
    box2.loadFromFile("bin/box3.png");
    playerWin1.loadFromFile("bin/p1Win.png");
    playerWin2.loadFromFile("bin/p2Win.png");
    target.loadFromFile("bin/target.png");

    Sprite smenu(menu);
    Sprite sback(background);
    Sprite sbox1(box1);
    Sprite sbox2(box2);
    Sprite sgameOver;
    Sprite p1win(playerWin1);
    Sprite p2win(playerWin2);
    Sprite starget(target);

    sbox1.setPosition(box1X, box1Y);
    sbox2.setPosition(box2X, box2Y);
    setTargetPosition(&starget);

    music.openFromFile("bin/battle.ogg");
    music.play();
    music.setLoop(true);

    overlaps.loadFromFile("bin/cloth-inventory.WAV");
    Sound point(overlaps);

    font.loadFromFile("bin/font.ttf");
    pp1.setFont(font);
    pp2.setFont(font);
    pp1.setString(play1+"0");
    pp2.setString(play2+"0");
    pp1.setPosition(10, 10);
    pp2.setPosition(600, 10);

    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            switch(state){
            case 1:
                switch(event.type){
                case Event::Closed:
                    app.close();

                case Event::KeyPressed:
                    if(Keyboard::isKeyPressed(Keyboard::Escape))
                        app.close();
                    else if(Keyboard::isKeyPressed(Keyboard::Space))
                        state = 2;
                }
                break;

            case 2:
                switch(event.type){
                case Event::Closed:
                    app.close();

                case Event::KeyPressed:
                    if(Keyboard::isKeyPressed(Keyboard::Escape))
                        app.close();

                    if(Keyboard::isKeyPressed(Keyboard::Return))
                        state = 3;

                    keyUp = Keyboard::isKeyPressed(Keyboard::Up);
                    keyDown = Keyboard::isKeyPressed(Keyboard::Down);
                    keyRight = Keyboard::isKeyPressed(Keyboard::Right);
                    keyLeft = Keyboard::isKeyPressed(Keyboard::Left);
                    keyW = Keyboard::isKeyPressed(Keyboard::W);
                    keyS = Keyboard::isKeyPressed(Keyboard::S);
                    keyD = Keyboard::isKeyPressed(Keyboard::D);
                    keyA = Keyboard::isKeyPressed(Keyboard::A);

                    if(keyUp)
                        if(box1Y>edgeUp)
                            box1Y -= 4;
                    if(keyDown)
                        if(box1Y<edgeDown)
                            box1Y += 4;
                    if(keyLeft)
                        if(box1X>edgeLeft)
                            box1X -= 4;
                    if(keyRight)
                        if(box1X<edgeRight)
                            box1X += 4;
                    if(keyW)
                        if(box2Y>edgeUp)
                            box2Y -= 4;
                    if(keyS)
                        if(box2Y<edgeDown)
                            box2Y += 4;
                    if(keyA)
                        if(box2X>edgeLeft)
                            box2X -= 4;
                    if(keyD)
                        if(box2X<edgeRight)
                            box2X += 4;

                    sbox1.setPosition(box1X, box1Y);
                    sbox2.setPosition(box2X, box2Y);
                    intersection(sbox1, sbox2, &starget, &point, &pp1, &pp2);
                    win(&sgameOver, &p1win, &p2win);
                }
                break;

            case 3:
                switch(event.type){
                case Event::Closed:
                    app.close();

                case Event::KeyPressed:
                    if(Keyboard::isKeyPressed(Keyboard::Escape))
                        app.close();
                    else if(Keyboard::isKeyPressed(Keyboard::Space)){
                        state = 2;
                        box1X = 700; box1Y = 280; box2X = 200; box2Y = 280;
                        sbox1.setPosition(box1X, box1Y);
                        sbox2.setPosition(box2X, box2Y);
                        pointsP1 = 0;
                        pointsP2 = 0;
                        pp1.setString(play1+"0");
                        pp2.setString(play2+"0");
                    }
                }
            }
        }

        app.clear();

        switch(state){
        case 1:
            app.draw(smenu);
            break;

        case 2:
            app.draw(sback);
            app.draw(sbox1);
            app.draw(sbox2);
            app.draw(starget);
            app.draw(pp1);
            app.draw(pp2);
            break;

        case 3:
            if(pointsP1>=20)
                app.draw(p1win);
            else if(pointsP2>=20)
                app.draw(p2win);
        }

        app.display();
    }

    return EXIT_SUCCESS;
}
