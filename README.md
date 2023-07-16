# filltexture

Testing two methods for filling the texture

1. Create a delaunay triangulation (QHull) from the vertices and fill the texture with the vertices inside the triangles
2. RBF (radial basis function from Alglib) interpolation of the vertices to fill the texture

QHull downloaded from http://www.qhull.org/download/

Alglib downloaded from http://www.alglib.net/download.php

# build

```
./build.sh
```

# run

Create a texture with 2048x2048 pixels and fill it with 400 random color vertices and fill the texture with the two methods. Save the textures to a BMP file in the current directory.

**NOTE**: takes a while to run RBF interpolation. The delaunay triangulation is fast.

```
./filltexture
```
