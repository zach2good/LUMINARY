// ============================================================================================
// LUMINARY - a game engine and framework for the SEGA Mega Drive
// ============================================================================================
// Matt Phillips - Big Evil Corporation Ltd - 7th August 2019
// ============================================================================================
// Types.h - Data types for working with Luminary scenes, entities, components
// ============================================================================================

#pragma once

#include <ion/core/Types.h>

namespace luminary
{
	enum class ParamSize
	{
		Byte,
		Word,
		Long
	};

	struct Param
	{
		std::string name;
		ParamSize size;
		std::string value;
		std::vector<std::string> tags;
	};

	struct SpawnData
	{
		u32 positionX;
		u32 positionY;
		std::string name;
		std::vector<Param> params;
	};

	struct Component
	{
		std::string name;
		SpawnData spawnData;
		std::vector<Param> params;
	};

	struct Entity
	{
		std::string name;
		SpawnData spawnData;
		std::vector<Param> params;
		std::vector<Component> components;
	};
}