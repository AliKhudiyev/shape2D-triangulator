#include "triangluator.hpp"
#include "tests.hpp"

#include <iostream>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION( TestTriangulator );

int main(){

    vector<vertex_t> vertices{
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.5, 1.5},
        {0.0, 1.0}
    };

    vector<vertex_t> vertices1{
        {0.0, 0.0},
        {1.0, 0.5},
        {0.0, 1.0},
        {0.5, 0.5}
    };

    vector<vertex_t> vertices2{
        {0.0, 0.0},
        {0.5, 0.5},
        {0.0, 1.0},
        {1.0, 0.5},
        {0.75, 0.0},
        {1.0, -0.5},
        {0.0, -1.0},
        {0.5, -0.5}
    };

    vector<triangle_t> triangles;
    vector<unsigned> indices;

    Line l1{Vertex{0.0, 0.0}, Vertex{1.0, 0.0}};
    Line l2{Vertex{0.1, 0.1}, Vertex{1.0, 1.0}};
    Line l3{Vertex{0.0, 0.0}, Vertex{0.0, 1.0}};
    Line l4{Vertex{0.5, 0.5}, Vertex{1.0, 0.5}};

    coef_t coefs1 = tell_coefs(l1), coefs2 = tell_coefs(l2), coefs3 = tell_coefs(l3);
    
    // cout<<coefs1.data[0]<<" "<<coefs1.data[1]<<" |> "<<coefs1.data[2]<<'\n';
    // cout<<coefs2.data[0]<<" "<<coefs2.data[1]<<" |> "<<coefs2.data[2]<<'\n';
    // cout<<coefs3.data[0]<<" "<<coefs3.data[1]<<" |> "<<coefs3.data[2]<<'\n';

    //cout<<intersects(l1, l2)<<'\n';
    //cout<<"side: "<<(int)tell_side(l3, l3)<<'\n';
    //cout<<"shape: "<<(int)tell_shape(l4, vertices1)<<'\n';
    //auto side1 = tell_side(Vertex{0.3, 0.3}, l2);

    //cout<<"\n";
    // auto side1 = tell_side(l1, l1);
    // if(side1==Side::FIRST){
    //     cout<<"f\n";
    // } else if(side1==Side::SECOND){
    //     cout<<"s\n";
    // } else{
    //     cout<<"m\n";
    // }
    //cout<<(int)tell_side(l2, l1);

    //cout<<tell_shape(vertices)<<'\n';
    //cout<<tell_shape(vertices1)<<'\n';

    triangulate(vertices2, triangles, indices);

    // for(const auto& triangle: triangles){
    //     cout<<triangle.v[0].data[0]<<", "<<triangle.v[0].data[1]<<"\n";
    //     cout<<triangle.v[1].data[0]<<", "<<triangle.v[1].data[1]<<"\n";
    //     cout<<triangle.v[2].data[0]<<", "<<triangle.v[2].data[1]<<"\n\n";
    // }

    for(const auto& index: indices){
        cout<<' '<<index;
    }   cout<<endl;

    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("cppTestBasicMathResults.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}