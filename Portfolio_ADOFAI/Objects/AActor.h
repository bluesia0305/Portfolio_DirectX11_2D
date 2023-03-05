#pragma once

class AActor abstract
{
public:
	AActor();
	virtual ~AActor();

	virtual void Start() abstract;
	virtual void Update() abstract;
	virtual void End() abstract;
};