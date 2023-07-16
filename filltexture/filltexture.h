#pragma once

#include <math.h>
#include "S3DL/S3DLVector3.h"
#include "S3DL/S3DLMesh.h"
#include "S3DL/S3DLColors.h"

// по набору точек TextureCoordinates и Colors в этих точках
// строит текстуру заполняя промежутки между точками
// используется триангуляция Делоне и билинейная заливка треугольников
void fillTexture(
	const S3DLArray<S3DLVector2>& TextureCoordinates,
	const S3DLArray<S3DLVector3>& Colors,
	const int width, const int height,
	S3DLColorPicture& texture); 

// тоже самое, только применяется восполение RBF
void fillTextureRBF(
	const S3DLArray<S3DLVector2>& TextureCoordinates,
	const S3DLArray<S3DLVector3>& Colors,
	const int width, const int height,
	S3DLColorPicture& texture); 
