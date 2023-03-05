#pragma once
#include "IPoolableObject.h"

template<typename T>
class ObjectPool final
{
private:
	std::vector<T*> Pool;

public:
	virtual ~ObjectPool()
	{
		for (auto elem : Pool)
			delete elem;

		Pool.clear();
	}

    T* GetRecycledObject()
    {
        T* pObject = nullptr;
        if (Pool.empty())
        {
			Pool.push_back(pObject = new T);
            return pObject;
        }

        for (auto elem : Pool)
        {
            if (elem->IsRecyclable())
            {
				pObject = elem;
				pObject->OffRecycle();
                return pObject;
            }
        }

        if (pObject == nullptr) Pool.push_back(pObject = new T);
        return pObject;
    }

    void Update()
    {
        for (auto elem : Pool)
        { if (!elem->IsRecyclable()) elem->Update(); }
    }

	void Shutdown()
	{
		for (auto elem : Pool)
			elem->ForceRecycle();
	}

	void Reset()
	{
		for (auto elem : Pool)
			delete elem;

		Pool.clear();
	}
};