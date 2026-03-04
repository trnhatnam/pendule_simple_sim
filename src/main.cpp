#include <SFML/Graphics.hpp>
#include "pendule.h"
#include "namedPipeGuard.h"
#include <cmath>
#include <iostream>
#include <windows.h>

int main()
{
    float fps = 30.0;

    sf::RenderWindow window(sf::VideoMode({600, 600}), "My simulation");
    window.setFramerateLimit(fps);

	// création de la pendule
    Pendule p1({300,100}, 
                45*M_PI/180, // angle en radians
                300); // longueur du fil en pixel

    // variables initiales
    float u0 = 0;
    float v0 = p1.getAngle();
    float u1 = 0;
    float v1 = 0;
    float h = 1.0/fps;
    float g = 9.81 * 100; // mètres -> pixel ?
    float l = p1.getLongueur() ; // dépend de l'échelle cm -> pixel


    // création du pipe
    HANDLE pipe = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\pipePython"),
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, 1024, 1024, 0, NULL);
    if (pipe == INVALID_HANDLE_VALUE) 
        return 1;

    NamedPipeGuard pipeGuard(pipe); // RAII = Resource Acquisition Is Initialization
    std::cout << "En attente de connexion..." << std::endl;
    ConnectNamedPipe(pipeGuard.get(), NULL);
    DWORD bytesWritten;

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

        // Calcul de la prochaine position
        u1 = u0 - (h*g/l) * sin(v0);
        v1 = v0 + h*u1; // remplacer u0 par u1, lf Euler-Cromer
        u0 = u1;
        v0 = v1;

        
        // Écriture du message
        WriteFile(pipeGuard.get(), &v0, sizeof(v0), &bytesWritten, NULL);
        FlushFileBuffers(pipeGuard.get());

        // Mise à jour
        p1.setAngle(v0);
        
        
    }

    
    return 0;
}