#include "triangluator.hpp"

#include <iostream>
#include <unistd.h>

bool Vertex::operator==(const Vertex& v) const{
    if(abs(data[0] - v.data[0]) < epsilon && 
        abs(data[1] - v.data[1]) < epsilon && 
        abs(data[2] - v.data[2]) < epsilon){
            return true;
        }
    return false;
}

Vertex Vertex::operator+(const Vertex& v) const{
    Vertex res;
    res.data[0] = data[0] + v.data[0];
    res.data[1] = data[1] + v.data[1];
    res.data[2] = data[2] + v.data[2];
    return res;
}

Vertex Vertex::operator-(const Vertex& v) const{
    Vertex res;
    res.data[0] = data[0] - v.data[0];
    res.data[1] = data[1] - v.data[1];
    res.data[2] = data[2] - v.data[2];
    return res;
}

Vertex& Vertex::operator+=(const Vertex& v){
    data[0] += v.data[0];
    data[1] += v.data[1];
    data[2] += v.data[2];
    return *this;
}

Vertex& Vertex::operator-=(const Vertex& v){
    data[0] -= v.data[0];
    data[1] -= v.data[1];
    data[2] -= v.data[2];
    return *this;
}

Vertex Vertex::operator*(float k) const{
    Vertex res;
    res.data[0] = data[0] * k;
    res.data[1] = data[1] * k;
    res.data[2] = data[2] * k;
    return res;
}

Vertex& Vertex::operator*=(float k){
    data[0] *= k;
    data[1] *= k;
    data[2] *= k;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Vertex& v){
    out<<'('<<v.data[0]<<", "<<v.data[1]<<", "<<v.data[2]<<')';
    return out;
}

void Vertex::print() const{
    std::cout<<'('<<data[0]<<", "<<data[1]<<')';
}

std::ostream& operator<<(std::ostream& out, const Line& l){
    out<<l.v[0]<<'-'<<l.v[1];
    return out;
}

std::ostream& operator<<(std::ostream& out, const Triangle& tri){
    out<<tri.v[0]<<'-'<<tri.v[1]<<'-'<<tri.v[2];
    return out;
}

// =============================================

bool is_equal(float a, float b){
    if(abs(a-b) < epsilon) return true;
    return false;
}

bool is_lessthan(float a, float b){
    if(!is_equal(a, b) && a < b) return true;
    return false;
}

bool is_greaterthan(float a, float b){
    return (!is_equal(a, b) && !is_lessthan(a, b));
}

coef_t tell_coefs(const line_t& line){
    coef_t coefs;

    if(is_equal(line.v[0].data[0], line.v[1].data[0])){
        if(is_equal(line.v[0].data[1], line.v[1].data[1])){
            coefs = {0.0f, line.v[0].data[1], 1.0f};
        } else{
            coefs = {line.v[0].data[0], 0.0f, -1.0f};
        }
    } else{
        coefs.data[0] = (line.v[0].data[1] - line.v[1].data[1]) / (line.v[0].data[0] - line.v[1].data[0]);
        coefs.data[1] = line.v[0].data[1] - coefs.data[0] * line.v[0].data[0];
        coefs.data[2] = 1.0;
    }

    return coefs;
}

FunctionType tell_functype(const coef_t& coefs){
    if(coefs.data[2] >= -0.1f) return FunctionType::VALID;
    return FunctionType::NONVALID;
}

float calc(float x, const coef_t& coefs){
    auto type = tell_functype(coefs);

    if(type == FUNC_REGULAR){
        return coefs.data[0] * x + coefs.data[1];
    }
    return 0.0f;
}

float tell_y(float x, const coef_t& coefs){
    return calc(x, coefs);
}

float tell_x(float y, const coef_t& coefs){
    auto type = tell_functype(coefs);

    if(type == FUNC_REGULAR){
        return (y - coefs.data[1]) / coefs.data[0];
    }
    return coefs.data[0];
}


bool is_inside(const vertex_t& v, const triangle_t& triangle){
    line_t line[3];
    std::vector<vertex_t> vertices(3);
    std::vector<line_t> lines(3);

    for(unsigned i=0;i<3;++i){
        vertices[i] = triangle.v[i];
        lines[i] = {triangle.v[i], triangle.v[(i+1)%3]};
        if(tell_side(v, lines[i]) == Side::MIDDLE) return true;
    }
    for(unsigned i=0;i<3;++i){
        line[i] = {v, triangle.v[i]};
        if(tell_shape(line[i], vertices) == ShapeType::CONVEX) return false;
    }
    return true;
}

Side tell_side(const vertex_t& v, const line_t& line){
    auto coefs = tell_coefs(line);
    auto type = tell_functype(coefs);

    // std::cout<<coefs.data[0]<<" - "<<coefs.data[1]<<" |validity: "<<coefs.data[2]<<'\n';
    // std::cout<<"calc: "<<calc(v.data[0], coefs)<<'\n';
    // std::cout<<"vdata: "<<v.data[0]<<" and "<<v.data[1]<<'\n';
    if(type == FUNC_REGULAR){
        if(is_lessthan(v.data[1], calc(v.data[0], coefs))) return Side::FIRST;
        else if(is_equal(v.data[1], calc(v.data[0], coefs))) return Side::MIDDLE;
        return Side::SECOND;
    } else{
        // std::cout<<"\tFunc vertical\n";
        if(is_lessthan(v.data[0], coefs.data[0])) return Side::FIRST;
        else if(is_equal(v.data[0], coefs.data[0])) return Side::MIDDLE;
        return Side::SECOND;
    }
}

Side tell_side(const line_t& l, const line_t& line){
    auto side1 = tell_side(l.v[0], line);
    auto side2 = tell_side(l.v[1], line);

    if((side1 == Side::FIRST && side2 == Side::SECOND) ||
        (side2 == Side::FIRST && side1 == Side::SECOND)){
            return Side::BOTH;
        }
    else if(side1 != Side::MIDDLE){
        return side1;
    }

    return side2;
}

Side tell_binary_side(const vertex_t& v, const line_t& line){
    if(tell_side(v, line) == Side::MIDDLE)
        return Side::FIRST;
    return tell_side(v, line);
}

Side tell_binary_side(const line_t& l, const line_t& line){
    auto v1 = l.v[0];
    auto v2 = l.v[1];

    auto side1 = tell_side(v1, line);
    auto side2 = tell_side(v2, line);

    if((side1 == Side::FIRST && side2 == Side::SECOND) ||
        (side2 == Side::FIRST && side1 == Side::SECOND)){
        return Side::BOTH;
    } else if(side1 != Side::MIDDLE){
        return side1;
    }
    return side2;
}

ShapeType tell_shape(const std::vector<vertex_t>& vertices){
    std::vector<line_t> lines;
    unsigned count = vertices.size();

    for(unsigned i=0;i<count;++i){
        lines.push_back({vertices[i], vertices[(i+1)%count]});
    }

    unsigned nlines = lines.size();

    for(unsigned i=0;i<nlines;++i){
        std::cout<<std::fixed<<std::setprecision(8);
        // std::cout<<"Line: "<<lines[i].v[0]<<" - "<<lines[i].v[1]<<'\n';
        if(tell_shape(lines[i], vertices) == ShapeType::CONCAVE)
            return ShapeType::CONCAVE;
    }
    return ShapeType::CONVEX;
}

ShapeType tell_shape(const line_t& line, const std::vector<vertex_t>& vertices){
    auto side = tell_side(vertices[0], line);
    unsigned count = vertices.size();
    // std::cout<<"# of vertices: "<<count<<'\n';
    for(unsigned i=0;i<count;++i){
        // std::cout<<" > "<<(int)side<<'\n';
        auto v = vertices[(i+1)%count];
        // std::cout<<"next point: "<<v<<'\n';
        if(side == Side::MIDDLE){
            // std::cout<<"continue...\n";
            side = tell_side(v, line);
            continue;
        }
        auto new_side = tell_side(v, line);
        // std::cout<<" |> "<<new_side<<'\n';
        if(new_side != Side::MIDDLE && side != new_side){
            // std::cout<<" >>> CONCAVE\n";
            return ShapeType::CONCAVE;
        }
    }
    // std::cout<<" >>> CONVEX\n";
    return ShapeType::CONVEX;
}

vertex_t tell_intersection(const coef_t& coefs1, const coef_t& coefs2){
    vertex_t point;
    auto type1 = tell_functype(coefs1);
    auto type2 = tell_functype(coefs2);


    if(type1 == FUNC_VERTICAL && type2 == FUNC_VERTICAL){
        point.data[2] = -1.0f; // do not intersect
    } else if(type1 == FUNC_VERTICAL){
        point.data[0] = coefs1.data[0];
        point.data[1] = calc(point.data[0], coefs2);
        point.data[2] = 1.0f;
    } else if(type2 == FUNC_VERTICAL){
        point.data[0] = coefs2.data[0];
        point.data[1] = calc(point.data[0], coefs1);
        point.data[2] = 1.0f;
    } else{
        if(is_equal(coefs1.data[0], coefs2.data[0])){
            point.data[2] = -1.0f; // do not intersect
        } else{
            point.data[0] = (coefs1.data[1] - coefs2.data[1]) / (coefs2.data[0] - coefs1.data[0]);
            point.data[1] = calc(point.data[0], coefs1);
            point.data[2] = 1.0f;
        }
    }

    return point;
}

bool intersects(const line_t& l1, const line_t& l2){
    auto coefs1 = tell_coefs(l1);
    auto coefs2 = tell_coefs(l2);
    auto point = tell_intersection(coefs1, coefs2);

    if(point.data[2] < 0.0f) return false;

    float xmin1 = l1.v[0].data[0], xmax1 = l1.v[1].data[0];
    float ymin1 = l1.v[0].data[1], ymax1 = l1.v[1].data[1];
    
    if(xmin1 > xmax1){
        float tmp = xmin1; xmin1 = xmax1; xmax1 = tmp;
    }
    if(ymin1 > ymax1){
        float tmp = ymin1; ymin1 = ymax1; ymax1 = tmp;
    }

    float xmin2 = l2.v[0].data[0], xmax2 = l2.v[1].data[0];
    float ymin2 = l2.v[0].data[1], ymax2 = l2.v[1].data[1];

    if(xmin2 > xmax2){
        float tmp = xmin2; xmin2 = xmax2; xmax2 = tmp;
    }
    if(ymin2 > ymax2){
        float tmp = ymin2; ymin2 = ymax2; ymax2 = tmp;
    }

    if(!is_greaterthan(xmin1, point.data[0]) && !is_greaterthan(point.data[0], xmax1) && 
        !is_greaterthan(xmin2, point.data[0]) && !is_greaterthan(point.data[0], xmax2) &&
        
        !is_greaterthan(ymin1, point.data[1]) && !is_greaterthan(point.data[1], ymax1) && 
        !is_greaterthan(ymin2, point.data[1]) && !is_greaterthan(point.data[1], ymax2)){
            return true;
        }

    return false;
}


bool is_separable(unsigned index1, unsigned index2, const std::vector<vertex_t>& vertices){
    // TO DO
    if(index1 == index2 || index1+1 == index2 || 
        index1 == index2+1 || 
        (index1 == 0 && index2 == vertices.size()-1) ||
        (index2 == 0 && index1 == vertices.size()-1)){
        return false;
    }

    auto v1 = vertices[index1];
    auto v2 = vertices[index2];
    auto line = line_t{v1, v2};
    unsigned count = vertices.size();
    std::vector<line_t> lines;

    for(unsigned i=0;i<count;++i){
        if(i==index1 || i==index2 ||
            (i+1)%count==index1 || (i+1)%count==index2){ continue; }
        lines.push_back({vertices[i], vertices[(i+1)%count]});
    }

    for(const auto& l: lines){
        if(intersects(line, l)) return false;
    }
    if(tell_shape(line, vertices) == ShapeType::CONVEX){ 
        return false;
    }

    return true;
}

void triangulate(const std::vector<vertex_t>& vertices, std::vector<triangle_t>& triangles, std::vector<unsigned>& indices, std::vector<unsigned> _indices){
    if(_indices.empty()){
        //std::cout<<"filling the _indices!\n";
        for(unsigned i=0;i<vertices.size();++i) _indices.push_back(i);
    }
    unsigned count = vertices.size();
    //std::cout<<" |>| Beginning with count of "<<count<<"\n\n";

    if(count == 3){
        auto triangle = triangle_t{vertices[0], vertices[1], vertices[2]};
        triangles.push_back(triangle);
        for(unsigned i=0;i<3;++i) indices.push_back(_indices[i]);
        //std::cout<<" | Found! "<<'\n';
        return ;
    }

    for(unsigned i=0;i<count;++i){
        //std::cout<<"Iteration #"<<i<<"\n";
        for(unsigned j=0;j<=count;++j){
            //std::cout<<" sub iteration #"<<j<<'\n';
            if(is_separable(i, j%count, vertices)){
                std::cout<<"> Joining indices "<<_indices[i]<<" and "<<_indices[(j)%count]<<'\n';
                std::vector<vertex_t> verts1, verts2;
                std::vector<unsigned> _inds1, _inds2;

                auto it1 = std::find(vertices.cbegin(), vertices.cend(), vertices[i]) - vertices.cbegin();
                auto it2 = std::find(vertices.cbegin(), vertices.cend(), vertices[(j)%count]) - vertices.cbegin();
                // std::cout<<it2->data[0]<<", "<<it2->data[1]<<" ===\n";
                //std::cout<<" >> verts1 indices: ";
                for(auto it = it1; it <= it2; ++it){
                    verts1.push_back(vertices[it]);
                    _inds1.push_back(_indices[it]);
                    //std::cout<<_indices[it]<<' ';
                }
                //std::cout<<"\n >> verts2 indices: ";
                for(auto it = it2; it != count; ++it){
                    verts2.push_back(vertices[it]);
                    _inds2.push_back(_indices[it]);
                    //std::cout<<_indices[it]<<' ';
                }   //std::cout<<" ";
                for(auto it = 0; it <= it1; ++it){
                    verts2.push_back(vertices[it]);
                    _inds2.push_back(_indices[it]);
                    //std::cout<<_indices[it]<<' ';
                }   //std::cout<<'\n';

                //usleep(1000000);
                triangulate(verts1, triangles, indices, _inds1);
                triangulate(verts2, triangles, indices, _inds2);
                return ;
            }
        }
    }
}

void triangulate(const std::vector<vertex_t>& vertices, std::vector<unsigned>& indices){
    if(__indices.empty() || __indices.size() != vertices.size()){
        //std::cout<<"filling the _indices!\n";
        __indices.clear();
        for(unsigned i=0;i<vertices.size();++i) __indices.push_back(i);
    }
    unsigned count = vertices.size();
    //std::cout<<" |>| Beginning with count of "<<count<<"\n\n";

    if(count == 3){
        for(unsigned i=0;i<3;++i) indices.push_back(__indices[i]);
        //std::cout<<" | Found! "<<'\n';
        return ;
    }

    for(unsigned i=0;i<count;++i){
        //std::cout<<"Iteration #"<<i<<"\n";
        for(unsigned j=0;j<=count;++j){
            //std::cout<<" sub iteration #"<<j<<'\n';
            if(is_separable(i, j%count, vertices)){
                std::cout<<"> Joining indices "<<__indices[i]<<" and "<<__indices[(j)%count]<<'\n';
                std::vector<vertex_t> verts1, verts2;
                std::vector<unsigned> _inds1, _inds2;

                auto it1 = std::find(vertices.cbegin(), vertices.cend(), vertices[i]) - vertices.cbegin();
                auto it2 = std::find(vertices.cbegin(), vertices.cend(), vertices[(j)%count]) - vertices.cbegin();
                // std::cout<<it2->data[0]<<", "<<it2->data[1]<<" ===\n";
                //std::cout<<" >> verts1 indices: ";
                for(auto it = it1; it <= it2; ++it){
                    verts1.push_back(vertices[it]);
                    _inds1.push_back(__indices[it]);
                    //std::cout<<_indices[it]<<' ';
                }
                //std::cout<<"\n >> verts2 indices: ";
                for(auto it = it2; it != count; ++it){
                    verts2.push_back(vertices[it]);
                    _inds2.push_back(__indices[it]);
                    //std::cout<<_indices[it]<<' ';
                }   //std::cout<<" ";
                for(auto it = 0; it <= it1; ++it){
                    verts2.push_back(vertices[it]);
                    _inds2.push_back(__indices[it]);
                    //std::cout<<_indices[it]<<' ';
                }   //std::cout<<'\n';

                //usleep(1000000);
                triangulate(verts1, indices);
                triangulate(verts2, indices);
                return ;
            }
        }
    }
}

// ===========================================

#define STATE_CONVEX        0
#define STATE_CONCAVE       1
#define STATE_CONCAVE_HOLES 2

float shape_policy::min_x_dist = 0.1f;
float shape_policy::max_x_dist = 10.0f;
float shape_policy::min_y_dist = 0.1f;
float shape_policy::max_y_dist = 10.0f;


void set_shape_policy(float min_x, float min_y, float max_x, float max_y){
    shape_policy::min_x_dist = min_x;
    shape_policy::min_y_dist = min_y;
    shape_policy::max_x_dist = max_x;
    shape_policy::max_y_dist = max_y;
}

float generate_number(float a, float b){
    if(a>b){
        float tmp = a;
        a = b;
        b = tmp;
    }

    return (float)rand()/(float)RAND_MAX * (b - a) + a;
}

vertex_t generate_point(float xmin, float ymin, float xmax, float ymax){
    return {generate_number(xmin, xmax), generate_number(ymin, ymax)};
}

vertex_t generate_point(const std::vector<vertex_t>& vertices){
    vertex_t vertex;
    float k = generate_number();
    float coef = generate_number();
    unsigned count = vertices.size();
    coef = k = 0.5f;

    if(count == 3){
        for(unsigned i=0;i<count;++i){
            if(vertices[i] == vertices[(i+1)%count]){
                vertex = (vertices[(i+1)%3] + vertices[(i+2)%3]) * 0.5f;
                return vertex;
                std::cerr<<"ERROR[generator]: Cannot generate a point inside the line!\n";
                assert(false);
            }
        }

        float xdir = vertices[1].data[0] - vertices[0].data[0];
        float ydir = vertices[1].data[1] - vertices[0].data[1];
        float x = vertices[0].data[0] + xdir * coef;
        float y = vertices[0].data[1] + ydir * coef;
        float dx = (vertices[2].data[0] - vertices[1].data[0]) * (coef * k);
        float dy = (vertices[2].data[1] - vertices[1].data[1]) * (coef * k);
        
        vertex = {x + dx, y + dy};
    } else if(count == 4){
        std::vector<vertex_t> verts(3);
        float choice = generate_number();

        if(choice < 0.5f){
            verts[0] = vertices[0];
            verts[1] = vertices[1];
            verts[2] = vertices[2];
        } else{
            verts[0] = vertices[0];
            verts[1] = vertices[2];
            verts[2] = vertices[3];
        }
        
        return generate_point(verts);
    } else{
        std::cerr<<"ERROR[generator]: Point cannot be generated!\n";
        assert(false);
    }

    return vertex;
}

std::vector<vertex_t> generate_convex(unsigned nvertices){
    std::vector<vertex_t> vertices(nvertices);
    std::vector<vertex_t> tri(3), quad(4);
    vertex_t beg_point, end_point;
    vertex_t point1, point2;
    vertex_t dir1, dir2;
    vertex_t origin1, origin2;
    //float k = generate_number(0.1f);

    for(unsigned i=0;i<3;++i){
        vertices[i] = generate_point(0.0f, 0.0f, 4.0f, 4.0f);
        std::cout<<"Iteration #"<<i<<'\n';
        std::cout<<"Generating point...\n";
        if(i==2){
            vertices[2] = generate_point(4.0f, 0.0f, 8.0f, 4.0f);
        }
        std::cout<<vertices[i]<<"\n\n";
    }
    beg_point = vertices[0];
    origin1 = vertices[1];
    for(unsigned i=3;i<nvertices;++i){
        float k = generate_number(0.5f, 2.0f);
        std::cout<<"Iteration #"<<i<<'\n';
        end_point = vertices[i-1];
        origin2 = vertices[i-2];

        dir1 = beg_point - origin1;
        dir2 = end_point - origin2;
        point1 = end_point + dir2 * (5.0f * k);
        point2 = beg_point + dir1 * k;

        std::cout<<"Generating point...\n";
        
        coef_t coefs1 = tell_coefs({beg_point, origin1});
        coef_t coefs2 = tell_coefs({end_point, origin2});
        vertex_t int_point = tell_intersection(coefs1, coefs2);
        line_t int_line = {beg_point, end_point};

        if(!is_equal(int_point.data[2], -1.0f) && tell_side(int_point, int_line) != tell_side(vertices[1], int_line)){
            tri[0] = beg_point;
            tri[1] = end_point;
            tri[2] = int_point;

            vertices[i] = generate_point(tri);
        } else{
            quad[0] = beg_point;
            quad[1] = end_point;
            quad[2] = point1;
            quad[3] = point2;

            vertices[i] = generate_point(quad);
        }
        std::cout<<vertices[i]<<"\n\n";
    }

    return vertices;
}

std::vector<vertex_t> generate_concave(unsigned nvertices){
    std::vector<vertex_t> vertices(nvertices);

    ;

    return vertices;
}

std::vector<vertex_t> generate_concave_holes(unsigned nvertices){
    std::vector<vertex_t> vertices(nvertices);

    ;

    return vertices;
}

std::vector<vertex_t> generate_shape(unsigned nvertices){
    unsigned state = (unsigned)generate_number(0.0f, 3.0);

    switch (state)
    {
    case STATE_CONVEX:
        return generate_convex(nvertices);
        break;
    
    case STATE_CONCAVE:
        return generate_concave(nvertices);
        break;
    
    default: break;
    }

    return generate_concave_holes(nvertices);
}

std::vector<vertex_t> convex_polygon(unsigned nvertices){
    std::vector<vertex_t> vertices(nvertices);
    float r = (float)nvertices / 10.0f;
    float angle = 360.0f / (float)nvertices;
    float curr = 0.0, rad;

    for(unsigned i=0; i<nvertices; ++i, curr+=angle){
        rad = curr * M_PI / 180.0f;
        vertices[i] = {r*cosf(rad), r*sinf(rad)};
    }

    return vertices;
}

unsigned get_line_index(unsigned i1, unsigned i2){
    if(i1 == i2){
        std::cerr<<"ERROR[line index]: same indices!\n";
        assert(false);
    }

    if(i1>i2){
        unsigned tmp = i1;
        i1 = i2;
        i2 = tmp;
    }

    unsigned index = -1; // useless function!!!

    return index;
}

concav_expn _make_concave(const triangle_t& triangle, const uvec& indices, const uvec& line_indices, unsigned desired_index){
    concav_expn expn;

    vertex_t points[3] = {triangle.v[0], triangle.v[1], triangle.v[2]};
    vertex_t vertex = generate_point({points[0], points[1], points[2]});

    // std::cout<<" dbg: generated vertex is "<<vertex<<"\n";

    unsigned index = line_indices[0]; // Randomize!
    unsigned beg = indices[0], mid = indices[1], end = indices[2];
    unsigned neig = indices[(index+2)%3];

    std::cout<<" dbg: Adding post concav_expn section...\n";

    expn.vertex = vertex;
    expn.line_index = index;

    unsigned i1, i2;
    if(neig == beg){
        i1 = mid; i2 = end;
    }
    else if(neig == mid){
        i1 = beg; i2 = end;
    }
    else{
        i1 = beg; i2 = mid;
    }

    std::cout<<" dbg: beg/mid/end, neig: "<<beg<<'/'<<mid<<'/'<<end<<", "<<neig<<'\n';
    std::cout<<" dbg: i1/i2: "<<i1<<'/'<<i2<<'\n';
    std::cout<<" dbg: Adding first concav_expn section...\n";

    expn.indices[0].push_back(neig);
    expn.indices[0].push_back(desired_index);
    expn.indices[0].push_back(i1);

    expn.line_indices[0].push_back(1);
    int neig_index = std::find(indices.cbegin(), indices.cend(), neig) - indices.cbegin();
    int i1_index = std::find(indices.cbegin(), indices.cend(), i1) - indices.cbegin();
    unsigned small_index = neig_index < i1_index ? neig_index : i1_index;
    unsigned place = small_index + abs(neig_index-i1_index) - 1;
    if(std::find(line_indices.cbegin(), line_indices.cend(), place) != line_indices.cend()){
        expn.line_indices[0].push_back(2);
    }

    expn.tris[0] = {
        points[std::find(indices.cbegin(), indices.cend(), neig)-indices.cbegin()],
        vertex,
        points[std::find(indices.cbegin(), indices.cend(), i1)-indices.cbegin()]
    };

    // -----------------------
    std::cout<<" dbg: Adding second concav_expn section...\n";

    expn.indices[1].push_back(neig);
    expn.indices[1].push_back(desired_index);
    expn.indices[1].push_back(i2);

    expn.line_indices[1].push_back(1);
    // neig_index = std::find(indices.cbegin(), indices.cend(), neig) - indices.cbegin();
    int i2_index = std::find(indices.cbegin(), indices.cend(), i2) - indices.cbegin();
    small_index = neig_index < i2_index ? neig_index : i2_index;
    place = small_index + abs(neig_index-i2_index) - 1;
    if(std::find(line_indices.cbegin(), line_indices.cend(), place) != line_indices.cend()){
        expn.line_indices[1].push_back(2);
    }

    expn.tris[1] = {
        points[std::find(indices.cbegin(), indices.cend(), neig)-indices.cbegin()],
        vertex,
        points[std::find(indices.cbegin(), indices.cend(), i2)-indices.cbegin()]
    };

    std::cout<<" dbg: Done!\n";
    return expn;
}

std::pair<vertex_t, unsigned> make_concave(const triangle_t& triangle, const std::vector<int>& line_indices){
    std::pair<vertex_t, unsigned> pair;

    vertex_t vertex = generate_point({triangle.v[0], triangle.v[1], triangle.v[2]});
    unsigned index = 0; // TO DO

    pair.first = vertex;
    pair.second = index;

    return pair;
}

std::vector<vertex_t> concave_polygon(unsigned nvertices, unsigned nbasevertices){
    std::vector<vertex_t> vertices;
    std::vector<uvec> indices;  // to keep track of indices of all triangles
    std::vector<uvec> line_indices; // to keep track of actual line segments
    std::vector<triangle_t> triangles; // to keep track of all triangles
    std::vector<vertex_t> verts;

    vertices.push_back({0.0f, 0.0f});
    vertices.push_back({1.0f, 0.0f});
    vertices.push_back({0.5f, 1.0f});

    verts = vertices;

    uvec inds;
    inds.push_back(0);
    inds.push_back(1);
    inds.push_back(2);
    indices.push_back(inds);

    uvec linds;
    linds.push_back(0);
    linds.push_back(1);
    linds.push_back(2);
    line_indices.push_back(linds);

    triangle_t tri = {vertices[0], vertices[1], vertices[2]};
    triangles.push_back(tri);

    if(nvertices <= nbasevertices){
        std::cout<<"Base shape is not accepted!\n";
        assert(false);
    }

    unsigned n = nvertices - nbasevertices;
    vertex_t vertex;
    unsigned index;

    for(unsigned i=0;i<n;++i){
        std::cout<<"Adding a vertex...\n";

        triangle_t tri = triangles[0];
        uvec inds = indices[0];
        uvec linds = line_indices[0];

        std::cout<<"All triangles:\n";
        for(const auto& tri: triangles){
            std::cout<<tri<<'\n';
        }

        std::cout<<" | chosen triangle data: "<<tri<<'\n';
        std::cout<<" | chosen indices data: "<<inds[0]<<"-"<<inds[1]<<"-"<<inds[2]<<'\n';
        std::cout<<" | chosen line indices data: ";
        for(const auto& lind: linds){
            std::cout<<lind<<'-';
        }
        std::cout<<'\n';

        auto expn = _make_concave(tri, inds, linds, 3+i);

        std::cout<<" | obtained expn\n";
        std::cout<<" | obtained line index: "<<expn.line_index<<'\n';

        vertex = expn.vertex;
        std::cout<<" |> tri @ line index: "<<tri.v[expn.line_index]<<'\n';
        auto place1 = std::find(verts.begin(), verts.end(), tri.v[expn.line_index])-verts.cbegin();
        auto place2 = std::find(verts.begin(), verts.end(), tri.v[(expn.line_index+1)%3])-verts.cbegin();
        auto place = place1 > place2 ? place1 : place2;
        verts.insert(verts.cbegin()+place, vertex);
        vertices.push_back(vertex);

        triangles.erase(triangles.begin());
        triangles.insert(triangles.begin(), expn.tris[0]);
        triangles.insert(triangles.begin()+1, expn.tris[1]);

        indices.erase(indices.begin());
        indices.insert(indices.begin(), expn.indices[0]);
        indices.insert(indices.begin()+1, expn.indices[1]);

        line_indices.erase(line_indices.begin());
        line_indices.insert(line_indices.begin(), expn.line_indices[0]);
        line_indices.insert(line_indices.begin()+1, expn.line_indices[1]);

        std::cout<<" > Added vertex data: "<<vertex<<" @ index "<<place<<"\n";
        for(const auto& v: vertices){
            std::cout<<v<<", ";
        }
        std::cout<<"\n\n";

    }

    return verts;
}