#include "rs-tl/enum.hpp"
#include "rs-unit-test.hpp"
#include <string>
#include <type_traits>
#include <vector>

RS_DEFINE_ENUM(Etype, uint32_t, 1, alpha, bravo, charlie)
RS_DEFINE_ENUM_CLASS(Eclass, int64_t, -1, xray, yankee, zulu)

void test_rs_format_enum_macros() {

    std::vector<std::string> names;
    std::vector<Etype> etvec;
    std::vector<Eclass> ecvec;
    Etype et = {};
    Eclass ec = {};

    TEST(std::is_enum_v<Etype>);
    TEST(std::is_enum_v<Eclass>);

    TEST_TYPE(std::underlying_type_t<Etype>, uint32_t);
    TEST_TYPE(std::underlying_type_t<Eclass>, int64_t);

    TEST_EQUAL(uint32_t(alpha),          1u);
    TEST_EQUAL(uint32_t(bravo),          2u);
    TEST_EQUAL(uint32_t(charlie),        3u);
    TEST_EQUAL(int64_t(Eclass::xray),    -1);
    TEST_EQUAL(int64_t(Eclass::yankee),  0);
    TEST_EQUAL(int64_t(Eclass::zulu),    1);

    TRY(names = list_enum_names(Etype()));
    TEST_EQUAL(RS::UnitTest::format_range(names), "[alpha,bravo,charlie]");
    TRY(etvec = list_enum_values(Etype()));
    TEST_EQUAL(etvec.size(), 3u);
    REQUIRE(etvec.size() == 3);
    TEST_EQUAL(etvec[0], alpha);
    TEST_EQUAL(etvec[1], bravo);
    TEST_EQUAL(etvec[2], charlie);

    TRY(names = list_enum_names(Eclass()));
    TEST_EQUAL(RS::UnitTest::format_range(names), "[xray,yankee,zulu]");
    TRY(ecvec = list_enum_values(Eclass()));
    TEST_EQUAL(ecvec.size(), 3u);
    REQUIRE(ecvec.size() == 3);
    TEST_EQUAL(ecvec[0], Eclass::xray);
    TEST_EQUAL(ecvec[1], Eclass::yankee);
    TEST_EQUAL(ecvec[2], Eclass::zulu);

    TEST_EQUAL(to_string(alpha),           "alpha");
    TEST_EQUAL(to_string(bravo),           "bravo");
    TEST_EQUAL(to_string(charlie),         "charlie");
    TEST_EQUAL(to_string(Etype(0)),        "0");
    TEST_EQUAL(to_string(Eclass::xray),    "xray");
    TEST_EQUAL(to_string(Eclass::yankee),  "yankee");
    TEST_EQUAL(to_string(Eclass::zulu),    "zulu");
    TEST_EQUAL(to_string(Eclass(-2)),      "-2");

    TEST(parse_enum("alpha", et));    TEST_EQUAL(et, alpha);
    TEST(parse_enum("bravo", et));    TEST_EQUAL(et, bravo);
    TEST(parse_enum("charlie", et));  TEST_EQUAL(et, charlie);
    TEST(parse_enum("xray", ec));     TEST_EQUAL(ec, Eclass::xray);
    TEST(parse_enum("yankee", ec));   TEST_EQUAL(ec, Eclass::yankee);
    TEST(parse_enum("zulu", ec));     TEST_EQUAL(ec, Eclass::zulu);

    TEST(! parse_enum("xray", et));
    TEST(! parse_enum("alpha", ec));

}
