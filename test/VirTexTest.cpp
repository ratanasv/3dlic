#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "virtex.h"

class MockTex2DFactory : TextureAbstractFactory {

};

BOOST_AUTO_TEST_CASE(TestVirTex) {
	BOOST_REQUIRE_EQUAL(1, 1);
}


