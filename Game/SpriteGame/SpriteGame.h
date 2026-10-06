#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"

class SpriteGame : public nu::Game
{
public:
	enum GameState
	{
		Title,
		StartGame,
		StartLevel,
		GamePlay,
		GameOver
	};

public:
	SpriteGame() = default;
	SpriteGame(nu::Scene* scene) : nu::Game( scene )
	{ }

	bool Initialize() override;
	void Update(float dt) override;
	void Draw(class nu::Renderer& renderer) override;

	void OnPlayerDead();

	void AddPoints(int points) { m_score += points; }


private:
	void SpawnPlayer();
	void SpawnEnemy();

	int m_score{ 0 };
	int m_lives = 0 ;

	float m_stateTimer = 0.0f;

	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;
	int m_spawnCount = 0;

	GameState m_gameState = Title;

	nu::Font* m_titleFont{ nullptr };
	nu::Text* m_titleText{ nullptr };

	nu::Font* m_gameOverFont{ nullptr };
	nu::Text* m_gameOverText{ nullptr };

	nu::Font* m_gameFont{ nullptr };

	nu::Text* m_scoreText{ nullptr };
	nu::Text* m_livesText{ nullptr };
};