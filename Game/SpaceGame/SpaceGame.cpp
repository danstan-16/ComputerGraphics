#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Assets.h"

using namespace nu;
class Scene;

bool SpaceGame::Initialize()
{
	SetWorkingDirectory("spacegame");

	Game::Initialize();

	m_scene = std::make_unique<nu::Scene>();
	m_scene->SetGame(this);
	m_scene->Load("data/scene.json");

	m_titleFont = new Font();
	m_titleFont->Load("fonts/airstrike.ttf", 64);

	m_titleText = new Text(Resources().GetWithID<Font>("title_font", "fonts/airstrike.ttf", 64.0f));
	m_titleText->Create(Engine::Get().GetRenderer(), "XENON", Color{ 1.0f, 1.0f, 1.0f });

	m_gameFont = new Font();
	m_gameFont->Load("fonts/airstrike.ttf", 32);

	m_scoreText = new Text(Resources().GetWithID<Font>("score_font", "fonts/airstrike.ttf", 32.0f));
	m_livesText = new Text(Resources().GetWithID<Font>("lives_font", "fonts/airstrike.ttf", 32.0f));

	m_gameOverFont = new Font();
	m_gameOverFont->Load("fonts/airstrike.ttf", 64);

	m_gameOverText = new Text(Resources().GetWithID<Font>("gameOver_font", "fonts/airstrike.ttf", 64.0f));

	Engine::Get().GetAudio().AddSound("explosion", "../../Build/Assets/audio/explosion.mp3");
	Engine::Get().GetAudio().AddSound("pewpew", "../../Build/Assets/audio/pewpew.mp3");


	return true;
}

void SpaceGame::Update(float dt)
{
	switch (m_gameState)
	{
	case SpaceGame::Title:
		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_gameState = SpaceGame::StartGame;
		}
		break;

	case SpaceGame::StartGame:
		m_score = 0;
		m_lives = 3;
		m_spawnTime = 5.0f;
		m_stateTimer = 0.5f;
		m_gameState = SpaceGame::StartLevel;
		break;

	case SpaceGame::StartLevel:
		m_scene->RemoveAllActors();
		SpawnPlayer();
		m_spawnTime = 5.0f;
		m_gameState = SpaceGame::GamePlay;
		break;

	case SpaceGame::GamePlay:
		//Engine::Get().GetAudio().PlaySound("theme-music");
		m_spawnTimer -= dt;
		if (m_spawnTimer <= 0)
		{
			m_spawnTimer = m_spawnTime;
			SpawnEnemy();
			m_spawnCount++;
			if (m_spawnCount > 5)
			{
				m_spawnCount = 0;
				m_spawnTime -= 0.5f;
			}
		}
		break;
	case SpaceGame::GameOver:
		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_scene->RemoveAllActors();
			m_gameState = SpaceGame::Title;
		}
		break;

	default:
		break;
	}

	Game::Update(dt);
}


void SpaceGame::Draw(nu::Renderer& renderer)
{
	renderer.DrawTexture(*nu::Resources().Get<Texture>("textures/space_background.jpg", Engine::Get().GetRenderer()), Engine::Get().GetRenderer().GetWidth() * 0.5f, Engine::Get().GetRenderer().GetHeight() * 0.5f, 0.0f, 3.0f);

	switch (m_gameState)
	{
	case SpaceGame::Title:
		// draw title
		m_titleText->Draw(renderer, (float)renderer.GetWidth() - 750.0f, (float)renderer.GetHeight() - 800.0f);
		break;

	case SpaceGame::StartGame:
		break;
	case SpaceGame::StartLevel:
		break;
	case SpaceGame::GamePlay:
		// draw score / lives
		m_scoreText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
		m_scoreText->Draw(renderer, 30, 30);

		m_livesText->Create(renderer, "Lives: " + std::to_string(m_lives), { 1.0f, 1.0f, 1.0f });
		m_livesText->Draw(renderer, (float)renderer.GetWidth() - 200.0f, 30.0f);
		break;

	case SpaceGame::GameOver:
		// draw gameover
		m_gameOverText->Create(renderer, "GAME OVER", { 1.0f, 0.0f, 0.0f });
		m_gameOverText->Draw(renderer, (float)renderer.GetWidth() - 850.0f, (float)renderer.GetHeight() - 900.0f);
		break;
	default:
		break;
	}

	Game::Draw(renderer);
}

void SpaceGame::SpawnPlayer()
{
	auto actor = Factory::Instance().Create<Actor>("PlayerPrototype");
	m_scene->AddActor(std::move(actor));
}

void SpaceGame::SpawnEnemy()
{
	int model = RandomInt(3);

	if (model == 0) {
		auto actor = Factory::Instance().Create<Actor>("Enemy01");
		actor->SetPosition(Vector2(RandomFloat((float)Engine::Get().GetRenderer().GetWidth()), RandomFloat((float)Engine::Get().GetRenderer().GetHeight())));
		m_scene->AddActor(std::move(actor));
	}
	else if (model == 1)
	{
		auto actor = Factory::Instance().Create<Actor>("Enemy02");
		actor->SetPosition(Vector2(RandomFloat((float)Engine::Get().GetRenderer().GetWidth()), RandomFloat((float)Engine::Get().GetRenderer().GetHeight())));
		m_scene->AddActor(std::move(actor));
	}
	else
	{
		auto actor = Factory::Instance().Create<Actor>("Enemy03");
		actor->SetPosition(Vector2(RandomFloat((float)Engine::Get().GetRenderer().GetWidth()), RandomFloat((float)Engine::Get().GetRenderer().GetHeight())));
		m_scene->AddActor(std::move(actor));
	}
}

void SpaceGame::OnPlayerDead()
{
	m_lives--;
	if (m_lives == 0)
	{
		m_gameState = SpaceGame::GameOver;
	}
	else m_gameState = SpaceGame::StartLevel;
}