#include "stdafx.h"
#include "ObjectManager.h"

void ObjectManager::DeleteActor(AActor* actor)
{
	auto found = std::find(Actors.begin(), Actors.end(), actor);
	if (found != Actors.end())
		Actors.erase(found);
}