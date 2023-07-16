#include "stdafx.h"

#include <omp.h>

#include "filltexture.h"

//- Alkud start

#define fixed int

inline
S3DLVector3 Dec2Bar(S3DLVector2 p1, S3DLVector2 p2, S3DLVector2 p3, S3DLVector2 p)
{
	float k1 = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
	float k2 = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
	float k3 = 1 - k1 - k2;
	return S3DLVector3(k1, k2, k3);
}

inline
void swap(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}
inline 
fixed int_to_fixed(int value)
{
	return (value << 16);
}
inline
int fixed_to_int(fixed value)
{
	return (value < 0) ? ((value >> 16) - 1) : (value >> 16);
}

void fillTreangle(const S3DLArray<S3DLVector2>& TextureCoordinates, const S3DLArray<S3DLVector3>& Colors, int i0, int i1, int i2, S3DLColorPicture& texture)

{
	if(TextureCoordinates[i1].y < TextureCoordinates[i0].y)
	{
		swap(i0, i1);
	} // ����� p1, p2 �����������
	if(TextureCoordinates[i2].y < TextureCoordinates[i0].y)
	{
		swap(i0, i2);
	} // ����� p1, p3 �����������
	if(TextureCoordinates[i1].y > TextureCoordinates[i2].y)
	{
		swap(i1, i2);
	}
	
	const S3DLVector2 p[3] = {S3DLVector2(TextureCoordinates[i0].x * texture.width(), TextureCoordinates[i0].y * texture.height()), S3DLVector2(TextureCoordinates[i1].x * texture.width(), TextureCoordinates[i1].y * texture.height()), S3DLVector2(TextureCoordinates[i2].x * texture.width(), TextureCoordinates[i2].y * texture.height())};

	const int x0 = p[0].x;
	const int x1 = p[1].x;
	const int x2 = p[2].x;

	const int y0 = p[0].y;
	const int y1 = p[1].y;
	const int y2 = p[2].y;

	// ���������� �� ��� x ��� ��� ������
	// ������������
	// ��������� ����������
	// � ������, ���� �������� ���� �����
	// ���������, ����������
	// ���������� ������� ����
	fixed dx13 = (y2 != y0) ? int_to_fixed(x2 - x0) / (y2 - y0) : 0;
	fixed dx12 = (y1 != y0) ? int_to_fixed(x1 - x0) / (y1 - y0) : 0;
	fixed dx23 = (y2 != y1) ? int_to_fixed(x2 - x1) / (y2 - y1) : 0;

	// "������� �����"
	// ���������� ��� ��������� � ������� �����
	fixed wx1 = int_to_fixed(x0);
	fixed wx2 = wx1;

	// ��������� ���������� dx13 � ������ ����������
	int _dx13 = dx13;

	// ������������� ���������� ����� �������, �����
	// � �������� ������ ���������
	// ����� wx1 ���� ������ ����� wx2
	if(dx13 > dx12)
	{
		swap(dx13, dx12);
	}

	// ����������� ������� ���������������
	for(int i = y0; i < y1; i++)
	{
		// ������ �������������� ����� ����� �������� �������
		for(int j = fixed_to_int(wx1); j <= fixed_to_int(wx2); j++)
		{
			S3DLVector3 baric = Dec2Bar(p[0], p[1], p[2], S3DLVector2(j, i));
			S3DLVector3 color = (Colors[i0] * baric.x + Colors[i1] * baric.y + Colors[i2] * baric.z) * 255.0f;
			texture.SetAt(j, i, S3DLRGB((unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z));
		}
		wx1 += dx13;
		wx2 += dx12;
	}

	// ����������� ������, ����� �������� ���������������� ���
	// ���� �������� ������� ����� �� ��� x,
	// �.�. ���������� ��� ���������
	if(y0 == y1)
	{
		wx1 = int_to_fixed(min(x0, x1));
		wx2 = int_to_fixed(max(x0, x1));
	}

	// ������������� ����������
	// (���������� ����������� ����������)
	if(_dx13 < dx23)
	{
		swap(_dx13, dx23);
	}

	// ����������� ������ ���������������
	for(int i = y1; i <= y2; i++)
	{
		// ������ �������������� ����� ����� �������� �������
		for(int j = fixed_to_int(wx1); j <= fixed_to_int(wx2); j++)
		{
			S3DLVector3 baric = Dec2Bar(p[0], p[1], p[2], S3DLVector2(j, i));
			S3DLVector3 color = (Colors[i0] * baric.x + Colors[i1] * baric.y + Colors[i2] * baric.z) * 255.0f;
			texture.SetAt(j, i, S3DLRGB((unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z));
		}
		wx1 += _dx13;
		wx2 += dx23;
	}
}

//- Alkud end




#include "libqhullcpp/Qhull.h"
#include "libqhullcpp/QhullPoints.h"
#include "libqhullcpp/QhullFacetList.h"
#include "libqhullcpp/QhullVertexSet.h"
void fillTexture(const S3DLArray<S3DLVector2>& TextureCoordinates, const S3DLArray<S3DLVector3>& Colors, const int width, const int height, S3DLColorPicture & texture)
{
	// �� ���� ����� �� �����
	QHULL_LIB_CHECK

	// ����������� texture !!!
	texture.resize(width, height);
	
	// ������������� foat � double
	const int sz = TextureCoordinates.size();
	std::vector<double> texc(sz * 2);
	for(int i = 0; i < sz; ++i)
	{
		texc[2 * i + 0] = TextureCoordinates[i].x;
		texc[2 * i + 1] = TextureCoordinates[i].y;
	}

	//���������������
	orgQhull::Qhull qhull("", 2, sz, texc.data(), "d Qt");
	
	//��������� ������� ���� �������������

// 	// � �������������
// 	std::vector<orgQhull::QhullFacet> fs = qhull.facetList().toStdVector();
// 	const int szf = fs.size();
// #pragma omp parallel for schedule(guided)
// 	for(int i = 0; i < szf; ++i)
// 	{
// 		const orgQhull::QhullFacet &f = fs[i];
// 		if(!f.isGood()) continue;
// 		const orgQhull::QhullVertexSet &v = f.vertices();
// 		fillTreangle(TextureCoordinates, Colors, v[0].point().id(), v[1].point().id(), v[2].point().id(), texture);
// 	}

	// ��� ������������, ���� ����� ������
	//long long t = GetTickCount();
	for(orgQhull::QhullFacet f = qhull.beginFacet(); f != qhull.endFacet(); f = f.next())
	{
		if(!f.isGood()) continue;
		const orgQhull::QhullVertexSet &v = f.vertices();
		fillTreangle(TextureCoordinates, Colors, v[0].point().id(), v[1].point().id(), v[2].point().id(), texture);
	}
	//std::cout << GetTickCount() - t << "sec\n";
}


// ����������� � RBF, ����� ��� �������� � ���� Alglib ����
#include "alglib/src/interpolation.h"
void fillTextureRBF(const S3DLArray<S3DLVector2>& TextureCoordinates, const S3DLArray<S3DLVector3>& Colors, const int width, const int height, S3DLColorPicture & texture)
{
	// ����������� texture !!!
	texture.resize(width, height);
	
	// ����������� RBF
	alglib::rbfmodel model;
	alglib::rbfreport rep;

	rbfcreate(2, 3, model);

	// ������������� 
	const int sz = TextureCoordinates.size();

	alglib::real_2d_array points;
	points.setlength(sz, 5);

	for(int i = 0; i < sz; ++i)
	{
		points[i][0] = TextureCoordinates[i].x * width;
		points[i][1] = TextureCoordinates[i].y * height;

		points[i][2] = Colors[i].r * 255;
		points[i][3] = Colors[i].g * 255;
		points[i][4] = Colors[i].b * 255;
	}

	//��������� ������ RBF
	double RBase = 500.0; // ������� ��������� ����� �������
	int NLayers = 3;// log(2.0*RBase) / log(2.0) + 2; // ������ ���� ������� ������� ��������� ����� ����� ���������
	rbfsetalgomultilayer(model, RBase, NLayers);

	rbfsetpoints(model, points);
	rbfbuildmodel(model, rep);

	// ��������� ��������
#pragma omp parallel for schedule(guided)
	for(int y = 0; y < height; ++y)
	{
		alglib::rbfmodel model1 = model;
		for(int x = 0; x < width; ++x)
		{
			alglib::real_1d_array p, f;
			p.setlength(2);
			p[0] = x;
			p[1] = y;
			rbfcalc(model1, p, f);
			S3DLRGB c;
			c.r = f[0] < 0 ? 0 : (f[0] > 255 ? 255 : f[0]);
			c.g = f[1] < 0 ? 0 : (f[1] > 255 ? 255 : f[1]);
			c.b = f[2] < 0 ? 0 : (f[2] > 255 ? 255 : f[2]);
			
			texture.SetAt(x, y, c);
		}
	}
}
