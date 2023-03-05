#pragma once
#define INSTANCE(T) T::GetInstance()

template<typename T> class Singleton
{
public:
	static T* GetInstance()
	{
		static T* instance = new T;
		return instance;
	}

	static void Release()
	{ delete GetInstance(); }
};