#include "triangulator.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <cassert>

#define CONVEX_POLY     0
#define CONCAVE_POLY    1

using namespace std;

void import(istream& in, vector<Vertex>& vertices, const char delim = ',');

// 
int main(int argc, char* const* argv){
    srand(time(0));
    const string usage = 
        "./main -[g] [cx/cv] [nvertices]\n"
        "Check README for more detailed information!\n"
    ;

    vector<vertex_t> vertices;
    vector<triangle_t> triangles;
    vector<unsigned> indices;

    bool tflag = true;
    bool nflag = false;
    bool pflag = false;
    string filepath, opt2;
    int shapetype;
    unsigned nvertices = 0;

    opterr = 0;
    int c;

    while ((c = getopt (argc, argv, "g:n:p:")) != -1)
        switch (c)
        {
        case 'g':
            tflag = false;
            opt2 = string(optarg);
            if(opt2=="cx") shapetype = CONVEX_POLY;
            else if(opt2=="cv") shapetype = CONCAVE_POLY;
            else{
                cerr<<"No such option "<<opt2<<"!\n";
                abort();
            }
            // cout<<"g flag: "<<opt2<<"\n";
            break;
        case 'n':
            nflag = true;
            nvertices = (unsigned)strtol(optarg, nullptr, 10);
            // cout<<"n flag: "<<nvertices<<"\n";
            break;
        case 'p':
            pflag = true;
            filepath = string(optarg);
            // cout<<"p flag: "<<filepath<<"\n";
            break;
        case '?':
            if (optopt == 'c')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
            fprintf (stderr,
                    "Unknown option character `\\x%x'.\n",
                    optopt);
            return 1;
        default:
            abort ();
        }

    if(tflag){
        if(pflag){
            // const string prefix = "../examples/";
            // std::cout<<"Importing with the prefix: "<<prefix;
            ifstream file(filepath);
            if(!file){
                std::cerr<<"ERROR[import]: file cannot be opened!\n";
                assert(false);
            }
            import(file, vertices);
            file.close();
        } else{
            import(cin, vertices, ' ');
        }
        triangulate(vertices, triangles, indices);
    } else{
        if(nflag){
            if(shapetype==CONVEX_POLY){
                vertices = convex_polygon(nvertices);
            } else if(shapetype==CONCAVE_POLY){
                vertices = generate_concave(nvertices);
            }
        } else{
            cerr<<"Need to provide number of vertices!\n";
            abort();
        }
    }

    for(const auto& v: vertices){
        v.print();
        cout<<'\n';
    }
    if(tflag){
        cout<<"\nIndices: ";
        for(const auto& index: indices){
            cout<<' '<<index;
        }
        cout<<endl;
    }

    //triangulate(vertices, triangles, indices);

    // for(const auto& triangle: triangles){
    //     cout<<triangle.v[0].data[0]<<", "<<triangle.v[0].data[1]<<"\n";
    //     cout<<triangle.v[1].data[0]<<", "<<triangle.v[1].data[1]<<"\n";
    //     cout<<triangle.v[2].data[0]<<", "<<triangle.v[2].data[1]<<"\n\n";
    // }

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

    string type;
    type = tell_shape(vertices) == ShapeType::CONVEX? "convex" : "concave";
    
    cout<<"Type: "<<type<<'\n';

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