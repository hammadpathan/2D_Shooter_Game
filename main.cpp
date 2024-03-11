#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>

using namespace sf;
using namespace std;

class Bullet
{
public:
    CircleShape circle;
    Vector2f currVelocity;
    float maxSpeed;

    Bullet(float radius = 5.0f)
        : currVelocity(0.0f, 0.0f), maxSpeed(15.0f)
    {
        circle.setRadius(radius);
        circle.setFillColor(Color::Red);
    }
};



int main() {

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(512, 512), "2D Shooter Game");
    window.setFramerateLimit(144);
    int width = 512;
    int height = 512;

    // Player
    sf::CircleShape player(25.0f);
    //sf::CircleShape shape(100.f);
    player.setFillColor(sf::Color::Green);

    // Enemy
    RectangleShape enemy;
    enemy.setFillColor(Color::Magenta);
    enemy.setSize(Vector2f(50.0f, 50.0f));
    int spawnCounter = 20;

    vector<RectangleShape> enemies;

    // Bullets
    Bullet b1;

    vector<Bullet> bullets;
    //bullets.push_back(Bullet(b1)); // add element to vector



    // Vectors
    Vector2f playerCenter;
    Vector2f mousePosWindow;
    Vector2f aimDir;
    Vector2f aimDirNorm;

    // LENGTH OF VECTOR: |V| = sqrt(V.x^2 + V.y^2)
    // NORMALIZE VECTOR: U = V / |V|



    while (window.isOpen())
    {
        sf::Event evnt;

        while (window.pollEvent(evnt))
        {

            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::TextEntered:
                printf("%c", evnt.text.unicode);
                break;
            case sf::Event::Resized:
                printf("The new size is width %i and height %i\n", evnt.size.width, evnt.size.height);
                width = evnt.size.width;
                height = evnt.size.height;
                window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (player.getPosition().x <= 0) {}
            else
            {
                player.move(sf::Vector2f(-5.0f, 0.0f));
            }
        }
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (player.getPosition().x + 53 > width) {}
            else {
                player.move(sf::Vector2f(5.0f, 0.0f));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (player.getPosition().y <= 0) {}
            else {
                player.move(sf::Vector2f(0.0f, -5.0f));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (player.getPosition().y + 53 > height) {}
            else {
                player.move(sf::Vector2f(0.0f, 5.0f));
            }
        }


        //Update
        playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
        //cout << "playercenter = " << playerCenter.x << "," << playerCenter.y;
        mousePosWindow = Vector2f(Mouse::getPosition(window));
        aimDir = mousePosWindow - playerCenter;                                         // line between player and mouse position
        aimDirNorm = Vector2f(aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)), aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));


        // Enemies
        if (spawnCounter < 20)
        {
            spawnCounter++;
        }
        if (spawnCounter >= 20 && enemies.size() < 20)
        {
            enemy.setPosition(Vector2f(rand() % window.getSize().x, rand() % window.getSize().y));
            enemies.push_back(RectangleShape(enemy));

            spawnCounter = 0;
        }

        cout << spawnCounter << "\n";
        
        // Shooting

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            b1.circle.setPosition(playerCenter);
            b1.currVelocity = aimDirNorm * b1.maxSpeed;

            bullets.push_back(Bullet(b1));
        }

        for (size_t i = 0; i < bullets.size(); i++) 
        {
            bullets[i].circle.move(bullets[i].currVelocity);
           
            // Out of Bounds
            if (bullets[i].circle.getPosition().x < 0 || bullets[i].circle.getPosition().x >= window.getSize().x
                || bullets[i].circle.getPosition().y < 0 || bullets[i].circle.getPosition().y >= window.getSize().y)
            {
                bullets.erase(bullets.begin() + i);
            }

            // Enemy Collision
            else
            {
                for (size_t k = 0; k < enemies.size(); k++) 
                {
                    if (bullets[i].circle.getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
                    {
                        bullets.erase(bullets.begin() + i);
                        enemies.erase(enemies.begin() + k);
                        break;
                    }
                }

            }

        }

         
        // Draw
        window.clear();

        for (size_t i = 0; i < enemies.size(); i++)
        {
            window.draw(enemies[i]);
        }

        window.draw(player);

        for (size_t i = 0; i < bullets.size(); i++) 
        {
            window.draw(bullets[i].circle);
        }


        window.display();
    }


    return 0;
}