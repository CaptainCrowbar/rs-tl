#include "rs-tl/fixed-binary.hpp"
#include "rs-unit-test.hpp"
#include <limits>
#include <unordered_set>

using namespace RS::TL;

namespace {

    const auto min_int32 = std::numeric_limits<int32_t>::min();
    const auto max_int32 = std::numeric_limits<int32_t>::max();
    const auto min_int64 = std::numeric_limits<int64_t>::min();
    const auto max_int64 = std::numeric_limits<int64_t>::max();

}

void test_rs_tl_fixed_binary_implementation_selection() {

    TEST_TYPE(Binary<1>, SmallBinary<1>);
    TEST_TYPE(Binary<5>, SmallBinary<5>);
    TEST_TYPE(Binary<10>, SmallBinary<10>);
    TEST_TYPE(Binary<20>, SmallBinary<20>);
    TEST_TYPE(Binary<30>, SmallBinary<30>);
    TEST_TYPE(Binary<40>, SmallBinary<40>);
    TEST_TYPE(Binary<50>, SmallBinary<50>);
    TEST_TYPE(Binary<60>, SmallBinary<60>);
    TEST_TYPE(Binary<64>, SmallBinary<64>);
    TEST_TYPE(Binary<65>, LargeBinary<65>);
    TEST_TYPE(Binary<70>, LargeBinary<70>);
    TEST_TYPE(Binary<80>, LargeBinary<80>);
    TEST_TYPE(Binary<90>, LargeBinary<90>);
    TEST_TYPE(Binary<100>, LargeBinary<100>);
    TEST_TYPE(Binary<200>, LargeBinary<200>);
    TEST_TYPE(Binary<500>, LargeBinary<500>);
    TEST_TYPE(Binary<1000>, LargeBinary<1000>);

}

void test_rs_tl_fixed_binary_small_binary_5() {

    using binary = SmallBinary<5>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(int(binary::max()), 31);

    TEST(c.fits_in<int>());  TEST_EQUAL(int(c), 10);  TEST(c);
    TEST(x.fits_in<int>());  TEST_EQUAL(int(x), 0);   TEST(! x);

    TRY(x = binary(25));
    TRY(y = binary(10));

    TEST_EQUAL(int(x), 25);
    TEST_EQUAL(int(y), 10);
    TEST_EQUAL(int(z), 0);

    TEST_EQUAL(x.bin(), "11001");
    TEST_EQUAL(y.bin(), "01010");
    TEST_EQUAL(z.bin(), "00000");
    TEST_EQUAL(x.dec(), "25");
    TEST_EQUAL(y.dec(), "10");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "19");
    TEST_EQUAL(y.hex(), "0a");
    TEST_EQUAL(z.hex(), "00");
    TEST_EQUAL(double(x), 25.0);
    TEST_EQUAL(double(y), 10.0);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 5u);
    TEST_EQUAL(y.significant_bits(), 4u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(int(z), 0);
    TRY(z = - x);  TEST_EQUAL(int(z), 7);
    TRY(z = - y);  TEST_EQUAL(int(z), 22);

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(int(z), 31);
    TRY(z = ~ x);  TEST_EQUAL(int(z), 6);
    TRY(z = ~ y);  TEST_EQUAL(int(z), 21);

    TRY(z = x + y);  TEST_EQUAL(int(z), 3);
    TRY(z = x - y);  TEST_EQUAL(int(z), 15);
    TRY(z = y - x);  TEST_EQUAL(int(z), 17);
    TRY(z = x * y);  TEST_EQUAL(int(z), 26);
    TRY(z = x / y);  TEST_EQUAL(int(z), 2);
    TRY(z = x % y);  TEST_EQUAL(int(z), 5);
    TRY(z = y / x);  TEST_EQUAL(int(z), 0);
    TRY(z = y % x);  TEST_EQUAL(int(z), 10);
    TRY(z = x & y);  TEST_EQUAL(int(z), 8);
    TRY(z = x | y);  TEST_EQUAL(int(z), 27);
    TRY(z = x ^ y);  TEST_EQUAL(int(z), 19);

    TRY(z = x << 0);   TEST_EQUAL(int(z), 25);  TRY(z = x >> 0);   TEST_EQUAL(int(z), 25);
    TRY(z = x << 1);   TEST_EQUAL(int(z), 18);  TRY(z = x >> 1);   TEST_EQUAL(int(z), 12);
    TRY(z = x << 2);   TEST_EQUAL(int(z), 4);   TRY(z = x >> 2);   TEST_EQUAL(int(z), 6);
    TRY(z = x << 3);   TEST_EQUAL(int(z), 8);   TRY(z = x >> 3);   TEST_EQUAL(int(z), 3);
    TRY(z = x << 4);   TEST_EQUAL(int(z), 16);  TRY(z = x >> 4);   TEST_EQUAL(int(z), 1);
    TRY(z = x << 5);   TEST_EQUAL(int(z), 0);   TRY(z = x >> 5);   TEST_EQUAL(int(z), 0);

    TEST(++x);    TEST_EQUAL(int(x), 26);
    TEST(--x);    TEST_EQUAL(int(x), 25);
    TEST(x--);    TEST_EQUAL(int(x), 24);
    TEST(x++);    TEST_EQUAL(int(x), 25);
    TEST(++y);    TEST_EQUAL(int(y), 11);
    TEST(--y);    TEST_EQUAL(int(y), 10);
    TEST(y--);    TEST_EQUAL(int(y), 9);
    TEST(y++);    TEST_EQUAL(int(y), 10);
    TEST(++z);    TEST_EQUAL(int(z), 1);
    TEST(! --z);  TEST_EQUAL(int(z), 0);
    TEST(! z--);  TEST_EQUAL(int(z), 31);
    TEST(z++);    TEST_EQUAL(int(z), 0);

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TRY(x = rotl(y, 0));   TEST_EQUAL(int(x), 10);
    TRY(x = rotl(y, 1));   TEST_EQUAL(int(x), 20);
    TRY(x = rotl(y, 2));   TEST_EQUAL(int(x), 9);
    TRY(x = rotl(y, 3));   TEST_EQUAL(int(x), 18);
    TRY(x = rotl(y, 4));   TEST_EQUAL(int(x), 5);
    TRY(x = rotl(y, 5));   TEST_EQUAL(int(x), 10);
    TRY(x = rotl(y, 6));   TEST_EQUAL(int(x), 20);
    TRY(x = rotl(y, 7));   TEST_EQUAL(int(x), 9);
    TRY(x = rotl(y, 8));   TEST_EQUAL(int(x), 18);
    TRY(x = rotl(y, 9));   TEST_EQUAL(int(x), 5);
    TRY(x = rotl(y, 10));  TEST_EQUAL(int(x), 10);

    TRY(x = rotr(y, 0));   TEST_EQUAL(int(x), 10);
    TRY(x = rotr(y, 1));   TEST_EQUAL(int(x), 5);
    TRY(x = rotr(y, 2));   TEST_EQUAL(int(x), 18);
    TRY(x = rotr(y, 3));   TEST_EQUAL(int(x), 9);
    TRY(x = rotr(y, 4));   TEST_EQUAL(int(x), 20);
    TRY(x = rotr(y, 5));   TEST_EQUAL(int(x), 10);
    TRY(x = rotr(y, 6));   TEST_EQUAL(int(x), 5);
    TRY(x = rotr(y, 7));   TEST_EQUAL(int(x), 18);
    TRY(x = rotr(y, 8));   TEST_EQUAL(int(x), 9);
    TRY(x = rotr(y, 9));   TEST_EQUAL(int(x), 20);
    TRY(x = rotr(y, 10));  TEST_EQUAL(int(x), 10);

    TRY(x = binary::from_double(25.0));
    TEST_EQUAL(x.dec(), "25");
    TEST_EQUAL(double(x), 25.0);
    TEST_EQUAL(int(x), 25);

}

void test_rs_tl_fixed_binary_small_binary_35() {

    using binary = SmallBinary<35>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(uint64_t(binary::max()), 0x7ffffffffull);
    TEST_EQUAL(binary::max().hex(), "7ffffffff");

    TEST(c.fits_in<int64_t>());  TEST_EQUAL(int64_t(c), 42);  TEST(c);
    TEST(x.fits_in<int64_t>());  TEST_EQUAL(int64_t(x), 0);   TEST(! x);

    TRY(x = binary(0x123456789ull));
    TRY(y = binary(0xabcdefull));

    TEST_EQUAL(int64_t(x), 4886718345);
    TEST_EQUAL(int64_t(y), 11259375);
    TEST_EQUAL(int64_t(z), 0);

    TEST_EQUAL(x.bin(), "00100100011010001010110011110001001");
    TEST_EQUAL(y.bin(), "00000000000101010111100110111101111");
    TEST_EQUAL(z.bin(), "00000000000000000000000000000000000");
    TEST_EQUAL(x.dec(), "4886718345");
    TEST_EQUAL(y.dec(), "11259375");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "123456789");
    TEST_EQUAL(y.hex(), "000abcdef");
    TEST_EQUAL(z.hex(), "000000000");
    TEST_EQUAL(double(x), 4886718345.0);
    TEST_EQUAL(double(y), 11259375.0);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 33u);
    TEST_EQUAL(y.significant_bits(), 24u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(uint64_t(z), 0u);
    TRY(z = - x);  TEST_EQUAL(uint64_t(z), 0x6dcba9877ull);
    TRY(z = - y);  TEST_EQUAL(uint64_t(z), 0x7ff543211ull);

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(uint64_t(z), 0x7ffffffffull);
    TRY(z = ~ x);  TEST_EQUAL(uint64_t(z), 0x6dcba9876ull);
    TRY(z = ~ y);  TEST_EQUAL(uint64_t(z), 0x7ff543210ull);

    TRY(z = x + y);  TEST_EQUAL(uint64_t(z), 0x123f13578ull);
    TRY(z = x - y);  TEST_EQUAL(uint64_t(z), 0x12299999aull);
    TRY(z = y - x);  TEST_EQUAL(uint64_t(z), 0x6dd666666ull);
    TRY(z = x * y);  TEST_EQUAL(uint64_t(z), 0x2aa375de7ull);
    TRY(z = x / y);  TEST_EQUAL(uint64_t(z), 0x0000001b2ull);
    TRY(z = x % y);  TEST_EQUAL(uint64_t(z), 0x00002485bull);
    TRY(z = y / x);  TEST_EQUAL(uint64_t(z), 0u);
    TRY(z = y % x);  TEST_EQUAL(uint64_t(z), 0x000abcdefull);
    TRY(z = x & y);  TEST_EQUAL(uint64_t(z), 0x000014589ull);
    TRY(z = x | y);  TEST_EQUAL(uint64_t(z), 0x123efefefull);
    TRY(z = x ^ y);  TEST_EQUAL(uint64_t(z), 0x123eeaa66ull);

    TRY(z = x << 0);   TEST_EQUAL(uint64_t(z), 0x123456789ull);  TRY(z = x >> 0);   TEST_EQUAL(uint64_t(z), 0x123456789ull);
    TRY(z = x << 5);   TEST_EQUAL(uint64_t(z), 0x468acf120ull);  TRY(z = x >> 5);   TEST_EQUAL(uint64_t(z), 0x0091a2b3cull);
    TRY(z = x << 10);  TEST_EQUAL(uint64_t(z), 0x5159e2400ull);  TRY(z = x >> 10);  TEST_EQUAL(uint64_t(z), 0x00048d159ull);
    TRY(z = x << 15);  TEST_EQUAL(uint64_t(z), 0x2b3c48000ull);  TRY(z = x >> 15);  TEST_EQUAL(uint64_t(z), 0x00002468aull);
    TRY(z = x << 20);  TEST_EQUAL(uint64_t(z), 0x678900000ull);  TRY(z = x >> 20);  TEST_EQUAL(uint64_t(z), 0x000001234ull);
    TRY(z = x << 25);  TEST_EQUAL(uint64_t(z), 0x712000000ull);  TRY(z = x >> 25);  TEST_EQUAL(uint64_t(z), 0x000000091ull);
    TRY(z = x << 30);  TEST_EQUAL(uint64_t(z), 0x240000000ull);  TRY(z = x >> 30);  TEST_EQUAL(uint64_t(z), 0x000000004ull);
    TRY(z = x << 35);  TEST_EQUAL(uint64_t(z), 0u);                TRY(z = x >> 35);  TEST_EQUAL(uint64_t(z), 0u);

    TEST(++x);    TEST_EQUAL(uint64_t(x), 0x12345678aull);
    TEST(--x);    TEST_EQUAL(uint64_t(x), 0x123456789ull);
    TEST(x--);    TEST_EQUAL(uint64_t(x), 0x123456788ull);
    TEST(x++);    TEST_EQUAL(uint64_t(x), 0x123456789ull);
    TEST(++y);    TEST_EQUAL(uint64_t(y), 0xabcdf0ull);
    TEST(--y);    TEST_EQUAL(uint64_t(y), 0xabcdefull);
    TEST(y--);    TEST_EQUAL(uint64_t(y), 0xabcdeeull);
    TEST(y++);    TEST_EQUAL(uint64_t(y), 0xabcdefull);
    TEST(++z);    TEST_EQUAL(uint64_t(z), 1u);
    TEST(! --z);  TEST_EQUAL(uint64_t(z), 0u);
    TEST(! z--);  TEST_EQUAL(uint64_t(z), 0x7ffffffffull);
    TEST(z++);    TEST_EQUAL(uint64_t(z), 0u);

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TRY(x = rotl(y, 0));   TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotl(y, 5));   TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotl(y, 10));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotl(y, 15));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotl(y, 20));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotl(y, 25));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotl(y, 30));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotl(y, 35));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotl(y, 40));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotl(y, 45));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotl(y, 50));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotl(y, 55));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotl(y, 60));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotl(y, 65));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotl(y, 70));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);

    TRY(x = rotr(y, 0));   TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotr(y, 5));   TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotr(y, 10));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotr(y, 15));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotr(y, 20));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotr(y, 25));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotr(y, 30));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotr(y, 35));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotr(y, 40));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotr(y, 45));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotr(y, 50));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotr(y, 55));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotr(y, 60));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotr(y, 65));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotr(y, 70));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);

    TRY(x = binary::from_double(12345678901.0));
    TEST_EQUAL(x.dec(), "12345678901");
    TEST_EQUAL(double(x), 12345678901.0);
    TEST_EQUAL(uint64_t(x), 12345678901ull);

    TRY(x = 0ull);      TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 127ull);    TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 128ull);    TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 32767ull);  TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 32768ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());

    TRY(x = 0ull);      TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), 0);       TEST_EQUAL(int32_t(x), 0);      TEST_EQUAL(int64_t(x), 0ll);
    TRY(x = 127ull);    TEST_EQUAL(int8_t(x), 127);   TEST_EQUAL(int16_t(x), 127);     TEST_EQUAL(int32_t(x), 127);    TEST_EQUAL(int64_t(x), 127ll);
    TRY(x = 128ull);    TEST_EQUAL(int8_t(x), -128);  TEST_EQUAL(int16_t(x), 128);     TEST_EQUAL(int32_t(x), 128);    TEST_EQUAL(int64_t(x), 128ll);
    TRY(x = 32767ull);  TEST_EQUAL(int8_t(x), -1);    TEST_EQUAL(int16_t(x), 32767);   TEST_EQUAL(int32_t(x), 32767);  TEST_EQUAL(int64_t(x), 32767ll);
    TRY(x = 32768ull);  TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), -32768);  TEST_EQUAL(int32_t(x), 32768);  TEST_EQUAL(int64_t(x), 32768ll);

}

void test_rs_tl_fixed_binary_large_binary_35() {

    using binary = LargeBinary<35>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(uint64_t(binary::max()), 0x7ffffffffull);
    TEST_EQUAL(binary::max().hex(), "7ffffffff");

    TEST(c.fits_in<int64_t>());  TEST_EQUAL(int64_t(c), 42);  TEST(c);
    TEST(x.fits_in<int64_t>());  TEST_EQUAL(int64_t(x), 0);   TEST(! x);

    TRY(x = binary(0x123456789ull));
    TRY(y = binary(0xabcdefull));

    TEST_EQUAL(int64_t(x), 4886718345);
    TEST_EQUAL(int64_t(y), 11259375);
    TEST_EQUAL(int64_t(z), 0);

    TEST_EQUAL(x.bin(), "00100100011010001010110011110001001");
    TEST_EQUAL(y.bin(), "00000000000101010111100110111101111");
    TEST_EQUAL(z.bin(), "00000000000000000000000000000000000");
    TEST_EQUAL(x.dec(), "4886718345");
    TEST_EQUAL(y.dec(), "11259375");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "123456789");
    TEST_EQUAL(y.hex(), "000abcdef");
    TEST_EQUAL(z.hex(), "000000000");
    TEST_EQUAL(double(x), 4886718345.0);
    TEST_EQUAL(double(y), 11259375.0);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 33u);
    TEST_EQUAL(y.significant_bits(), 24u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(uint64_t(z), 0u);
    TRY(z = - x);  TEST_EQUAL(uint64_t(z), 0x6dcba9877ull);
    TRY(z = - y);  TEST_EQUAL(uint64_t(z), 0x7ff543211ull);

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(uint64_t(z), 0x7ffffffffull);
    TRY(z = ~ x);  TEST_EQUAL(uint64_t(z), 0x6dcba9876ull);
    TRY(z = ~ y);  TEST_EQUAL(uint64_t(z), 0x7ff543210ull);

    TRY(z = x + y);  TEST_EQUAL(uint64_t(z), 0x123f13578ull);
    TRY(z = x - y);  TEST_EQUAL(uint64_t(z), 0x12299999aull);
    TRY(z = y - x);  TEST_EQUAL(uint64_t(z), 0x6dd666666ull);
    TRY(z = x * y);  TEST_EQUAL(uint64_t(z), 0x2aa375de7ull);
    TRY(z = x / y);  TEST_EQUAL(uint64_t(z), 0x0000001b2ull);
    TRY(z = x % y);  TEST_EQUAL(uint64_t(z), 0x00002485bull);
    TRY(z = y / x);  TEST_EQUAL(uint64_t(z), 0u);
    TRY(z = y % x);  TEST_EQUAL(uint64_t(z), 0x000abcdefull);
    TRY(z = x & y);  TEST_EQUAL(uint64_t(z), 0x000014589ull);
    TRY(z = x | y);  TEST_EQUAL(uint64_t(z), 0x123efefefull);
    TRY(z = x ^ y);  TEST_EQUAL(uint64_t(z), 0x123eeaa66ull);

    TRY(z = x << 0);   TEST_EQUAL(uint64_t(z), 0x123456789ull);  TRY(z = x >> 0);   TEST_EQUAL(uint64_t(z), 0x123456789ull);
    TRY(z = x << 5);   TEST_EQUAL(uint64_t(z), 0x468acf120ull);  TRY(z = x >> 5);   TEST_EQUAL(uint64_t(z), 0x0091a2b3cull);
    TRY(z = x << 10);  TEST_EQUAL(uint64_t(z), 0x5159e2400ull);  TRY(z = x >> 10);  TEST_EQUAL(uint64_t(z), 0x00048d159ull);
    TRY(z = x << 15);  TEST_EQUAL(uint64_t(z), 0x2b3c48000ull);  TRY(z = x >> 15);  TEST_EQUAL(uint64_t(z), 0x00002468aull);
    TRY(z = x << 20);  TEST_EQUAL(uint64_t(z), 0x678900000ull);  TRY(z = x >> 20);  TEST_EQUAL(uint64_t(z), 0x000001234ull);
    TRY(z = x << 25);  TEST_EQUAL(uint64_t(z), 0x712000000ull);  TRY(z = x >> 25);  TEST_EQUAL(uint64_t(z), 0x000000091ull);
    TRY(z = x << 30);  TEST_EQUAL(uint64_t(z), 0x240000000ull);  TRY(z = x >> 30);  TEST_EQUAL(uint64_t(z), 0x000000004ull);
    TRY(z = x << 35);  TEST_EQUAL(uint64_t(z), 0u);                TRY(z = x >> 35);  TEST_EQUAL(uint64_t(z), 0u);

    TEST(++x);    TEST_EQUAL(x.hex(), "12345678a");
    TEST(--x);    TEST_EQUAL(x.hex(), "123456789");
    TEST(x--);    TEST_EQUAL(x.hex(), "123456788");
    TEST(x++);    TEST_EQUAL(x.hex(), "123456789");
    TEST(++y);    TEST_EQUAL(y.hex(), "000abcdf0");
    TEST(--y);    TEST_EQUAL(y.hex(), "000abcdef");
    TEST(y--);    TEST_EQUAL(y.hex(), "000abcdee");
    TEST(y++);    TEST_EQUAL(y.hex(), "000abcdef");
    TEST(++z);    TEST_EQUAL(z.hex(), "000000001");
    TEST(! --z);  TEST_EQUAL(z.hex(), "000000000");
    TEST(! z--);  TEST_EQUAL(z.hex(), "7ffffffff");
    TEST(z++);    TEST_EQUAL(z.hex(), "000000000");

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TEST_EQUAL(x.dec(), "4886718345");
    TEST_EQUAL(y.dec(), "11259375");
    TEST_EQUAL(z.dec(), "0");

    TRY(x = rotl(y, 0));   TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotl(y, 5));   TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotl(y, 10));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotl(y, 15));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotl(y, 20));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotl(y, 25));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotl(y, 30));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotl(y, 35));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotl(y, 40));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotl(y, 45));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotl(y, 50));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotl(y, 55));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotl(y, 60));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotl(y, 65));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotl(y, 70));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);

    TRY(x = rotr(y, 0));   TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotr(y, 5));   TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotr(y, 10));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotr(y, 15));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotr(y, 20));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotr(y, 25));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotr(y, 30));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotr(y, 35));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);
    TRY(x = rotr(y, 40));  TEST_EQUAL(uint64_t(x), 0x3c0055e6full);
    TRY(x = rotr(y, 45));  TEST_EQUAL(uint64_t(x), 0x3de002af3ull);
    TRY(x = rotr(y, 50));  TEST_EQUAL(uint64_t(x), 0x4def00157ull);
    TRY(x = rotr(y, 55));  TEST_EQUAL(uint64_t(x), 0x5e6f7800aull);
    TRY(x = rotr(y, 60));  TEST_EQUAL(uint64_t(x), 0x2af37bc00ull);
    TRY(x = rotr(y, 65));  TEST_EQUAL(uint64_t(x), 0x01579bde0ull);
    TRY(x = rotr(y, 70));  TEST_EQUAL(uint64_t(x), 0x000abcdefull);

    TRY(x = binary::from_double(12345678901.0));
    TEST_EQUAL(x.dec(), "12345678901");
    TEST_EQUAL(double(x), 12345678901.0);
    TEST_EQUAL(uint64_t(x), 12345678901ull);

    TRY(x = 0ull);      TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 127ull);    TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 128ull);    TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 32767ull);  TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 32768ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());

    TRY(x = 0ull);      TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), 0);       TEST_EQUAL(int32_t(x), 0);      TEST_EQUAL(int64_t(x), 0ll);
    TRY(x = 127ull);    TEST_EQUAL(int8_t(x), 127);   TEST_EQUAL(int16_t(x), 127);     TEST_EQUAL(int32_t(x), 127);    TEST_EQUAL(int64_t(x), 127ll);
    TRY(x = 128ull);    TEST_EQUAL(int8_t(x), -128);  TEST_EQUAL(int16_t(x), 128);     TEST_EQUAL(int32_t(x), 128);    TEST_EQUAL(int64_t(x), 128ll);
    TRY(x = 32767ull);  TEST_EQUAL(int8_t(x), -1);    TEST_EQUAL(int16_t(x), 32767);   TEST_EQUAL(int32_t(x), 32767);  TEST_EQUAL(int64_t(x), 32767ll);
    TRY(x = 32768ull);  TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), -32768);  TEST_EQUAL(int32_t(x), 32768);  TEST_EQUAL(int64_t(x), 32768ll);

}

void test_rs_tl_fixed_binary_large_binary_100() {

    using binary = LargeBinary<100>;

    static const binary c(42);
    binary x, y, z;

    TEST_EQUAL(binary::max().hex(), "fffffffffffffffffffffffff");

    TEST(c.fits_in<int64_t>());  TEST_EQUAL(int64_t(c), 42);  TEST(c);
    TEST(x.fits_in<int64_t>());  TEST_EQUAL(int64_t(x), 0);   TEST(! x);

    TRY((x = binary{0xfedcba987ull, 0x6543210fedcba987ull}));
    TRY((y = binary{0x123456789ull, 0xabcdef0123456789ull}));

    TEST_EQUAL(uint64_t(x), 0x6543210fedcba987ull);
    TEST_EQUAL(uint64_t(y), 0xabcdef0123456789ull);
    TEST_EQUAL(uint64_t(z), 0u);

    TEST_EQUAL(x.bin(), "1111111011011100101110101001100001110110010101000011001000010000111111101101110010111010100110000111");
    TEST_EQUAL(y.bin(), "0001001000110100010101100111100010011010101111001101111011110000000100100011010001010110011110001001");
    TEST_EQUAL(z.bin(), "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    TEST_EQUAL(x.dec(), "1262016597560548382007796410759");
    TEST_EQUAL(y.dec(), "90144042682896311822508713865");
    TEST_EQUAL(z.dec(), "0");
    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba987");
    TEST_EQUAL(y.hex(), "123456789abcdef0123456789");
    TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TEST_NEAR(double(x), 1.262016598e30, 1e21);
    TEST_NEAR(double(y), 9.014404268e28, 1e19);
    TEST_EQUAL(double(z), 0.0);
    TEST_EQUAL(x.significant_bits(), 100u);
    TEST_EQUAL(y.significant_bits(), 97u);
    TEST_EQUAL(z.significant_bits(), 0u);

    TRY(z = - z);  TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TRY(z = - x);  TEST_EQUAL(z.hex(), "0123456789abcdef012345679");
    TRY(z = - y);  TEST_EQUAL(z.hex(), "edcba9876543210fedcba9877");

    TRY(z.clear());
    TEST(! z);
    TEST_EQUAL(z, binary());

    TRY(z = ~ z);  TEST_EQUAL(z.hex(), "fffffffffffffffffffffffff");
    TRY(z = ~ x);  TEST_EQUAL(z.hex(), "0123456789abcdef012345678");
    TRY(z = ~ y);  TEST_EQUAL(z.hex(), "edcba9876543210fedcba9876");

    TRY(z = x + y);  TEST_EQUAL(z.hex(), "1111111111111101111111110");
    TRY(z = x - y);  TEST_EQUAL(z.hex(), "eca8641fdb975320eca8641fe");
    TRY(z = y - x);  TEST_EQUAL(z.hex(), "13579be02468acdf13579be02");
    TRY(z = x * y);  TEST_EQUAL(z.hex(), "9efd92c744933bccc59960a3f");
    TRY(z = x / y);  TEST_EQUAL(z.hex(), "000000000000000000000000e");
    TRY(z = x % y);  TEST_EQUAL(z.hex(), "00000000000000f0000000009");
    TRY(z = y / x);  TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TRY(z = y % x);  TEST_EQUAL(z.hex(), "123456789abcdef0123456789");
    TRY(z = x & y);  TEST_EQUAL(z.hex(), "1214121812141210121412181");
    TRY(z = x | y);  TEST_EQUAL(z.hex(), "fefcfef8fefcfef0fefcfef8f");
    TRY(z = x ^ y);  TEST_EQUAL(z.hex(), "ece8ece0ece8ece0ece8ece0e");

    TRY(z = x << 0);    TEST_EQUAL(z.hex(), "fedcba9876543210fedcba987");  TRY(z = x >> 0);    TEST_EQUAL(z.hex(), "fedcba9876543210fedcba987");
    TRY(z = x << 10);   TEST_EQUAL(z.hex(), "72ea61d950c843fb72ea61c00");  TRY(z = x >> 10);   TEST_EQUAL(z.hex(), "003fb72ea61d950c843fb72ea");
    TRY(z = x << 20);   TEST_EQUAL(z.hex(), "a9876543210fedcba98700000");  TRY(z = x >> 20);   TEST_EQUAL(z.hex(), "00000fedcba9876543210fedc");
    TRY(z = x << 30);   TEST_EQUAL(z.hex(), "1d950c843fb72ea61c0000000");  TRY(z = x >> 30);   TEST_EQUAL(z.hex(), "00000003fb72ea61d950c843f");
    TRY(z = x << 40);   TEST_EQUAL(z.hex(), "543210fedcba9870000000000");  TRY(z = x >> 40);   TEST_EQUAL(z.hex(), "0000000000fedcba987654321");
    TRY(z = x << 50);   TEST_EQUAL(z.hex(), "c843fb72ea61c000000000000");  TRY(z = x >> 50);   TEST_EQUAL(z.hex(), "0000000000003fb72ea61d950");
    TRY(z = x << 60);   TEST_EQUAL(z.hex(), "0fedcba987000000000000000");  TRY(z = x >> 60);   TEST_EQUAL(z.hex(), "000000000000000fedcba9876");
    TRY(z = x << 70);   TEST_EQUAL(z.hex(), "b72ea61c00000000000000000");  TRY(z = x >> 70);   TEST_EQUAL(z.hex(), "000000000000000003fb72ea6");
    TRY(z = x << 80);   TEST_EQUAL(z.hex(), "ba98700000000000000000000");  TRY(z = x >> 80);   TEST_EQUAL(z.hex(), "00000000000000000000fedcb");
    TRY(z = x << 90);   TEST_EQUAL(z.hex(), "61c0000000000000000000000");  TRY(z = x >> 90);   TEST_EQUAL(z.hex(), "00000000000000000000003fb");
    TRY(z = x << 100);  TEST_EQUAL(z.hex(), "0000000000000000000000000");  TRY(z = x >> 100);  TEST_EQUAL(z.hex(), "0000000000000000000000000");

    TEST(++x);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba988");
    TEST(--x);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba987");
    TEST(x--);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba986");
    TEST(x++);    TEST_EQUAL(x.hex(), "fedcba9876543210fedcba987");
    TEST(++y);    TEST_EQUAL(y.hex(), "123456789abcdef012345678a");
    TEST(--y);    TEST_EQUAL(y.hex(), "123456789abcdef0123456789");
    TEST(y--);    TEST_EQUAL(y.hex(), "123456789abcdef0123456788");
    TEST(y++);    TEST_EQUAL(y.hex(), "123456789abcdef0123456789");
    TEST(++z);    TEST_EQUAL(z.hex(), "0000000000000000000000001");
    TEST(! --z);  TEST_EQUAL(z.hex(), "0000000000000000000000000");
    TEST(! z--);  TEST_EQUAL(z.hex(), "fffffffffffffffffffffffff");
    TEST(z++);    TEST_EQUAL(z.hex(), "0000000000000000000000000");

    TEST(x != y);
    TEST(x > y);
    TEST(x >= y);
    TEST(y != x);
    TEST(y < x);
    TEST(y <= x);

    TEST_EQUAL(x.dec(), "1262016597560548382007796410759");
    TEST_EQUAL(y.dec(), "90144042682896311822508713865");
    TEST_EQUAL(z.dec(), "0");

    TRY(x = rotl(y, 0));    TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotl(y, 10));   TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotl(y, 20));   TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotl(y, 30));   TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotl(y, 40));   TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotl(y, 50));   TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotl(y, 60));   TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotl(y, 70));   TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotl(y, 80));   TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotl(y, 90));   TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotl(y, 100));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotl(y, 110));  TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotl(y, 120));  TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotl(y, 130));  TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotl(y, 140));  TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotl(y, 150));  TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotl(y, 160));  TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotl(y, 170));  TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotl(y, 180));  TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotl(y, 190));  TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotl(y, 200));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");

    TRY(x = rotr(y, 0));    TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotr(y, 10));   TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotr(y, 20));   TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotr(y, 30));   TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotr(y, 40));   TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotr(y, 50));   TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotr(y, 60));   TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotr(y, 70));   TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotr(y, 80));   TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotr(y, 90));   TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotr(y, 100));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");
    TRY(x = rotr(y, 110));  TEST_EQUAL(x.hex(), "e2448d159e26af37bc048d159");
    TRY(x = rotr(y, 120));  TEST_EQUAL(x.hex(), "56789123456789abcdef01234");
    TRY(x = rotr(y, 130));  TEST_EQUAL(x.hex(), "8d159e2448d159e26af37bc04");
    TRY(x = rotr(y, 140));  TEST_EQUAL(x.hex(), "0123456789123456789abcdef");
    TRY(x = rotr(y, 150));  TEST_EQUAL(x.hex(), "7bc048d159e2448d159e26af3");
    TRY(x = rotr(y, 160));  TEST_EQUAL(x.hex(), "bcdef0123456789123456789a");
    TRY(x = rotr(y, 170));  TEST_EQUAL(x.hex(), "26af37bc048d159e2448d159e");
    TRY(x = rotr(y, 180));  TEST_EQUAL(x.hex(), "6789abcdef012345678912345");
    TRY(x = rotr(y, 190));  TEST_EQUAL(x.hex(), "d159e26af37bc048d159e2448");
    TRY(x = rotr(y, 200));  TEST_EQUAL(x.hex(), "123456789abcdef0123456789");

    TRY(x = binary::from_double(1.23456789e30));
    TEST_MATCH(x.dec(), "^12345678\\d{23}$");
    TEST_NEAR(double(x), 1.23456789e30, 1e21);

    TRY(x = 0ull);                    TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 127ull);                  TEST(x.fits_in<int8_t>());    TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 128ull);                  TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 32767ull);                TEST(! x.fits_in<int8_t>());  TEST(x.fits_in<int16_t>());    TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 32768ull);                TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 2147483647ull);           TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(x.fits_in<int32_t>());    TEST(x.fits_in<int64_t>());
    TRY(x = 2147483648ull);           TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(! x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 9223372036854775807ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(! x.fits_in<int32_t>());  TEST(x.fits_in<int64_t>());
    TRY(x = 9223372036854775808ull);  TEST(! x.fits_in<int8_t>());  TEST(! x.fits_in<int16_t>());  TEST(! x.fits_in<int32_t>());  TEST(! x.fits_in<int64_t>());

    TRY(x = 0ull);                    TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), 0);       TEST_EQUAL(int32_t(x), 0);          TEST_EQUAL(int64_t(x), 0);
    TRY(x = 127ull);                  TEST_EQUAL(int8_t(x), 127);   TEST_EQUAL(int16_t(x), 127);     TEST_EQUAL(int32_t(x), 127);        TEST_EQUAL(int64_t(x), 127);
    TRY(x = 128ull);                  TEST_EQUAL(int8_t(x), -128);  TEST_EQUAL(int16_t(x), 128);     TEST_EQUAL(int32_t(x), 128);        TEST_EQUAL(int64_t(x), 128);
    TRY(x = 32767ull);                TEST_EQUAL(int8_t(x), -1);    TEST_EQUAL(int16_t(x), 32767);   TEST_EQUAL(int32_t(x), 32767);      TEST_EQUAL(int64_t(x), 32767);
    TRY(x = 32768ull);                TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), -32768);  TEST_EQUAL(int32_t(x), 32768);      TEST_EQUAL(int64_t(x), 32768);
    TRY(x = 2147483647ull);           TEST_EQUAL(int8_t(x), -1);    TEST_EQUAL(int16_t(x), -1);      TEST_EQUAL(int32_t(x), max_int32);  TEST_EQUAL(int64_t(x), 2147483647);
    TRY(x = 2147483648ull);           TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), 0);       TEST_EQUAL(int32_t(x), min_int32);  TEST_EQUAL(int64_t(x), 2147483648);
    TRY(x = 9223372036854775807ull);  TEST_EQUAL(int8_t(x), -1);    TEST_EQUAL(int16_t(x), -1);      TEST_EQUAL(int32_t(x), -1);         TEST_EQUAL(int64_t(x), max_int64);
    TRY(x = 9223372036854775808ull);  TEST_EQUAL(int8_t(x), 0);     TEST_EQUAL(int16_t(x), 0);       TEST_EQUAL(int32_t(x), 0);          TEST_EQUAL(int64_t(x), min_int64);

}

namespace {

    template <typename T1, typename T2>
    void do_conversion_tests() {

        const auto ca = 0x0123456789abcdefull;
        const auto cb = ~ ca;
        const auto mask1 = uint64_t(T1::max());
        const auto mask2 = uint64_t(T2::max());

        T1 x1, y1, z1;
        T2 x2, y2, z2;

        TRY(x1 = static_cast<T1>(ca));  TEST_EQUAL(uint64_t(x1), ca & mask1);
        TRY(y2 = static_cast<T2>(x1));  TEST_EQUAL(uint64_t(y2), ca & mask1 & mask2);
        TRY(z1 = static_cast<T1>(y2));  TEST_EQUAL(uint64_t(z1), uint64_t(y2));
        TRY(x2 = static_cast<T2>(ca));  TEST_EQUAL(uint64_t(x2), ca & mask2);
        TRY(y1 = static_cast<T1>(x2));  TEST_EQUAL(uint64_t(y1), ca & mask2 & mask1);
        TRY(z2 = static_cast<T2>(y1));  TEST_EQUAL(uint64_t(z2), uint64_t(y1));
        TRY(x1 = static_cast<T1>(cb));  TEST_EQUAL(uint64_t(x1), cb & mask1);
        TRY(y2 = static_cast<T2>(x1));  TEST_EQUAL(uint64_t(y2), cb & mask1 & mask2);
        TRY(z1 = static_cast<T1>(y2));  TEST_EQUAL(uint64_t(z1), uint64_t(y2));
        TRY(x2 = static_cast<T2>(cb));  TEST_EQUAL(uint64_t(x2), cb & mask2);
        TRY(y1 = static_cast<T1>(x2));  TEST_EQUAL(uint64_t(y1), cb & mask2 & mask1);
        TRY(z2 = static_cast<T2>(y1));  TEST_EQUAL(uint64_t(z2), uint64_t(y1));

    }

}

void test_rs_tl_fixed_binary_type_conversions() {

    do_conversion_tests<SmallBinary<10>, SmallBinary<31>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<32>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<33>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<63>>();
    do_conversion_tests<SmallBinary<10>, SmallBinary<64>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<31>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<32>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<33>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<63>>();
    do_conversion_tests<SmallBinary<10>, LargeBinary<64>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<31>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<32>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<33>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<63>>();
    do_conversion_tests<LargeBinary<10>, SmallBinary<64>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<31>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<32>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<33>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<63>>();
    do_conversion_tests<LargeBinary<10>, LargeBinary<64>>();

}

void test_rs_tl_fixed_binary_string_parsing() {

    SmallBinary<35> x;
    LargeBinary<35> y;

    TEST(! SmallBinary<35>::try_parse_dec("", x));
    TEST(! SmallBinary<35>::try_parse_dec("a", x));
    TEST(! SmallBinary<35>::try_parse_hex("123456789xyz", x));
    TEST(SmallBinary<35>::try_parse_dec("0", x));                   TEST_EQUAL(uint64_t(x), 0u);
    TEST(SmallBinary<35>::try_parse_dec("42", x));                  TEST_EQUAL(uint64_t(x), 42u);
    TEST(SmallBinary<35>::try_parse_dec("1234567890", x));          TEST_EQUAL(uint64_t(x), 1234567890ull);
    TEST(SmallBinary<35>::try_parse_hex("123456789", x));           TEST_EQUAL(uint64_t(x), 0x123456789ull);
    TEST(SmallBinary<35>::try_parse_bin("101010110011001100", x));  TEST_EQUAL(uint64_t(x), 0x00002acccull);

    TEST(! LargeBinary<35>::try_parse_dec("", y));
    TEST(! LargeBinary<35>::try_parse_dec("a", y));
    TEST(! LargeBinary<35>::try_parse_hex("123456789xyz", y));
    TEST(LargeBinary<35>::try_parse_dec("0", y));                   TEST_EQUAL(uint64_t(y), 0u);
    TEST(LargeBinary<35>::try_parse_dec("42", y));                  TEST_EQUAL(uint64_t(y), 42u);
    TEST(LargeBinary<35>::try_parse_dec("1234567890", y));          TEST_EQUAL(uint64_t(y), 1234567890ull);
    TEST(LargeBinary<35>::try_parse_hex("123456789", y));           TEST_EQUAL(uint64_t(y), 0x123456789ull);
    TEST(LargeBinary<35>::try_parse_bin("101010110011001100", y));  TEST_EQUAL(uint64_t(y), 0x00002acccull);

}

void test_rs_tl_fixed_binary_hash_set() {

    std::unordered_set<SmallBinary<5>> set_s5;
    std::unordered_set<SmallBinary<50>> set_s50;
    std::unordered_set<LargeBinary<50>> set_l50;
    std::unordered_set<LargeBinary<500>> set_l500;

    TEST(set_s5.empty());
    TEST(set_s50.empty());
    TEST(set_l50.empty());
    TEST(set_l500.empty());

    for (int i = 1; i <= 10; ++i) {
        TRY(set_s5.insert(SmallBinary<5>(i)));
        TRY(set_s50.insert(SmallBinary<50>(i)));
        TRY(set_l50.insert(LargeBinary<50>(i)));
        TRY(set_l500.insert(LargeBinary<500>(i)));
    }

    TEST_EQUAL(set_s5.size(), 10u);
    TEST_EQUAL(set_s50.size(), 10u);
    TEST_EQUAL(set_l50.size(), 10u);
    TEST_EQUAL(set_l500.size(), 10u);

}
