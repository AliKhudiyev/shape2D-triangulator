#include "triangluator.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void import(istream& in, vector<Vertex>& vertices, const char delim = ',');

int main(int argc, const char** argv){
    srand(time(0));

    vector<vertex_t> vertices;
    /*
    if(argc == 1){
        import(cin, vertices, ' ');
    } else if(argc == 2){
        string file_prefix = "../examples/";
        string file_name(argv[1]);
        string filepath = file_prefix + file_name;
        ifstream file(filepath);
        
        if(!file){
            cerr<<"ERROR[import]: File cannot be imported!\n";
            assert(false);
        }
        import(file, vertices);
        file.close();
    }
    */

    vector<triangle_t> triangles;
    vector<unsigned> indices;

    //triangulate(vertices, triangles, indices);

    // for(const auto& triangle: triangles){
    //     cout<<triangle.v[0].data[0]<<", "<<triangle.v[0].data[1]<<"\n";
    //     cout<<triangle.v[1].data[0]<<", "<<triangle.v[1].data[1]<<"\n";
    //     cout<<triangle.v[2].data[0]<<", "<<triangle.v[2].data[1]<<"\n\n";
    // }

    for(const auto& index: indices){
        cout<<' '<<index;
    }   cout<<endl;

    if(argc == 1){
        std::cerr<<"Are you dumb, STOOPID or dumb?!\n";
        // assert(false);
    }
    // vertices = generate_convex(strtol(argv[1], nullptr, 10));
    vertices = concave_polygon(strtol(argv[1], nullptr, 10), 3);
    // vertices = convex_polygon(strtol(argv[1], nullptr, 10));

    // vertices.clear(); // concave rectangle example
    // vertices.push_back({0.664083, 0.242819});
    // vertices.push_back({0.0521657, 0.748719});
    // vertices.push_back({0.718302, 0.505896});
    // vertices.push_back({0.713009, 0.238704});
    // vertices.push_back({0.671389, 0.241933});
    // vertices.push_back({0.665037, 0.242635});
    // vertices.push_back({0.664334, 0.242762});

    // vertices.push_back({0.872089, 0.194687});
    // vertices.push_back({0.106305, 0.662687});
    // vertices.push_back({0.782236, 0.0376807});
    // vertices.push_back({0.874295, 0.187868});
    // vertices.push_back({0.872147, 0.194566});

    // vertices.push_back({1.22800124, 3.01726794});
    // vertices.push_back({3.22383475, 2.98937345});
    // vertices.push_back({5.03408527, 3.87499189});
    // vertices.push_back({5.03785324, 5.03693295});
    // vertices.push_back({3.13797617, 5.58400536});
    // solved

    // cout<<"epsilon: "<<epsilon<<"\n\n";
    
    for(const auto& v: vertices){
        v.print();
        cout<<'\n';
    }

    string type;
    type = tell_shape(vertices) == ShapeType::CONVEX? "convex" : "concave";
    
    // cout<<tell_shape(vertices);
    cout<<"Type: "<<type<<'\n';

    // Triangle triangle;
    // triangle.v[0] = {0.0f, 0.0f};
    // triangle.v[1] = {1.0f, 0.0f};
    // triangle.v[2] = {0.5f, 1.0f};

    // vertices.clear();
    // vertices.push_back(triangle.v[0]);
    // vertices.push_back(triangle.v[1]);
    // vertices.push_back(triangle.v[2]);

    // auto point = generate_point(vertices);
    // cout<<is_inside(point, triangle)<<'\n';;
    // cout<<tell_shape(vertices)<<'\n';

    return 0;
}

void import(istream& in, vector<Vertex>& vertices, const char delim){
    string input, pos;
    stringstream stream;

    while(getline(in, input)){
        if(input[0] == 'q') break;
        stream << input;

        getline(stream, pos, delim);
        float x = strtof(pos.c_str(), nullptr);
        
        getline(stream, pos, delim);
        float y = strtof(pos.c_str(), nullptr);
        
        auto vertex = Vertex{x, y};
        vertices.push_back(vertex);
    
        stream.clear();
    }
}