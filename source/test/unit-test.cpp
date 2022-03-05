// This file is generated by the rs-update-tests script

#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // types-test.cpp
    UNIT_TEST(rs_tl_types_mixins)
    UNIT_TEST(rs_tl_types_traits)
    UNIT_TEST(rs_tl_types_iterator_category)
    UNIT_TEST(rs_tl_types_range_category)

    // iterator-test.cpp
    UNIT_TEST(rs_tl_iterator_mixins)
    UNIT_TEST(rs_tl_iterator_dereference)
    UNIT_TEST(rs_tl_iterator_append_overwrite)
    UNIT_TEST(rs_tl_iterator_subrange)

    // log-test.cpp
    UNIT_TEST(rs_format_logging)
    UNIT_TEST(rs_format_logging_output)

    // meta-test.cpp
    UNIT_TEST(rs_tl_meta_append)
    UNIT_TEST(rs_tl_meta_concat)
    UNIT_TEST(rs_tl_meta_insert)
    UNIT_TEST(rs_tl_meta_insert_at)
    UNIT_TEST(rs_tl_meta_prefix)
    UNIT_TEST(rs_tl_meta_repeat)
    UNIT_TEST(rs_tl_meta_repeat_list)
    UNIT_TEST(rs_tl_meta_resize)
    UNIT_TEST(rs_tl_meta_skip)
    UNIT_TEST(rs_tl_meta_sublist)
    UNIT_TEST(rs_tl_meta_take)
    UNIT_TEST(rs_tl_meta_at_index)
    UNIT_TEST(rs_tl_meta_head)
    UNIT_TEST(rs_tl_meta_tail)
    UNIT_TEST(rs_tl_meta_most)
    UNIT_TEST(rs_tl_meta_last)
    UNIT_TEST(rs_tl_meta_max_min)
    UNIT_TEST(rs_tl_meta_fold)
    UNIT_TEST(rs_tl_meta_make_set)
    UNIT_TEST(rs_tl_meta_map)
    UNIT_TEST(rs_tl_meta_partial_reduce)
    UNIT_TEST(rs_tl_meta_remove)
    UNIT_TEST(rs_tl_meta_reverse)
    UNIT_TEST(rs_tl_meta_select)
    UNIT_TEST(rs_tl_meta_sort)
    UNIT_TEST(rs_tl_meta_unique)
    UNIT_TEST(rs_tl_meta_zip)
    UNIT_TEST(rs_tl_meta_inherit)
    UNIT_TEST(rs_tl_meta_tuples)
    UNIT_TEST(rs_tl_meta_all_of)
    UNIT_TEST(rs_tl_meta_count)
    UNIT_TEST(rs_tl_meta_find)
    UNIT_TEST(rs_tl_meta_in_list)
    UNIT_TEST(rs_tl_meta_is_empty)
    UNIT_TEST(rs_tl_meta_is_unique)
    UNIT_TEST(rs_tl_meta_length_of)

    // algorithm-test.cpp
    UNIT_TEST(rs_tl_algorithm_container_algorithms)
    UNIT_TEST(rs_tl_algorithm_diff)
    UNIT_TEST(rs_tl_algorithm_edit_distance)

    // binary-test.cpp
    UNIT_TEST(rs_tl_binary_byte_order)
    UNIT_TEST(rs_tl_binary_birwise_operations)

    // fixed-binary-small-binary-5-test.cpp
    UNIT_TEST(rs_tl_fixed_binary_small_binary_5)

    // fixed-binary-small-binary-35-test.cpp
    UNIT_TEST(rs_tl_fixed_binary_small_binary_35)

    // fixed-binary-large-binary-35-test.cpp
    UNIT_TEST(rs_tl_fixed_binary_large_binary_35)

    // fixed-binary-large-binary-100-test.cpp
    UNIT_TEST(rs_tl_fixed_binary_large_binary_100)

    // fixed-binary-misc-test.cpp
    UNIT_TEST(rs_tl_fixed_binary_implementation_selection)
    UNIT_TEST(rs_tl_fixed_binary_type_conversions)
    UNIT_TEST(rs_tl_fixed_binary_string_parsing)
    UNIT_TEST(rs_tl_fixed_binary_hash_set)

    // guard-test.cpp
    UNIT_TEST(rs_tl_scope_guards)

    // bounded-array-construction-test.cpp
    UNIT_TEST(rs_tl_bounded_array_construction)

    // bounded-array-insertion-test.cpp
    UNIT_TEST(rs_tl_bounded_array_insertion)

    // bounded-array-tracking-test.cpp
    UNIT_TEST(rs_tl_bounded_array_tracking)

    // bounded-array-misc-test.cpp
    UNIT_TEST(rs_tl_bounded_array_capacity)
    UNIT_TEST(rs_tl_bounded_array_keys)

    // compact-array-construction-test.cpp
    UNIT_TEST(rs_tl_compact_array_construction)

    // compact-array-insertion-test.cpp
    UNIT_TEST(rs_tl_compact_array_insertion)

    // compact-array-tracking-test.cpp
    UNIT_TEST(rs_tl_compact_array_tracking)

    // compact-array-misc-test.cpp
    UNIT_TEST(rs_tl_compact_array_capacity)
    UNIT_TEST(rs_tl_compact_array_keys)

    // index-table-test.cpp
    UNIT_TEST(rs_tl_index_table_classes)

    // mirror-map-test.cpp
    UNIT_TEST(rs_tl_mirror_map_construct)
    UNIT_TEST(rs_tl_mirror_map_iterators)
    UNIT_TEST(rs_tl_mirror_map_insert)
    UNIT_TEST(rs_tl_mirror_map_erase)
    UNIT_TEST(rs_tl_mirror_map_search)
    UNIT_TEST(rs_tl_mirror_map_duplicates)

    // stack-test.cpp
    UNIT_TEST(rs_tl_stack)

    // topological-order-test.cpp
    UNIT_TEST(rs_tl_topological_order)
    UNIT_TEST(rs_tl_topological_order_reverse)

    // uuid-test.cpp
    UNIT_TEST(rs_tl_uuid)

    // version-test.cpp
    UNIT_TEST(rs_tl_version)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
