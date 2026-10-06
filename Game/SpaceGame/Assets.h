#pragma once
#include "Renderer/Model.h"
#include "Renderer/Texture.h"
#include <memory>

namespace Assets
{
	extern std::shared_ptr<nu::Model> playerModel;
	extern std::shared_ptr<nu::Model> enemyModel1;
	extern std::shared_ptr<nu::Model> enemyModel2;
	extern std::shared_ptr<nu::Model> enemyModel3;
	extern std::shared_ptr<nu::Model> bulletModel;
}