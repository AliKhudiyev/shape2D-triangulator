#pragma once

#include "triangluator.hpp"

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
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp(){}
    void tearDown(){}

    protected:
    void testIsEqual();
    void testIsLessthan();
    void testIsGreaterthan();
};
