#pragma once
#include"../MakeCutSceneManager.h"

class MakeCutSceneElementBase
{
public:
	MakeCutSceneElementBase(void);
	virtual ~MakeCutSceneElementBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	virtual void Reset(void) = 0;

protected:

};

