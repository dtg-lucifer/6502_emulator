if(BUILD_TESTING)
    add_executable(main_test
        main_test.cpp
    )
    target_link_libraries(main_test PRIVATE
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(main_test)
endif()
