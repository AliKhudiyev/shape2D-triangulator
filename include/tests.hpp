#pragma once

#include "triangulator.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

class TestTriangulator: public CppUnit::TestFixture{
    CPPUNIT_TEST_SUITE(TestTriangulator);
    CPPUNIT_TEST(testIsEqual);
    CPPUNIT_TEST(testIsLessthan);
    CPPUNIT_TEST(testIsGreaterthan);
    
    CPPUNIT_TEST(testTellCoefs);
    CPPUNIT_TEST(testTellFunctype);
    CPPUNIT_TEST(testTellY);
    CPPUNIT_TEST(testTellX);

    CPPUNIT_TEST(testIsInside);
    CPPUNIT_TEST(testTellSide);
    CPPUNIT_TEST(testTellBinarySide);
    CPPUNIT_TEST(testTellShape);
    CPPUNIT_TEST(testTellIntersection);
    CPPUNIT_TEST(testIntersects);

    CPPUNIT_TEST(testIsSeperable);

    CPPUNIT_TEST(testGenerateNumber);
    CPPUNIT_TEST(testGeneratePoint);
    CPPUNIT_TEST(testGenerateConvex);
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp();
    void tearDown();

    protected:
    void testIsEqual();
    void testIsLessthan();
    void testIsGreaterthan();

    void testTellCoefs();
    void testTellFunctype();
    void testTellY();
    void testTellX();

    void testIsInside();
    void testTellSide();
    void testTellBinarySide();
    void testTellShape();
    void testTellIntersection();
    void testIntersects();

    void testIsSeperable();
    void testTriangulate();

    void testGenerateNumber();
    void testGeneratePoint();
    void testGenerateConvex();

    private:
    std::vector<Vertex> m_vertices;
    std::vector<Line> m_lines;
    std::vector<Triangle> m_triangles;
};
