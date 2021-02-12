#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <iomanip>
#include <map>

#define epsilon 1e-3
#define EXTENDED 2
#define NOT_EXTENDED 3

struct Vertex{
    float data[4];
    bool operator==(const Vertex& v) const;
    Vertex operator+(const Vertex& v) const;
    Vertex operator-(const Vertex& v) const;
    Vertex& operator+=(const Vertex& v);
    Vertex& operator-=(const Vertex& v);
    Vertex operator*(float k) const;
    Vertex& operator*=(float k);
    Vertex mult(float k) const{
        Vertex result;
        result.data[0] *= k;
        result.data[1] *= k;
        return result;
    }
    Vertex add(const Vertex& v) const{
        Vertex result;
        result.data[0] += v.data[0];
        result.data[1] += v.data[1];
        return result;
    }
    Vertex subtract(const Vertex& v) const{
        return add(v.mult(-1));
    }

    friend std::ostream& operator<<(std::ostream& out, const Vertex& v);
    void print() const;
};
std::ostream& operator<<(std::ostream& out, const Vertex& v);

struct Line{
    Vertex v[2];
    friend std::ostream& operator<<(std::ostream& out, const Line& l);
};
std::ostream& operator<<(std::ostream& out, const Line& l);

struct Triangle{
    Vertex v[3];
    friend std::ostream& operator<<(std::ostream& out, const Triangle& tri);
};
std::ostream& operator<<(std::ostream& out, const Triangle& tri);

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

// Basic math utility (for mathematical functions)

bool is_equal(float a, float b);
bool is_lessthan(float a, float b);
bool is_greaterthan(float a, float b);
coef_t tell_coefs(const line_t& line);
FunctionType tell_functype(const coef_t& coefs);
float calc(float x, const coef_t& coefs);
float tell_y(float x, const coef_t& coefs);
float tell_x(float y, const coef_t& coefs);

// ================================================
// Triangulator

bool is_inside(const vertex_t& v, const triangle_t& triangle);
Side tell_side(const vertex_t& v, const line_t& line);
Side tell_side(const line_t& l, const line_t& line);
Side tell_binary_side(const vertex_t& v, const line_t& line);
Side tell_binary_side(const line_t&, const line_t& line);
ShapeType tell_shape(const std::vector<vertex_t>& vertices);
ShapeType tell_shape(const line_t& line, const std::vector<vertex_t>& vertices);
vertex_t tell_intersection(const coef_t& coefs1, const coef_t& coefs2);
vertex_t tell_intersection2(const line_t& line1, const line_t& line2);
bool intersects(const line_t& l1, const line_t& l2);

static std::vector<unsigned> __indices;

bool is_separable(unsigned index1, unsigned index2, const std::vector<vertex_t>& vertices);
void triangulate(const std::vector<vertex_t>& vertices, std::vector<triangle_t>& triangles, std::vector<unsigned>& indices, std::vector<unsigned> _indices = std::vector<unsigned>());
void triangulate(const std::vector<vertex_t>& vertices, std::vector<unsigned>& indices);

// ================================================
// Random shape generation

using uvec = std::vector<unsigned>;
using ivec = std::vector<int>;

struct shape_policy{
    static float min_x_dist, max_x_dist;
    static float min_y_dist, max_y_dist;
};

struct concav_expn{
    triangle_t tris[2];
    uvec indices[2], line_indices[2];

    vertex_t vertex;
    unsigned line_index;
};

void set_shape_policy(float min_x, float min_y, float max_x, float max_y);
float generate_number(float a = 0.0f, float b = 1.0f);
vertex_t generate_point(float xmin = 0.0f, float ymin = 0.0f, float xmax = 1.0f, float ymax = 1.0f);
vertex_t generate_point(const std::vector<vertex_t>& vertices);
std::vector<vertex_t> generate_convex(unsigned nvertices);
std::vector<vertex_t> generate_concave(unsigned nvertices);
std::vector<vertex_t> generate_concave_holes(unsigned nvertices);
std::vector<vertex_t> generate_shape(unsigned nvertices);
std::vector<vertex_t> convex_polygon(unsigned nvertices, float radius = -1.0f);
unsigned get_line_index(unsigned i1, unsigned i2);
concav_expn make_concave(const triangle_t& triangle, const uvec& indices, const uvec& line_indices, unsigned desired_index);
std::vector<vertex_t> concave_polygon(unsigned nvertices, unsigned nbasevertices);
