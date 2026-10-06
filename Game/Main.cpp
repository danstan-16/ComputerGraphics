#include "Engine.h"
#include "SpaceGame/Player.h"
#include "SpaceGame/Enemy.h"
#include "SpaceGame/Assets.h"
#include "SpaceGame/SpaceGame.h"
#include "SpaceGame/Bullet.h"
#include "SpriteGame/SpriteGame.h"

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <random>

using namespace nu;

int main()
{
    nu::SetWorkingDirectory("Assets"); 
    
    // INITIALIZATION
    Engine::Get().Initialize();

    std::unique_ptr<SpriteGame> game = std::make_unique<SpriteGame>();
    game->Initialize();     

    std::vector<Vector2> points;
    
    // handle events
    bool quit = false;

    while (!quit) {

        SDL_Event e;

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }

            if (e.type == SDL_EVENT_KEY_DOWN && e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                quit = true;
            }
        }

        Engine::Get().Update();

        float dt = Engine::Get().GetTime().GetDeltaTime();

        game->Update(dt);

        Engine::Get().GetRenderer().SetColor(0.0f, 0.0f, 0.0f);
        Engine::Get().GetRenderer().Clear();

        game->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());
        Engine::Get().GetRenderer().Present();
    }
    game.reset();
    Engine::Get().Shutdown();

    return 0;
}