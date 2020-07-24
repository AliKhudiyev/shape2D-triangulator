# shape2D-triangulator

Shape2D-Triangulator is to triangulate any 2D shape or in other words, convert it to a bunch of triangles. The application of this project is rendering complex shapes in my own [LayZ Rendering Engine](https://github.com/AliKhudiyev/LayZ-Renderer-Engine). Since any 2/3D shape can be drawn with the help of triangles there is a need to use a library which expresses complex 2D shapes as triangles.

## Algorithm

I have come up with a fairly simple algorithm. Although it can be slower(it actually needs some heavy optimization) it does the job for now.

Algorithm to confirm if given points(*point1* and *point2*) are **Pairable Points**:

```
1. Draw a line between point1 and point2
2. Add every side of the shape, which excludes point1 and point2, to a set
3. If the line intersects with any line inside the set then [return False]
4. If the line is outside the shape then [return False]
5. [Return True]
```

Algorithm to triangulate given 2D shape:

```
1. If the shape has only 3 vertices [return it as a triangle]
2. Find a pair of points which are Pairable Points
3. Draw a line between them and obtain two sub-shapes
4. Do step #1 with the obtained sub-shapes
```

## How to use

Download the project and then open a terminal from the project folder. Type the following commands in the terminal:

```bash
mkdir build
cd build && cmake .. && make
```

### Program arguments

There are a couple of options for now:

- Give vertices to the program manually
- Import a csv file that contains all the vertices

To give all the vertices manually:

```bash
./main
> [vertex1.x, vertex1.y]
> [vertex2.x, vertex2.y]
> ...
> q # or ^C to begin the process
```

To import a csv file:

```bash
./main [path/to/file]
```

The csv file has to be formatted as shown below:

```
[vertex1.x, vertex1.y]
[vertex2.x, vertex2.y]
[vertex3.x, vertex3.y]
...
```

*Note:* If any vertex is ill-formed(i.e. vertex containing a character) or the number of vertices is less than 3 the program returns nothing.
