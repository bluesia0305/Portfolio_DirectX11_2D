#include "stdafx.h"
#include "AActor.h"

AActor::AActor()  { INSTANCE(ObjectManager)->AddActor(this); }
AActor::~AActor() { INSTANCE(ObjectManager)->DeleteActor(this); }