#include "stdafx.h"
#include "SimpleObjects.h"

Object::AImage::AImage(Image::Component component) { Component = component; }

Object::ASimpleImage::ASimpleImage(Image::Component component) : AImage(component) {}
void Object::ASimpleImage::Start() {}
void Object::ASimpleImage::Update()
{
	Component.Draw();
}
void Object::ASimpleImage::End() {}
float Object::ASimpleImage::GetAlphaLevel() const		   { return Component.AlphaLevel; }
void Object::ASimpleImage::SetAlphaLevel(float alphaLevel) { Component.AlphaLevel = alphaLevel; }
void Object::ASimpleImage::SetLength(vector<2> length)	   { Component.Length = length; }

Object::ARotatingImage::ARotatingImage(Image::Component component) : AImage(component) {}
void Object::ARotatingImage::Start() {}
void Object::ARotatingImage::Update()
{
	Component.Angle[2] -= 180.0f * DELTA_TIME;
	AdjustAngle(Component.Angle[2]);

	Component.Draw();
}
void Object::ARotatingImage::End() {}

Object::AAnim::AAnim(Animation::Component component) { Component = component; }

Object::ASimpleAnim::ASimpleAnim(Animation::Component component) : AAnim(component) {}
void Object::ASimpleAnim::Start() {}
void Object::ASimpleAnim::Update() { Component.Draw(); }
void Object::ASimpleAnim::End() {}