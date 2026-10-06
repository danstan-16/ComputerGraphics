#include "Assets.h"
#include "Renderer/Texture.h"
#include "Engine.h"

using namespace nu;

namespace Assets
{
	Mesh player{ {
			Vector2{2, 0},
			Vector2{0, 1},
			Vector2{-3, 3},
			Vector2{-1, 0 },
			Vector2{-3, -3},
			Vector2{0, -1},
			Vector2{2, 0}
			},
			Color {255.0f, 255.0f, 255.0f}
	};

	std::shared_ptr<Model> playerModel = std::make_shared<Model>(std::vector<Mesh>{player});

	Mesh enemyBody1{ {
			Vector2{2, 0},
			Vector2{0, 1},
			Vector2{-3, 3},
			Vector2{-1, 0 },
			Vector2{-3, -3},
			Vector2{0, -1},
			Vector2{2, 0}
			},
			Color {255.0f, 255.0f, 255.0f}
	};

	std::shared_ptr<Model> enemyModel1 = std::make_shared<Model>(std::vector<Mesh>{enemyBody1});

	Mesh enemyBody2{ {
			Vector2{2, 0},
			Vector2{0, 1},
			Vector2{-3, 1},
			Vector2{-1, 0 },
			Vector2{-3, -1},
			Vector2{0, -1},
			Vector2{2, 0}
			},
			Color {255.0f, 255.0f, 255.0f}
	};

	std::shared_ptr<Model> enemyModel2 = std::make_shared<Model>(std::vector<Mesh>{enemyBody2});

	Mesh enemyBody3{ {
			Vector2{2, 0},
			Vector2{0, 1},
			Vector2{-2, 1},
			Vector2{-1, 0 },
			Vector2{-2, -1},
			Vector2{0, -1},
			Vector2{2, 0}
			},
			Color {255.0f, 255.0f, 255.0f}
	};

	std::shared_ptr<Model> enemyModel3 = std::make_shared<Model>(std::vector<Mesh>{enemyBody3});

	Mesh bulletMesh{ {
			Vector2{ -1, -1 },
			Vector2{ 1, 0 },
			Vector2{ -1, 1 },
			Vector2{ -1, -1 },
			},
			Color{ 0.0f, 240.0f, 0.0f }
	};
	std::shared_ptr<Model> bulletModel = std::make_shared<Model>(std::vector<Mesh>{bulletMesh});
}