#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-exc");
    BOOST_CHECK(GetBoolArg("-exc"));
    BOOST_CHECK(GetBoolArg("-exc", false));
    BOOST_CHECK(GetBoolArg("-exc", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-exco"));
    BOOST_CHECK(!GetBoolArg("-exco", false));
    BOOST_CHECK(GetBoolArg("-exco", true));

    ResetArgs("-exc=0");
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", false));
    BOOST_CHECK(!GetBoolArg("-exc", true));

    ResetArgs("-exc=1");
    BOOST_CHECK(GetBoolArg("-exc"));
    BOOST_CHECK(GetBoolArg("-exc", false));
    BOOST_CHECK(GetBoolArg("-exc", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noexc");
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", false));
    BOOST_CHECK(!GetBoolArg("-exc", true));

    ResetArgs("-noexc=1");
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", false));
    BOOST_CHECK(!GetBoolArg("-exc", true));

    ResetArgs("-exc -noexc");  // -exc should win
    BOOST_CHECK(GetBoolArg("-exc"));
    BOOST_CHECK(GetBoolArg("-exc", false));
    BOOST_CHECK(GetBoolArg("-exc", true));

    ResetArgs("-exc=1 -noexc=1");  // -exc should win
    BOOST_CHECK(GetBoolArg("-exc"));
    BOOST_CHECK(GetBoolArg("-exc", false));
    BOOST_CHECK(GetBoolArg("-exc", true));

    ResetArgs("-exc=0 -noexc=0");  // -exc should win
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", false));
    BOOST_CHECK(!GetBoolArg("-exc", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--exc=1");
    BOOST_CHECK(GetBoolArg("-exc"));
    BOOST_CHECK(GetBoolArg("-exc", false));
    BOOST_CHECK(GetBoolArg("-exc", true));

    ResetArgs("--noexc=1");
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", false));
    BOOST_CHECK(!GetBoolArg("-exc", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-exc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-exc", "eleven"), "eleven");

    ResetArgs("-exc -bar");
    BOOST_CHECK_EQUAL(GetArg("-exc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-exc", "eleven"), "");

    ResetArgs("-exc=");
    BOOST_CHECK_EQUAL(GetArg("-exc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-exc", "eleven"), "");

    ResetArgs("-exc=11");
    BOOST_CHECK_EQUAL(GetArg("-exc", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-exc", "eleven"), "11");

    ResetArgs("-exc=eleven");
    BOOST_CHECK_EQUAL(GetArg("-exc", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-exc", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-exc", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-exc", 0), 0);

    ResetArgs("-exc -bar");
    BOOST_CHECK_EQUAL(GetArg("-exc", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-exc=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-exc", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-exc=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-exc", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--exc");
    BOOST_CHECK_EQUAL(GetBoolArg("-exc"), true);

    ResetArgs("--exc=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-exc", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noexc");
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", true));
    BOOST_CHECK(!GetBoolArg("-exc", false));

    ResetArgs("-noexc=1");
    BOOST_CHECK(!GetBoolArg("-exc"));
    BOOST_CHECK(!GetBoolArg("-exc", true));
    BOOST_CHECK(!GetBoolArg("-exc", false));

    ResetArgs("-noexc=0");
    BOOST_CHECK(GetBoolArg("-exc"));
    BOOST_CHECK(GetBoolArg("-exc", true));
    BOOST_CHECK(GetBoolArg("-exc", false));

    ResetArgs("-exc --noexc");
    BOOST_CHECK(GetBoolArg("-exc"));

    ResetArgs("-noexc -exc"); // exc always wins:
    BOOST_CHECK(GetBoolArg("-exc"));
}

BOOST_AUTO_TEST_SUITE_END()
