#pragma once
#include "SynchronizedMap.h"
#include <random>

namespace Map
{
	class IntroMap final : public SynchronizedMap
	{
	private:
		bool	isShowAll;

	public:
		IntroMap();
		virtual ~IntroMap();
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;

		void SetShowAll(bool isShowAll);
	};
}