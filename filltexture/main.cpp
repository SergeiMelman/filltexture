#include "stdafx.h"
#include "filltexture.h"

int main()
{
	// ��� ��������� ������
	int const coordsSize = 400; // �-�� �����
	int const width = 2048; // ������ ������� ��������
	int const height = 2048;

	S3DLArray<S3DLVector2> TextureCoordinates; // ������
	TextureCoordinates.resize(coordsSize);
	
	S3DLArray<S3DLVector3> Colors;
	Colors.resize(coordsSize);

	for(int i = 0; i < coordsSize; ++i)
	{
		TextureCoordinates[i].x = (float)(rand() - 10) / RAND_MAX; // ������ ������������ ����� �� ����
		TextureCoordinates[i].y = (float)(rand() - 10) / RAND_MAX;
		Colors[i].r = (float)rand() / RAND_MAX;
		Colors[i].g = (float)rand() / RAND_MAX;
		Colors[i].b = (float)rand() / RAND_MAX;
	}

	S3DLColorPicture texture;
	fillTexture(TextureCoordinates, Colors, width, height, texture);
	texture.Save("texture.bmp");


	//���� �����... � ���� - �������
// 	fillTextureRBF(TextureCoordinates, Colors, width, height, texture);
// 	texture.Save("textureRBF.bmp");

	return 0;
}

