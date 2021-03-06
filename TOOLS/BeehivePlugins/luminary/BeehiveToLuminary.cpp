// ============================================================================================
// LUMINARY - a game engine and framework for the SEGA Mega Drive
// ============================================================================================
// Matt Phillips - Big Evil Corporation Ltd - 9th February 2020
// ============================================================================================
// BeehiveToLuminary.h - Utilities for converting between Luminary and Beehive types
// ============================================================================================

#include "BeehiveToLuminary.h"
#include "Tags.h"

#include <ion/core/utils/STL.h>

namespace luminary
{
	namespace beehive
	{
		const SpriteSheet* FindSpriteSheet(const Actor& actor, const GameObjectType& gameObjectType, const GameObject* gameObject, const GameObjectVariable* variable)
		{
			//Sprite sheet from variable
			const SpriteSheet* spriteSheet = variable ? actor.GetSpriteSheet(actor.FindSpriteSheetId(variable->m_value)) : nullptr;

			//Sprite sheet from game object
			if (!spriteSheet && gameObject)
				spriteSheet = actor.GetSpriteSheet(gameObject->GetSpriteSheetId());

			//Sprite sheet from game object type
			if (!spriteSheet)
				spriteSheet = actor.GetSpriteSheet(gameObjectType.GetSpriteSheetId());

			return spriteSheet;
		}

		const SpriteAnimation* FindSpriteAnim(const Actor& actor, const GameObjectType& gameObjectType, const GameObject* gameObject, const GameObjectArchetype* archetype, const GameObjectVariable& variable, std::string& sheetName)
		{
			const SpriteSheet* spriteSheet = nullptr;
			const SpriteAnimation* spriteAnim = nullptr;

			//Sprite sheet from variable
			const GameObjectVariable* spriteSheetVar = gameObject ? gameObject->FindVariableByTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteSheet), variable.m_componentIdx) : nullptr;

			if (!spriteSheetVar)
				spriteSheetVar = archetype ? archetype->FindVariableByTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteSheet), variable.m_componentIdx) : nullptr;

			if (!spriteSheetVar)
				spriteSheetVar = gameObjectType.FindVariableByTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteSheet), variable.m_componentIdx);

			if (spriteSheetVar)
				spriteSheet = FindSpriteSheet(actor, gameObjectType, gameObject, spriteSheetVar);

			//Sprite sheet from game object
			if (!spriteSheet && gameObject)
				spriteSheet = actor.GetSpriteSheet(gameObject->GetSpriteSheetId());

			//Sprite sheet from game object type
			if (!spriteSheet)
				spriteSheet = actor.GetSpriteSheet(gameObjectType.GetSpriteSheetId());

			if (spriteSheet)
			{
				//Get name
				sheetName = spriteSheet->GetName();

				//Sprite anim from variable
				spriteAnim = spriteSheet->FindAnimation(variable.m_value);

				//Sprite anim from game object
				if (!spriteAnim && gameObject)
					spriteAnim = spriteSheet->GetAnimation(gameObject->GetSpriteAnim());

				//Sprite anim from game object type
				if (!spriteAnim)
					spriteAnim = spriteSheet->GetAnimation(gameObjectType.GetSpriteAnim());
			}

			return spriteAnim;
		}

		void ExportParam(luminary::Param& param, const GameObjectVariable& variable, const GameObjectType& gameObjectType, const GameObjectArchetype* archetype, const GameObject* gameObject, const Actor* actor, const luminary::ScriptAddressMap& scriptAddresses)
		{
			param.name = variable.m_name;
			param.value = "0x0";

			std::string scriptRoutine;

			//Search for supported tags
			if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::EntityDesc)))
			{
				std::stringstream stream;
				stream << variable.m_value << "_TypeDesc";
				param.value = stream.str();
			}
			else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::EntityArchetype)))
			{
				//Find entity type first
				std::string entityTypeName;
				const GameObjectVariable *typeVariable = nullptr;

				if (archetype)
				{
					//Find variable in archetype
					typeVariable = archetype->FindVariableByTag(luminary::tags::GetTagName(luminary::tags::TagType::EntityDesc), variable.m_componentIdx);
				}
				if (gameObject && !typeVariable)
				{
					//Find variable on instance
					typeVariable = gameObject->FindVariableByTag(luminary::tags::GetTagName(luminary::tags::TagType::EntityDesc), variable.m_componentIdx);
				}

				if (typeVariable)
					entityTypeName = typeVariable->m_value;
				else
					entityTypeName = gameObjectType.GetName();

				std::stringstream stream;
				stream << "Archetype_" << entityTypeName << "_" << variable.m_value;
				param.value = stream.str();
			}
			else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::PositionX)))
			{
				if (gameObject)
					param.value = std::to_string(gameObject->GetPosition().x + GameObject::spriteSheetBorderX);
			}
			else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::PositionY)))
			{
				if (gameObject)
					param.value = std::to_string(gameObject->GetPosition().y + GameObject::spriteSheetBorderY);
			}
			else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteSheet)))
			{
				if (actor)
				{
					if (const SpriteSheet* spriteSheet = FindSpriteSheet(*actor, gameObjectType, gameObject, &variable))
					{
						std::stringstream stream;
						stream << "actor_" << actor->GetName() << "_spritesheet_" << spriteSheet->GetName();
						param.value = stream.str();
					}
				}
			}
			else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteAnimation)))
			{
				if (actor)
				{
					if (const SpriteSheet* spriteSheet = FindSpriteSheet(*actor, gameObjectType, gameObject, &variable))
					{
						std::string sheetName;
						if (const SpriteAnimation* spriteAnim = FindSpriteAnim(*actor, gameObjectType, gameObject, archetype, variable, sheetName))
						{
							std::stringstream stream;
							stream << "actor_" << actor->GetName() << "_sheet_" << spriteSheet->GetName() << "_anim_" << spriteAnim->GetName();

							param.value = stream.str();
						}
					}
				}
			}
			else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::ScriptData)))
			{
				param.value = std::string("scriptdata_") + gameObjectType.GetName();
			}
			else if (variable.FindTagValue("SCRIPTFUNC", scriptRoutine))
			{
				ScriptAddressMap::const_iterator it = scriptAddresses.find(gameObjectType.GetName());
				if (it != scriptAddresses.end())
				{
					for (auto address : it->second)
					{
						if (address.routineName == scriptRoutine)
						{
							std::stringstream stream;
							stream << "0x" << SSTREAM_HEX4(address.routineAddress);
							param.value = stream.str();
							break;
						}
					}
				}
			}
			else
			{
				param.value = variable.m_value;

				//If game object has overridden the variable, take that value instead
				if (gameObject)
				{
					if (const GameObjectVariable* overriddenVar = gameObject->FindVariable(variable.m_name))
					{
						param.value = overriddenVar->m_value;
					}
				}
			}

			switch (variable.m_size)
			{
			case eSizeByte:
				param.size = luminary::ParamSize::Byte;
				break;
			case eSizeWord:
				param.size = luminary::ParamSize::Word;
				break;
			case eSizeLong:
				param.size = luminary::ParamSize::Long;
				break;
			}
		}

		void ExportArchetype(const Project& project, const GameObjectArchetype& srcArchetype, const luminary::ScriptAddressMap& scriptAddresses, luminary::Archetype& archetype)
		{
			if (const GameObjectType* gameObjectType = project.GetGameObjectType(srcArchetype.typeId))
			{
				archetype.name = srcArchetype.name;
				archetype.entityTypeName = gameObjectType->GetName();
				const Actor* actor = project.GetActor(gameObjectType->GetSpriteActorId());

				//Create archetype params
				int paramIdx = 0;
				int componentIdx = -1;

				const std::vector<GameObjectVariable>& variables = gameObjectType->GetVariables();

				for (int j = 0; j < variables.size(); j++, paramIdx++)
				{
					//Find overridden variable on archetype
					const GameObjectVariable* variable = srcArchetype.FindVariable(variables[j].m_name, variables[j].m_componentIdx);
					if (!variable)
					{
						//Use variable from game object type
						variable = &variables[j];
					}

					luminary::Param* param = nullptr;

					if (variable->m_componentIdx == -1)
					{
						//Entity param
						archetype.params.resize(paramIdx + 1);
						param = &archetype.params[paramIdx];
					}
					else
					{
						//Component param
						if (componentIdx != variable->m_componentIdx)
						{
							componentIdx = variable->m_componentIdx;
							archetype.components.resize(componentIdx + 1);
							archetype.components[componentIdx].name = variable->m_componentName;
							paramIdx = 0;
						}

						archetype.components[componentIdx].spawnData.params.resize(paramIdx + 1);
						param = &archetype.components[componentIdx].spawnData.params[paramIdx];
					}

					ExportParam(*param, *variable, *gameObjectType, &srcArchetype, nullptr, actor, scriptAddresses);
				}
			}
		}

		void ConvertScriptEntity(const GameObjectType& gameObjectType, luminary::Entity& entity)
		{
			entity.name = gameObjectType.GetName();

			const std::vector<GameObjectVariable>& variables = gameObjectType.GetScriptVariables();

			int paramIdx = 0;
			int componentIdx = -1;

			for (int j = 0; j < variables.size(); j++, paramIdx++)
			{
				const GameObjectVariable& variable = variables[j];
				luminary::Param* param = nullptr;

				if (variable.m_componentIdx == -1)
				{
					//Entity param
					entity.params.resize(paramIdx + 1);
					param = &entity.params[paramIdx];
				}
				else
				{
					//Component param
					if (componentIdx != variable.m_componentIdx)
					{
						componentIdx = variable.m_componentIdx;
						entity.components.resize(componentIdx + 1);
						entity.components[componentIdx].name = variable.m_componentName;
						paramIdx = 0;
					}

					entity.components[componentIdx].params.resize(paramIdx + 1);
					param = &entity.components[componentIdx].params[paramIdx];
				}

				param->name = variable.m_name;
				param->value = "0x0";

				switch (variable.m_size)
				{
				case eSizeByte:
					param->size = luminary::ParamSize::Byte;
					break;
				case eSizeWord:
					param->size = luminary::ParamSize::Word;
					break;
				case eSizeLong:
					param->size = luminary::ParamSize::Long;
					break;
				}
			}
		}

		void ExportEntity(const Project& project, const GameObjectType& gameObjectType, const GameObject& gameObject, const luminary::ScriptAddressMap& scriptAddresses, luminary::Entity& entity)
		{
			//Type name
			entity.name = gameObjectType.GetName();

			//Entity name
			if (gameObject.GetName().size() > 0)
				entity.spawnData.name = gameObject.GetName();
			else
				entity.spawnData.name = std::string("ent") + std::to_string(gameObject.GetId());

			//Spawn position
			entity.spawnData.positionX = gameObject.GetPosition().x + GameObject::spriteSheetBorderX;
			entity.spawnData.positionY = gameObject.GetPosition().y + GameObject::spriteSheetBorderY;
			entity.spawnData.width = (gameObject.GetDimensions().x > 0) ? gameObject.GetDimensions().x : gameObjectType.GetDimensions().x;
			entity.spawnData.height = (gameObject.GetDimensions().y > 0) ? gameObject.GetDimensions().y : gameObjectType.GetDimensions().y;

			//Sprite actor from game object
			const Actor* actor = project.GetActor(gameObject.GetSpriteActorId());

			//Sprite actor from game object type
			if (!actor)
				actor = project.GetActor(gameObjectType.GetSpriteActorId());

			//Create entity and component spawn params
			int paramIdx = 0;
			int componentIdx = -1;

			const std::vector<GameObjectVariable>& variables = gameObjectType.GetVariables();

			for (int j = 0; j < variables.size(); j++, paramIdx++)
			{
				//Find overridden variable on game object
				const GameObjectVariable* variable = gameObject.FindVariable(variables[j].m_name, variables[j].m_componentIdx);
				if (!variable)
				{
					//Use variable from game object type
					variable = &variables[j];
				}

				luminary::Param* param = nullptr;

				if (variable->m_componentIdx == -1)
				{
					//Entity param
					entity.spawnData.params.resize(paramIdx + 1);
					param = &entity.spawnData.params[paramIdx];
				}
				else
				{
					//Component param
					if (componentIdx != variable->m_componentIdx)
					{
						componentIdx = variable->m_componentIdx;
						entity.components.resize(componentIdx + 1);
						entity.components[componentIdx].name = variable->m_componentName;
						paramIdx = 0;
					}

					entity.components[componentIdx].spawnData.params.resize(paramIdx + 1);
					param = &entity.components[componentIdx].spawnData.params[paramIdx];
				}

				ExportParam(*param, *variable, gameObjectType, nullptr, &gameObject, actor, scriptAddresses);
			}
		}
	}
}