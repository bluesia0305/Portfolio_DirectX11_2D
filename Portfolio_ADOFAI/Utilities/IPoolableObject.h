#pragma once

class IPoolableObject abstract
{
public:
	virtual ~IPoolableObject() = default;
	virtual bool IsRecyclable() abstract;
	virtual void ForceRecycle() abstract;
	virtual void OffRecycle() abstract;
};