#pragma once
#include <vector>
#include <Core/Headers/Components.h>
#include "Core/Headers/Entity.h"
#include "Math/Vec3.h"
#include <Objects/Headers/Shader.h>
#include <memory>
#include <concepts>
#include <type_traits>

// TODO: Revisit later if issues arise
template<typename T>
concept isEntity = std::derived_from<T, Entity>;


class Scene
{
public:
	Scene() = default;;

public:

	size_t m_totalEntities{};
	std::vector<std::unique_ptr<Entity>> m_entities{};
	std::vector<LightEntity*> m_lights{};
	std::vector<MeshEntity*> m_meshes{};

	template<typename T, typename... TArgs>
		requires std::derived_from<T, Entity>
	T* createEntity(const std::string& name, TArgs&&... args)
	{
		auto entity = std::make_unique<T>(std::forward<TArgs>(args)...);
		entity->tag = name;
		m_entities.push_back(std::move(entity));

		auto* basePtr = m_entities.back().get();
		T* typedPtr = static_cast<T*>(basePtr);
		if constexpr (std::is_base_of_v<LightEntity, T>)
		{
			m_lights.push_back(static_cast<LightEntity*>(typedPtr));
			// if m_lightCountByType is static on T:
			typedPtr->setLightID(T::m_lightCountByType);
			++T::m_lightCountByType;
		}

		if constexpr (std::is_base_of_v<MeshEntity, T>)
		{
			m_meshes.push_back(static_cast<MeshEntity*>(typedPtr));
		}

		return typedPtr;
	}

		
	

public:
	
	void imguiUse(const std::unique_ptr<Entity>& entity);
	void applyLightCountsToShader(const std::shared_ptr<Shader>& shader);

	void render(const std::shared_ptr<Shader>& shader);

	const std::vector<std::unique_ptr<Entity>>& getEntities() const
	{
		return m_entities;
	}

	size_t getEntityCount() const
	{
		return m_entities.size();
	}

	void illuminate(const std::shared_ptr<Shader>& shader);

};

 