#include <time.h>
#include <string>

#include "filltexture.h"

using namespace std;

// для всяческих тестов
// for various tests
int
main()
{
    // number of points
    int const coordsSize = 40; // к-во точек
    // size of future texture
    int const width = 2048; // размер будущей текстуры
    int const height = 2048;

    S3DLArray<S3DLVector2> TextureCoordinates; // размер
    TextureCoordinates.resize(coordsSize);

    S3DLArray<S3DLVector3> Colors;
    Colors.resize(coordsSize);

    srand((unsigned)time(NULL));
    for(int i = 0; i < coordsSize; ++i) {
        // нельзя генерировать точку на краю
        // can't generate a point on the edge.
        TextureCoordinates[i].x = (float)(rand() - 10) / RAND_MAX;
        TextureCoordinates[i].y = (float)(rand() - 10) / RAND_MAX;
        Colors[i].r = (float)rand() / RAND_MAX;
        Colors[i].g = (float)rand() / RAND_MAX;
        Colors[i].b = (float)rand() / RAND_MAX;
    }

    string suf = to_string(rand());
    S3DLColorPicture texture;
    fillTexture(TextureCoordinates, Colors, width, height, texture);
    texture.Save(("texture" + suf + ".bmp").c_str());

    // дюже долго... а жаль - красиво
    // very long... but it's a pity - it's beautiful
    fillTextureRBF(TextureCoordinates, Colors, width, height, texture);
    texture.Save(("textureRBF" + suf + ".bmp").c_str());
    return 0;
}
