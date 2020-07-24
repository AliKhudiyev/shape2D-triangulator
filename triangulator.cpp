#include "triangluater.hpp"

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


Side is_inside(const vertex_t& v, const triangle_t& triangle);

Side tell_side(const vertex_t& v, const line_t& line){
    auto coefs = tell_coefs(line);
    auto type = tell_functype(coefs);

    //std::cout<<coefs.data[0]<<" - "<<coefs.data[1]<<" |> "<<coefs.data[2]<<'\n';
    //std::cout<<"calc: "<<calc(v.data[0], coefs)<<'\n';
    //std::cout<<"vdata: "<<v.data[0]<<" and "<<v.data[1]<<'\n';
    if(type == FUNC_REGULAR){
        if(is_lessthan(v.data[1], calc(v.data[0], coefs))) return Side::FIRST;
        else if(is_equal(v.data[1], calc(v.data[0], coefs))) return Side::MIDDLE;
        return Side::SECOND;
    } else{
        if(is_lessthan(v.data[0], coefs.data[0])) return Side::FIRST;
        else if(is_equal(v.data[0], coefs.data[0])) return Side::MIDDLE;
        return Side::SECOND;
    }
}

Side tell_side(const line_t& l, const line_t& line){
    // TO DO
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
        auto side = tell_side(lines[i], lines[(i+1)%nlines]);
        for(unsigned j=i+1;j<nlines;++j){
            if(side == Side::MIDDLE){
                side = tell_side(lines[i], lines[(i+1)%nlines]);
                continue;
            }
            if(side != tell_side(lines[i], lines[(i+1)%nlines]))
                return ShapeType::CONCAVE;
        }
    }
    return ShapeType::CONVEX;
}

ShapeType tell_shape(const line_t& line, const std::vector<vertex_t>& vertices){
    auto side = tell_side(vertices[0], line);
    unsigned count = vertices.size();
    for(unsigned i=0;i<count;++i){
        //std::cout<<" > "<<(int)side<<' ';
        auto v = vertices[(i+1)%count];
        if(side == Side::MIDDLE){
            side = tell_side(v, line);
            continue;
        }
        else if(tell_side(v, line) != Side::MIDDLE && side != tell_side(v, line)){
            //std::cout<<'\n';
            return ShapeType::CONCAVE;
        }
    }
    //std::cout<<'\n';
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