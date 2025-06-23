#pragma once

#include <Game/Component/IComponent.hpp>
#include <Game/Utils/Point.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace sw
{
	class Map;
	struct IUpdateContext;

	class MapObject : public std::enable_shared_from_this<MapObject>
	{
		friend class MapObjectBuilder;

	public:
		MapObject(uint32_t id, std::string type, uint32_t _x, uint32_t _y);

		virtual ~MapObject();

		uint32_t GetId() const;

		const std::string& GetType() const;

		uint32_t GetX() const;

		uint32_t GetY() const;

		const Point& GetPos() const;

		void SetPos(const Point& pos);

		virtual bool Update(IUpdateContext& context) = 0;

		template <IsComponent T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			auto comp = std::make_unique<T>(std::forward<Args>(args)...);
			T* rawPtr = comp.get();
			m_Components[typeid(T)] = std::move(comp);
			return rawPtr;
		}

		template <IsComponent T>
		T* GetComponent() const
		{
			auto it = m_Components.find(typeid(T));
			if (it != m_Components.end())
			{
				return static_cast<T*>(it->second.get());
			}
			return nullptr;
		}

	private:
		uint32_t m_unitId;
		std::string m_Type;
		Point m_Pos;

		std::unordered_map<std::type_index, std::unique_ptr<IComponent>> m_Components;
	};

}
