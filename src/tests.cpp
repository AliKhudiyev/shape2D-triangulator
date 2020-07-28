#include "tests.hpp"

void TestTriangulator::setUp(){
    ;
}

void TestTriangulator::tearDown(){
    ;
}

void TestTriangulator::testIsEqual(){
    CPPUNIT_ASSERT(is_equal(0.0, -1e-6) == true);
    CPPUNIT_ASSERT(is_equal(0.0, 1e-6) == true);
    CPPUNIT_ASSERT(is_equal(0.0, -epsilon) == false);
    CPPUNIT_ASSERT(is_equal(0.0, epsilon) == false);
}

void TestTriangulator::testIsLessthan(){
    CPPUNIT_ASSERT(is_lessthan(0.0, -1e-6) == false);
    CPPUNIT_ASSERT(is_lessthan(0.0, 1e-6) == false);
    CPPUNIT_ASSERT(is_lessthan(0.0, -epsilon) == false);
    CPPUNIT_ASSERT(is_lessthan(0.0, epsilon) == true);
}

void TestTriangulator::testIsGreaterthan(){
    CPPUNIT_ASSERT(is_greaterthan(0.0, -1e-6) == false);
    CPPUNIT_ASSERT(is_greaterthan(0.0, 1e-6) == false);
    CPPUNIT_ASSERT(is_greaterthan(0.0, -epsilon) == true);
    CPPUNIT_ASSERT(is_greaterthan(0.0, epsilon) == false);
}

void TestTriangulator::testTellCoefs(){
    ;
}

void TestTriangulator::testTellFunctype(){
    ;
}

void TestTriangulator::testTellY(){
    ;
}

void TestTriangulator::testTellX(){
    ;
}

void TestTriangulator::testIsInside(){
    Vertex p1 = {0.5f, 0.5f}, p2 = {0.5f, 0.0f}, p3 = {1.0f, 0.1f}, p4 = {0.0f, 0.0f};
    Triangle triangle;

    triangle.v[0] = {0.0f, 0.0f};
    triangle.v[1] = {1.0f, 0.0f};
    triangle.v[2] = {0.5f, 1.0f};

    CPPUNIT_ASSERT(is_inside(p1, triangle) == true);
    CPPUNIT_ASSERT(is_inside(p2, triangle) == true);
    CPPUNIT_ASSERT(is_inside(p3, triangle) == false);
    CPPUNIT_ASSERT(is_inside(p4, triangle) == true);
}

void TestTriangulator::testTellSide(){
    Line lines[3];
    Vertex points[3];

    lines[0] = {Vertex{0.0f, 0.0f}, Vertex{0.0f, 1.0f}};
    lines[1] = {Vertex{0.0f, 0.0f}, Vertex{1.0f, 0.0f}};
    lines[2] = {Vertex{0.0f, 0.0f}, Vertex{1.0f, 1.0f}};

    points[0] = {0.0f, 0.0f};
    points[1] = {1.0f, 1.0f};
    points[2] = {-1.0f, -1.0f};

    CPPUNIT_ASSERT(tell_side(points[0], lines[0]) == Side::MIDDLE);
    CPPUNIT_ASSERT(tell_side(points[1], lines[0]) == Side::SECOND);
    CPPUNIT_ASSERT(tell_side(points[2], lines[0]) == Side::FIRST);

    CPPUNIT_ASSERT(tell_side(points[0], lines[1]) == Side::MIDDLE);
    CPPUNIT_ASSERT(tell_side(points[1], lines[1]) == Side::SECOND);
    CPPUNIT_ASSERT(tell_side(points[2], lines[1]) == Side::FIRST);

    CPPUNIT_ASSERT(tell_side(points[0], lines[2]) == Side::MIDDLE);
    CPPUNIT_ASSERT(tell_side(points[1], lines[2]) == Side::MIDDLE);
    CPPUNIT_ASSERT(tell_side(points[2], lines[2]) == Side::MIDDLE);
}

void TestTriangulator::testTellBinarySide(){
    ;
}

void TestTriangulator::testTellShape(){
    ;
}

void TestTriangulator::testTellIntersection(){
    ;
}

void TestTriangulator::testIntersects(){
    ;
}

void TestTriangulator::testIsSeperable(){
    ;
}

void TestTriangulator::testGenerateNumber(){
    float n1, n2, n3;

    n1 = generate_number();
    n2 = generate_number(-2.0f, 3.0f);
    n3 = generate_number(3.0f, -2.0f);

    CPPUNIT_ASSERT(0.0f <= n1 && n1 <= 1.0f);
    CPPUNIT_ASSERT(-2.0f <= n2 && n2 <= 3.0f);
    CPPUNIT_ASSERT(-2.0f <= n3 && n3 <= 3.0f);
}

void TestTriangulator::testGeneratePoint(){
    Vertex p1, p2, p3;

    p1 = generate_point();
    p2 = generate_point(-1.0f, -1.0f, 1.0f, 1.0f);
    p3 = generate_point(1.0f, -1.0f, -1.0f, 1.0f);

    CPPUNIT_ASSERT(
        0.0f <= p1.data[0] && p1.data[0] <= 1.0f &&
        0.0f <= p1.data[1] && p1.data[1] <= 1.0f
    );
    CPPUNIT_ASSERT(
        -1.0f <= p2.data[0] && p2.data[0] <= 1.0f &&
        -1.0f <= p2.data[1] && p2.data[1] <= 1.0f
    );
    CPPUNIT_ASSERT(
        -1.0f <= p3.data[0] && p3.data[0] <= 1.0f &&
        -1.0f <= p3.data[1] && p3.data[1] <= 1.0f
    );

    // 2nd part

    std::vector<Vertex> triangle(3), quad(4);
    Triangle tri[3];

    triangle[0] = {0.0f, 0.0f};
    triangle[1] = {1.0f, 0.0f};
    triangle[2] = {0.5f, 1.0f};

    quad[0] = {0.0f, 0.0f};
    quad[0] = {1.0f, 0.0f};
    quad[0] = {1.0f, 1.0f};
    quad[0] = {0.0f, 1.0f};

    tri[0] = {triangle[0], triangle[1], triangle[2]};
    tri[1] = {quad[0], quad[1], quad[2]};
    tri[2] = {quad[1], quad[2], quad[3]};

    p1 = generate_point(triangle);
    p2 = generate_point(quad);
    p3 = generate_point(quad);

    CPPUNIT_ASSERT(
        is_inside(p1, tri[0]) == true
    );
    CPPUNIT_ASSERT(
        is_inside(p2, tri[1]) == true ||
        is_inside(p2, tri[2]) == true
    );
    CPPUNIT_ASSERT(
        is_inside(p3, tri[1]) == true ||
        is_inside(p3, tri[2]) == true
    );
}

void TestTriangulator::testGenerateConvex(){
    for(unsigned i=0;i<100;++i){
        std::vector<Vertex> vertices = generate_convex(20);
        CPPUNIT_ASSERT(tell_shape(vertices) == ShapeType::CONVEX);
    }
}