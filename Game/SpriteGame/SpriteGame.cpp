#include "SpriteGame.h"
#include "Engine.h"

using namespace nu;
class Scene;

bool SpriteGame::Initialize()
{
	SetWorkingDirectory("spritegame");

	Game::Initialize();

	m_scene = std::make_unique<nu::Scene>();
	m_scene->SetGame(this);
	m_scene->Load("data/scene.json");

	m_titleFont = new Font();
	m_titleFont->Load("fonts/airstrike.ttf", 64);

	m_titleText = new Text(Resources().GetWithID<Font>("title_font", "fonts/airstrike.ttf", 64.0f));
	m_titleText->Create(Engine::Get().GetRenderer(), "DRUIA", Color{ 1.0f, 1.0f, 1.0f });

	m_gameFont = new Font();
	m_gameFont->Load("fonts/airstrike.ttf", 32);

	m_scoreText = new Text(Resources().GetWithID<Font>("score_font", "fonts/airstrike.ttf", 32.0f));
	m_livesText = new Text(Resources().GetWithID<Font>("lives_font", "fonts/airstrike.ttf", 32.0f));

	m_gameOverFont = new Font();
	m_gameOverFont->Load("fonts/airstrike.ttf", 64);

	m_gameOverText = new Text(Resources().GetWithID<Font>("gameOver_font", "fonts/airstrike.ttf", 64.0f));

 	Engine::Get().GetAudio().AddSound("shoot", "audio/bow_shoot2.mp3");
	Engine::Get().GetAudio().AddSound("gameover", "audio/game_over.wav");
	Engine::Get().GetAudio().AddSound("dying", "audio/dying.mp3");

	return true;
}

void SpriteGame::Update(float dt)
{
	switch (m_gameState)
	{
	case SpriteGame::Title:
		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_gameState = SpriteGame::StartGame;
		}
		break;

	case SpriteGame::StartGame:
		m_score = 0;
		m_lives = 3;
		m_spawnTime = 5.0f;
		m_stateTimer = 0.5f;
		m_gameState = SpriteGame::StartLevel;
		break;

	case SpriteGame::StartLevel:
		m_scene->RemoveAllActors();
		m_scene->Load("data/level.json");

		SpawnPlayer();
		m_spawnTime = 5.0f;
		m_gameState = SpriteGame::GamePlay;
		break;

	case SpriteGame::GamePlay:
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
	case SpriteGame::GameOver:
		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_scene->RemoveAllActors();
			m_gameState = SpriteGame::Title;
		}
		break;

	default:
		break;
	}

	Game::Update(dt);
}


void SpriteGame::Draw(nu::Renderer& renderer)
{
	renderer.EnableCamera(false);
	renderer.DrawTexture(*nu::Resources().Get<Texture>("textures/nature_background.jpg", Engine::Get().GetRenderer()), Engine::Get().GetRenderer().GetWidth() * 0.5f, Engine::Get().GetRenderer().GetHeight() * 0.5f, 0.0f, 0.3f);

	switch (m_gameState)
	{
	case SpriteGame::Title:
		// draw title
		m_titleText->Draw(renderer, (float)renderer.GetWidth() - 750.0f, (float)renderer.GetHeight() - 800.0f);
		break;

	case SpriteGame::StartGame:
		break;
	case SpriteGame::StartLevel:
		break;
	case SpriteGame::GamePlay:
		// draw score / lives
		m_scoreText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
		m_scoreText->Draw(renderer, 30, 30);

		m_livesText->Create(renderer, "Lives: " + std::to_string(m_lives), { 1.0f, 1.0f, 1.0f });
		m_livesText->Draw(renderer, (float)renderer.GetWidth() - 200.0f, 30.0f);
		break;

	case SpriteGame::GameOver:
		// play game over sound
		Engine::Get().GetAudio().PlaySound("gameover");

		// draw gameover
		m_gameOverText->Create(renderer, "GAME OVER", { 1.0f, 0.0f, 0.0f });
		m_gameOverText->Draw(renderer, (float)renderer.GetWidth() - 850.0f, (float)renderer.GetHeight() - 900.0f);
		break;
	default:
		break;
	}

	renderer.EnableCamera(true);
	Game::Draw(renderer);
}

void SpriteGame::SpawnPlayer()
{
	auto actor = Factory::Instance().Create<Actor>("PlayerPrototype");
	m_scene->AddActor(std::move(actor));
}

void SpriteGame::SpawnEnemy()
{
		auto actor = Factory::Instance().Create<Actor>("EnemyPrototype");
		actor->SetPosition(Vector2(RandomFloat((float)Engine::Get().GetRenderer().GetWidth()), RandomFloat((float)Engine::Get().GetRenderer().GetHeight())));
		m_scene->AddActor(std::move(actor));
}

void SpriteGame::OnPlayerDead()
{
	Engine::Get().GetAudio().PlaySound("dying");
	m_lives--;
	if (m_lives == 0)
	{
		m_gameState = SpriteGame::GameOver;
	}
	else m_gameState = SpriteGame::StartLevel;
}