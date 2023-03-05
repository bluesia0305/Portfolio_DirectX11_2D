#pragma once
#include "Utilities/Singleton.h"
#include "Objects/AActor.h"
#include <algorithm>

class ObjectManager final : public Singleton<ObjectManager>
{
private:
	std::vector<AActor*> Actors;

public:
	__forceinline void AddActor(AActor* actor) { Actors.push_back(actor); }
	void DeleteActor(AActor* actor);
	template<class T> T* GetActor() const;
	template<class T> void GetActors(std::vector<T*>&) const;
};

template<class T>
inline T* ObjectManager::GetActor() const
{
	for (AActor* elem : Actors)
	{
		T* elem_downcasted = dynamic_cast<T*>(elem);
		if (elem_downcasted)
			return elem_downcasted;
	}
	return nullptr;
}

template<class T>
inline void ObjectManager::GetActors(std::vector<T*>& actor_storage) const
{
	for (AActor* elem : Actors)
	{
		T* elem_downCasted = dynamic_cast<T*>(elem);
		if (elem_downCasted)
			actor_storage.push_back(elem_downCasted);
	}
}