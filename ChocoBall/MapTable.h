#pragma once
#include "stdafx.h"

struct MapObject
{
	const char* FileName;
	D3DXVECTOR3 Position;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Scale;
};

//static MapObject MapObjects_Boss[] = {
//
//};