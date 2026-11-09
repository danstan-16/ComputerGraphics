#include "Engine.h"
#include "Core/file.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

//#define MAX_LIGHTS 8

using namespace nu;

//enum class LightType : uint32_t
//{
//    Directional,
//    Point, 
//    Spot
//};

int main()
{
    nu::SetWorkingDirectory("Assets"); 
    
    // INITIALIZATION
    Engine::Get().Initialize();    

    auto renderer = Engine::Get().GetRenderer();
    Scene scene;
    scene.Load("scenes/scene.json");

    auto pipeline = std::make_shared<Pipeline>();
    
    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));
    
    auto model = Resources().Get<Model>("models/spot.obj", renderer);
    auto texture = Resources().Get<Texture>("textures/spot_diffuse-1.png", renderer);

    auto vshader = Resources().Get<nu::Shader>("shaders/phong.vert", renderer, 3);
    auto fshader = Resources().Get<nu::Shader>("shaders/phong.frag", renderer, 3, 1);

    pipeline->Create(*vshader.get(), *fshader.get(),
        renderer.GetGPUDevice(),
        renderer.GetWindow());

    // UNIFORMS
    //struct VertexUniform
    //{
    //    glm::mat4 mvp;
    //    glm::mat4 model;
    //    glm::vec3 cameraPosition;
    //    float padding1;
    //};

    //VertexUniform vertexUniform;

    //struct LightUniform
    //{
    //    glm::vec3 position;
    //    LightType type;
    //    glm::vec3 direction;
    //    float innerCutoff;
    //    glm::vec3 color;
    //    float intensity;
    //    glm::vec3 ambient;
    //    float outerCutoff;
    //    float range;
    //    glm::vec3 padding;
    //};

    //LightUniform lightUniform{
    //    .position{ 10, 10, 10 },
    //    .type{ LightType::Point },
    //    .direction{ 0, -1, 0 },
    //    .innerCutoff{ 0.3f },
    //    .color{ 1, 1, 1 },
    //    .intensity{ 1 },
    //    .ambient{ 0.1f },
    //    .outerCutoff{ 1.6f },
    //    .range{ 15 }
    //};
    //  
    //struct MaterialUniform
    //{
    //    glm::vec4 color;
    //    glm::vec2 tiling;
    //    glm::vec2 offset;
    //    float shininess;
    //    glm::vec3 padding;
    //};


    //MaterialUniform materialUniform
    //{
    //    .color{ 1, 1, 1, 1 },
    //    .tiling{ 1.0f, 1.0f },
    //    .offset{ 0.0f, 0.0f },
    //    .shininess{ 32.0f }
    //};

    //vertexUniform.cameraPosition = { 0, 0, 5 };

    //Transform transform;



    // main loop
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

        // ENGINE
        Engine::Get().Update();

        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = +1;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.y = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.y = -1;
        
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.z = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.z = -1;

        //vertexUniform.cameraPosition += direction * 4.0f * dt;

        //if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1))) lightUniform.type = LightType::Directional;
        //if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2))) lightUniform.type = LightType::Point;
        //if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3)))
        //{
        //    lightUniform.type = LightType::Spot;
        //    lightUniform.position = glm::vec3{ 0, 6, 0 };
        //    lightUniform.innerCutoff = glm::cos(glm::radians(30.0f));
        //    lightUniform.outerCutoff = glm::cos(glm::radians(60.0f));
        //}

        // TRANSFORM
        //transform.rotation.y = time * 90;

        // VIEW MATRIX
        //auto view = glm::lookAtRH(vertexUniform.cameraPosition, vertexUniform.cameraPosition + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

        // PROJECTION MATRIX
        float aspectRatio = renderer.GetWidth() / (float)renderer.GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 20.0f);

        // MVP
        //vertexUniform.model = transform.GetMatrix();
        //vertexUniform.mvp = projection * view * vertexUniform.model;

        // RENDER
        renderer.BeginFrame();

        renderer.SetPipeline(*pipeline);
        renderer.SetFragmentTexture(0, *texture);

        //Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        //Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        //Engine::Get().GetRenderer().SetVertexUniform(2, &materialUniform, sizeof(materialUniform));

        //Engine::Get().GetRenderer().SetFragmentUniform(0, &vertexUniform, sizeof(vertexUniform));
        //Engine::Get().GetRenderer().SetFragmentUniform(1, &lightUniform, sizeof(lightUniform));
        //Engine::Get().GetRenderer().SetFragmentUniform(2, &materialUniform, sizeof(materialUniform));

        scene.Draw(renderer);
        //model->Draw(renderer);

        renderer.EndFrame();
    }

    Engine::Get().Shutdown();

    return 0;
}