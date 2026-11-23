
#include "Engine.h"
#include "Core/file.h"
#include "Math/Transform.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Framework/Scene.h"
#include "Resources/ResourceManager.h"
#include "Components/CameraComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

using namespace nu;

int main()
{
    nu::SetWorkingDirectory("Assets"); 
    
    // INITIALIZATION
    Engine::Get().Initialize();    

    Scene scene;
    scene.Load("scenes/scene.json");

    auto renderer = Engine::Get().GetRenderer();

    // main loop
    bool quit = false;

    while (!quit) {

        SDL_Event e;

        while (SDL_PollEvent(&e)) 
        {
            Engine::Get().GetEditor().ProcessEvent(e);

            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }

            if (e.type == SDL_EVENT_KEY_DOWN && e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                quit = true;
            }
        }

        // ENGINE
        Engine::Get().Update();

        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        scene.Update(dt);

        Engine::Get().GetEditor().BeginFrame();
        Engine::Get().GetEditor().Update(scene);

        // RENDER
        renderer.BeginFrame();
        scene.Draw(renderer);

        renderer.EndRenderPass();
        Engine::Get().GetEditor().Draw(renderer);

        renderer.EndFrame();
    }
    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}