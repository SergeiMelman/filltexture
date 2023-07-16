#include <omp.h>
#include <algorithm>

// Alglib
#include "interpolation.h"

// QHull
#include "libqhullcpp/Qhull.h"
#include "libqhullcpp/QhullPoints.h"
#include "libqhullcpp/QhullFacetList.h"
#include "libqhullcpp/QhullVertexSet.h"

#include "filltexture.h"

typedef int fixedFloat;

using namespace std;

// Calculate baricentric coordinates of point p in triangle p1, p2, p3
inline S3DLVector3
Dec2Bar(S3DLVector2 p1, S3DLVector2 p2, S3DLVector2 p3, S3DLVector2 p)
{
    float k1 = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y))
        / ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
    float k2 = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y))
        / ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
    float k3 = 1 - k1 - k2;
    return S3DLVector3(k1, k2, k3);
}

inline fixedFloat
int_to_fixedFloat(int value)
{
    return (value << 16);
}
inline int
fixedFloat_to_int(fixedFloat value)
{
    return (value < 0) ? ((value >> 16) - 1) : (value >> 16);
}

void
fillTreangle(const S3DLArray<S3DLVector2> & TextureCoordinates,
    const S3DLArray<S3DLVector3> & Colors, int i0, int i1, int i2, S3DLColorPicture & texture)

{
    // сортируем точки по возрастанию
    if(TextureCoordinates[i1].y < TextureCoordinates[i0].y) {
        swap(i0, i1);
    } // точки p1, p2 упорядочены
    if(TextureCoordinates[i2].y < TextureCoordinates[i0].y) {
        swap(i0, i2);
    } // точки p1, p3 упорядочены
    if(TextureCoordinates[i1].y > TextureCoordinates[i2].y) {
        swap(i1, i2);
    }

    // вычисляем приращения
    const S3DLVector2 p[3] = {//
        S3DLVector2(TextureCoordinates[i0].x * texture.width(),
            TextureCoordinates[i0].y * texture.height()),
        S3DLVector2(TextureCoordinates[i1].x * texture.width(),
            TextureCoordinates[i1].y * texture.height()),
        S3DLVector2(TextureCoordinates[i2].x * texture.width(),
            TextureCoordinates[i2].y * texture.height())};

    const int x0 = p[0].x;
    const int x1 = p[1].x;
    const int x2 = p[2].x;

    const int y0 = p[0].y;
    const int y1 = p[1].y;
    const int y2 = p[2].y;

    // приращения по оси x для трёх сторон
    // треугольника
    // вычисляем приращения
    // в случае, если ординаты двух точек
    // совпадают, приращения
    // полагаются равными нулю
    fixedFloat dx13 = (y2 != y0) ? int_to_fixedFloat(x2 - x0) / (y2 - y0) : 0;
    fixedFloat dx12 = (y1 != y0) ? int_to_fixedFloat(x1 - x0) / (y1 - y0) : 0;
    fixedFloat dx23 = (y2 != y1) ? int_to_fixedFloat(x2 - x1) / (y2 - y1) : 0;

    // "рабочие точки"
    // изначально они находятся в верхней точке
    fixedFloat wx1 = int_to_fixedFloat(x0);
    fixedFloat wx2 = wx1;

    // сохраняем приращение dx13 в другой переменной
    int _dx13 = dx13;

    // упорядочиваем приращения таким образом, чтобы
    // в процессе работы алгоритмы
    // точка wx1 была всегда левее wx2
    if(dx13 > dx12) {
        swap(dx13, dx12);
    }

    // растеризуем верхний полутреугольник
    for(int i = y0; i < y1; i++) {
        // рисуем горизонтальную линию между рабочими точками
        for(int j = fixedFloat_to_int(wx1); j <= fixedFloat_to_int(wx2); j++) {
            S3DLVector3 baric = Dec2Bar(p[0], p[1], p[2], S3DLVector2(float(j), float(i)));
            S3DLVector3 color
                = (Colors[i0] * baric.x + Colors[i1] * baric.y + Colors[i2] * baric.z) * 255.0f;
            texture.SetAt(j, i,
                S3DLRGB((unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z));
        }
        wx1 += dx13;
        wx2 += dx12;
    }

    // вырожденный случай, когда верхнего полутреугольника нет
    // надо разнести рабочие точки по оси x,
    // т.к. изначально они совпадают
    if(y0 == y1) {
        wx1 = int_to_fixedFloat(min(x0, x1));
        wx2 = int_to_fixedFloat(max(x0, x1));
    }

    // упорядочиваем приращения
    // (используем сохраненное приращение)
    if(_dx13 < dx23) {
        swap(_dx13, dx23);
    }

    // растеризуем нижний полутреугольник
    for(int i = y1; i <= y2; i++) {
        // рисуем горизонтальную линию между рабочими точками
        for(int j = fixedFloat_to_int(wx1); j <= fixedFloat_to_int(wx2); j++) {
            S3DLVector3 baric = Dec2Bar(p[0], p[1], p[2], S3DLVector2(float(j), float(i)));
            S3DLVector3 color
                = (Colors[i0] * baric.x + Colors[i1] * baric.y + Colors[i2] * baric.z) * 255.0f;
            texture.SetAt(j, i,
                S3DLRGB((unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z));
        }
        wx1 += _dx13;
        wx2 += dx23;
    }
}

void
fillTexture(const S3DLArray<S3DLVector2> & TextureCoordinates,
    const S3DLArray<S3DLVector3> & Colors, const int width, const int height,
    S3DLColorPicture & texture)
{
    // не знаю зачем но нужно
    QHULL_LIB_CHECK

    // подготовить texture !!!
    texture.resize(width, height);

    // преобразовать foat в double
    const int sz = TextureCoordinates.size();
    std::vector<double> texc(sz * 2);
    for(int i = 0; i < sz; ++i) {
        texc[2 * i + 0] = TextureCoordinates[i].x;
        texc[2 * i + 1] = TextureCoordinates[i].y;
    }

    // треангулировать
    orgQhull::Qhull qhull("", 2, sz, texc.data(), "d Qt");

    // запустить заливку всех треугольников

    // 	// с параллелизмой
    // 	std::vector<orgQhull::QhullFacet> fs = qhull.facetList().toStdVector();
    // 	const int szf = fs.size();
    // #pragma omp parallel for schedule(guided)
    // 	for(int i = 0; i < szf; ++i)
    // 	{
    // 		const orgQhull::QhullFacet &f = fs[i];
    // 		if(!f.isGood()) continue;
    // 		const orgQhull::QhullVertexSet &v = f.vertices();
    // 		fillTreangle(TextureCoordinates, Colors, v[0].point().id(), v[1].point().id(),
    // v[2].point().id(), texture);
    // 	}

    // без параллелизму, тоже очень быстро
    // long long t = GetTickCount();
    for(orgQhull::QhullFacet f = qhull.beginFacet(); f != qhull.endFacet(); f = f.next()) {
        if(!f.isGood())
            continue;
        const orgQhull::QhullVertexSet & v = f.vertices();
        fillTreangle(TextureCoordinates, Colors, v[0].point().id(), v[1].point().id(),
            v[2].point().id(), texture);
    }
    // std::cout << GetTickCount() - t << "sec\n";
}

// наигравшись с RBF, можно сие грохнуть и весь Alglib тоже
void
fillTextureRBF(const S3DLArray<S3DLVector2> & TextureCoordinates,
    const S3DLArray<S3DLVector3> & Colors, const int width, const int height,
    S3DLColorPicture & texture)
{
    // prepare texture !!!
    texture.resize(width, height);

    // prepare RBF
    alglib::rbfmodel model;
    alglib::rbfreport rep;

    rbfcreate(2, 3, model);

    const int sz = TextureCoordinates.size();

    alglib::real_2d_array points;
    points.setlength(sz, 5);

    // convert into alglib format
    for(int i = 0; i < sz; ++i) {
        points[i][0] = TextureCoordinates[i].x * width;
        points[i][1] = TextureCoordinates[i].y * height;

        points[i][2] = Colors[i].r * 255.0f;
        points[i][3] = Colors[i].g * 255.0f;
        points[i][4] = Colors[i].b * 255.0f;
    }

    // build RBF model
    // average distance between points
    double RBase = 500.0;
    // log(2.0*RBase) / log(2.0) + 2; // хороша если разброс средних дистанций имеет малую дисперсию
    int NLayers = 3;
    rbfsetalgomultilayer(model, RBase, NLayers);

    rbfsetpoints(model, points);
    rbfbuildmodel(model, rep);

    // заполнить текстуру
    const int n = omp_get_max_threads();
    vector<alglib::rbfmodel> modelv(n, model);

// NON OMP time: real    4m46.437s; user    4m46.394s
// OMP time:     real    0m29.320s; user    5m18.604s
#pragma omp parallel for schedule(guided) shared(modelv, texture)
    for(int y = 0; y < height; ++y) {
        const int threadnum = omp_get_thread_num();
        alglib::rbfmodel & modeln = modelv[threadnum];
        alglib::real_1d_array p, f;
        p.setlength(2);
        for(int x = 0; x < width; ++x) {
            p[0] = x;
            p[1] = y;
            rbfcalc(modeln, p, f);
            const S3DLRGB c(
                clamp(f[0], 0.0, 255.0), clamp(f[1], 0.0, 255.0), clamp(f[2], 0.0, 255.0));

            texture.SetAt(x, y, c);
        }
    }
}
