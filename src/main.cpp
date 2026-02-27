#include <SFML/Graphics.hpp>
#include "pendule.h"
#include <cmath>
#include <iostream>

int main()
{
    float fps = 30.0;

    sf::RenderWindow window(sf::VideoMode({600, 600}), "My simulation");
    window.setFramerateLimit(fps);

	// création de la pendule
    Pendule p1({300,100});

    // variables initiales
    float u0 = 0;
    float v0 = p1.getAngle();
    float u1 = 0;
    float v1 = 0;
    float h = 1.0/fps;
    float g = 9.81 * 100; // mètres -> pixel ?
    float l = p1.getLongueur() ; // dépend de l'échelle cm -> pixel

    // fenetre
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(p1);
        
        window.display();

        // calcul de la prochaine position
        u1 = u0 - (h*g/l) * sin(v0);
        v1 = v0 + h*u1; // remplacer u0 par u1, lf Euler-Cromer
        u0 = u1;
        v0 = v1;

        //  mise à jour
        p1.setAngle(v0);
        //std::cout << v0 << std::endl;

    }
}