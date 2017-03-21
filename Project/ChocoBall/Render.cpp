#include "stdafx.h"
#include "Render.h"

int CRender::test = 0;

CRender::CRender()
{
	test++;
}


CRender::~CRender()
{
	test--;
}
