#pragma once

#include <math.h>
#include "S3DL/S3DLVector3.h"
#include "S3DL/S3DLMesh.h"
#include "S3DL/S3DLColors.h"

// �� ������ ����� TextureCoordinates � Colors � ���� ������
// ������ �������� �������� ���������� ����� �������
// ������������ ������������ ������ � ���������� ������� �������������
void fillTexture(
	const S3DLArray<S3DLVector2>& TextureCoordinates,
	const S3DLArray<S3DLVector3>& Colors,
	const int width, const int height,
	S3DLColorPicture& texture); 

// ���� �����, ������ ����������� ���������� RBF
void fillTextureRBF(
	const S3DLArray<S3DLVector2>& TextureCoordinates,
	const S3DLArray<S3DLVector3>& Colors,
	const int width, const int height,
	S3DLColorPicture& texture); 
