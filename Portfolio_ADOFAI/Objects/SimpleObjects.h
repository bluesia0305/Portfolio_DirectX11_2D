#pragma once
#include "AActor.h"

namespace Object
{
	class AImage : public AActor
	{
	protected:
		Image::Component	 Component;

	public:
		AImage(Image::Component component);
		virtual void Start() abstract;
		virtual void Update() abstract;
		virtual void End() abstract;
	};


	class ASimpleImage : public AImage
	{
	public:
		ASimpleImage(Image::Component component);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;

		virtual float GetAlphaLevel() const;
		virtual void SetAlphaLevel(float alphaLevel);
		virtual void SetLength(vector<2> length);
	};


	class ARotatingImage : public AImage
	{
	public:
		ARotatingImage(Image::Component component);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
	};


	class AAnim : public AActor
	{
	protected:
		Animation::Component Component;

	public:
		AAnim(Animation::Component component);
		virtual void Start() abstract;
		virtual void Update() abstract;
		virtual void End() abstract;
	};


	class ASimpleAnim : public AAnim
	{
	public:
		ASimpleAnim(Animation::Component component);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
	};
}