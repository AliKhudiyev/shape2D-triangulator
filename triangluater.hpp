#include <vector>
#include <algorithm>
#include <cmath>

#define epsilon 1e-5

struct Vertex{
    float data[3];
    bool operator==(const Vertex& v) const;
};

struct Line{
    Vertex v[2];
};

struct Triangle{
    Vertex v[3];
};

using vertex_t = Vertex;
using coef_t = Vertex;
using line_t = Line;
using triangle_t = Triangle;

enum Side{
    /* 
    * For a vertical line: if a point is on the left side
    * Otherwise: if a point is below
    * 
    * If every point of a line if FIRST
    */
    FIRST = 0, // in binary mode, MIDDLE == FIRST
    
    /* 
    * If a point is on the line
    * 
    * If a line is on the line
    */
    MIDDLE,

    /* 
    * For a vertical line: if a point is on the right side
    * Otherwise: if a point is above
    * 
    * If every point of a line is SECOND
    */
    SECOND,

    FIRST_MIDDLE,
    SECOND_MIDDLE,
    BOTH
};

enum ShapeType{
    CONVEX = 0,
    CONCAVE
};

enum FunctionType{
    VALID = -1,
    NONVALID = 1  // when a line is vertical: x = const
};

#define IS_INSIDE       Side::FIRST
#define IS_ON_SIDE      Side::MIDDLE
#define IS_NOT_INSIDE   Side::SECOND

#define FUNC_REGULAR    FunctionType::VALID
#define FUNC_VERTICAL   FunctionType::NONVALID

bool is_equal(float a, float b);
bool is_lessthan(float a, float b);
bool is_greaterthan(float a, float b);
coef_t tell_coefs(const line_t& line);
FunctionType tell_functype(const coef_t& coefs);
float calc(float x, const coef_t& coefs);
float tell_y(float x, const coef_t& coefs);
float tell_x(float y, const coef_t& coefs);

Side is_inside(const vertex_t& v, const triangle_t& triangle);
Side tell_side(const vertex_t& v, const line_t& line);
Side tell_side(const line_t& l, const line_t& line);
Side tell_binary_side(const vertex_t& v, const line_t& line);
Side tell_binary_side(const line_t&, const line_t& line);
ShapeType tell_shape(const std::vector<vertex_t>& vertices);
ShapeType tell_shape(const line_t& line, const std::vector<vertex_t>& vertices);
vertex_t tell_intersection(const coef_t& coefs1, const coef_t& coefs2);
bool intersects(const line_t& l1, const line_t& l2);

bool is_separable(unsigned index1, unsigned index2, const std::vector<vertex_t>& vertices);
void triangulate(const std::vector<vertex_t>& vertices, std::vector<triangle_t>& triangles, std::vector<unsigned>& indices, std::vector<unsigned> _indices = std::vector<unsigned>());
