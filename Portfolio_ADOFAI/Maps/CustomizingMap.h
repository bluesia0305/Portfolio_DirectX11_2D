#pragma once
#include "SynchronizedMap.h"

namespace Map
{
	class CustomizingMap final : public SynchronizedMap
	{
	public:
		CustomizingMap();
		virtual ~CustomizingMap();
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
	};
}