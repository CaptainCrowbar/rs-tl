#include "rs-tl/types.hpp"
#include "rs-unit-test.hpp"
#include <string>
#include <vector>

using namespace RS::TL;

void test_rs_tl_types_traits() {

    TEST(is_range<std::string>);
    TEST(is_range<std::vector<int>>);
    TEST(is_range<std::vector<std::string>>);
    TEST(! is_range<int>);
    TEST(! is_range<void>);

    TEST_TYPE(RangeIterator<std::string>,                     std::string::iterator);
    TEST_TYPE(RangeIterator<const std::string>,               std::string::const_iterator);
    TEST_TYPE(RangeValue<std::string>,                        char);
    TEST_TYPE(RangeValue<const std::string>,                  char);
    TEST_TYPE(RangeIterator<std::vector<int>>,                std::vector<int>::iterator);
    TEST_TYPE(RangeIterator<const std::vector<int>>,          std::vector<int>::const_iterator);
    TEST_TYPE(RangeValue<std::vector<int>>,                   int);
    TEST_TYPE(RangeValue<const std::vector<int>>,             int);
    TEST_TYPE(RangeIterator<std::vector<std::string>>,        std::vector<std::string>::iterator);
    TEST_TYPE(RangeIterator<const std::vector<std::string>>,  std::vector<std::string>::const_iterator);
    TEST_TYPE(RangeValue<std::vector<std::string>>,           std::string);
    TEST_TYPE(RangeValue<const std::vector<std::string>>,     std::string);
    TEST_TYPE(RangeIterator<int>,                             void);
    TEST_TYPE(RangeValue<int>,                                void);
    TEST_TYPE(RangeIterator<void>,                            void);
    TEST_TYPE(RangeValue<void>,                               void);

}
