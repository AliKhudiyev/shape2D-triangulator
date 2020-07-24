#include "triangluator.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void import(istream& in, vector<Vertex>& vertices, const char delim = ',');

int main(int argc, const char** argv){

    vector<vertex_t> vertices;

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

    vector<triangle_t> triangles;
    vector<unsigned> indices;

    triangulate(vertices, triangles, indices);

    // for(const auto& triangle: triangles){
    //     cout<<triangle.v[0].data[0]<<", "<<triangle.v[0].data[1]<<"\n";
    //     cout<<triangle.v[1].data[0]<<", "<<triangle.v[1].data[1]<<"\n";
    //     cout<<triangle.v[2].data[0]<<", "<<triangle.v[2].data[1]<<"\n\n";
    // }

    for(const auto& index: indices){
        cout<<' '<<index;
    }   cout<<endl;

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