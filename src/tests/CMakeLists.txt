AUX_SOURCE_DIRECTORY(./src TEST_SOURCES)
add_executable(TESTING
    ${TEST_SOURCES}
)
AUX_SOURCE_DIRECTORY(./src MLP_SRCS)
file(GLOB_RECURSE MLP_HEADERS
    includes/*.h
)

target_link_libraries(TESTING
    Network
    Qt${QT_VERSION_MAJOR}::Widgets 
    Utils 
    EasyBMP
    gtest
)

foreach(RESOURCE_FILE ${RESOURCE_FILES})
    file(RELATIVE_PATH RELATIVE_PATH_TO_RESOURCE ${RES_DIR} ${RESOURCE_FILE})
    add_custom_command(
        TARGET TESTING POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RES_DIR}/${RELATIVE_PATH_TO_RESOURCE}
            $<TARGET_FILE_DIR:TESTING>/${RES_NAME}/${RELATIVE_PATH_TO_RESOURCE}
    )
endforeach()

include(GoogleTest)
gtest_discover_tests(TESTING)
add_test(NAME TestMyCode COMMAND TestMyCode)
