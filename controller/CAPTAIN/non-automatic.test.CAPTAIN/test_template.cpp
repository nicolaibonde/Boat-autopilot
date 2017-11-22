#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_template)

BOOST_AUTO_TEST_CASE(Test1)
{
	BOOST_CHECK(1<2);
}

BOOST_AUTO_TEST_SUITE_END()