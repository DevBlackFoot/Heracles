/********************************
*								*
*	Node Info.h					*
*								*
*	Created : 2022/07/06		*
*	Updated : 2022/07/06		*
*********************************/

#pragma once

#include "Token/TokenList.h"

/// <summary>
/// ��ü�� Transform ����, ��ӵ� Transform ������ ���Ե�
/// 
/// 2022. 04. 07
/// </summary>
struct NodeInfo
{
	float	position[4];

	float	rotation[4];
	float	rotationAngle;

	float	scale[4];
	float	scaleAxis[4];
	float	scaleAxisAngle;

	float	transform[4][4];

	float	inheritPosition[4];
	float	inheritRotation[4];
	float	inheritScale[4];

	void SetData(std::ifstream& data);
};