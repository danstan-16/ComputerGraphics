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

using namespace nu;

struct Color4
{
    float r, g, b, a;
};

struct Vertex
{
    Vector3 position;
    Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ Vector3{ -0.5f, -0.5f, 0.0f}, Color4{1, 0, 0, 1}}, // Bottom-Left
    Vertex{ Vector3{0.5f, -0.5f, 0.0f}, Color4{0, 1, 0, 1}}, // Bottom-Right
    Vertex{ Vector3{0.5f,  0.5f, 0.0f},  Color4{0, 0, 1, 1}}, // Top-Right
    Vertex{ Vector3{-0.5f,  0.5f, 0.0f},  Color4{0, 1, 1, 1}}, // Top-Left
};

std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

int main()
{
    nu::SetWorkingDirectory("Assets"); 
    
    // INITIALIZATION
    Engine::Get().Initialize();    

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer());

    auto ib = std::make_shared<IndexBuffer>();
    ib->Create(indices, Engine::Get().GetRenderer());
        
    Color4 tint{ 0, 0, 0, 1 };
    
    struct vertexUniform
    {
        glm::mat4 mvp;
    };

    vertexUniform uniform;
    glm::vec3 cameraPosition{ 0, 0, 5 };

    Transform transform;

    //auto model = Resources().Get<Model>("models/teapot-1.obj", Engine::Get().GetRenderer());

    //auto vshader = Resources().Get<nu::Shader>("shaders/lit.vert", Engine::Get().GetRenderer(), 1);
    //auto fshader = Resources().Get<nu::Shader>("shaders/lit.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));


    auto model = Resources().Get<Model>("models/spot.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/spot_diffuse-1.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/unlit.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<nu::Shader>("shaders/unlit.frag", Engine::Get().GetRenderer(), 1, 1);

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    Color4 color{ 1, 1, 1, 1 };

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

        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        //color.r = sin(Engine::Get().GetTime().GetTime() * 3.0f) * 0.5f + 0.5f;

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = +1;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.y = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.y = -1;
        
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.z = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.z = -1;

        cameraPosition += direction * 4.0f * dt;

        // TRANSFORM
        transform.rotation.y = time * 90;

        // VIEW MATRIX
        auto view = glm::lookAtRH(cameraPosition, cameraPosition + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

        // PROJECTION MATRIX
        float aspectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 20.0f);

        // MVP
        uniform.mvp = projection * view * transform.GetMatrix();

        // RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));

        Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));
        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    Engine::Get().Shutdown();

    return 0;
}