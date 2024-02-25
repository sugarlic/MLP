set(LIB_NAME Utils)
AUX_SOURCE_DIRECTORY(./src UTILS_SRCS)
file(GLOB_RECURSE UTILS_HEADERS
    includes/*.h
)
add_library(${LIB_NAME} STATIC
    ${UTILS_SRCS}
    ${UTILS_HEADERS}
)
target_link_libraries(${LIB_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets EasyBMP)
target_include_directories(${LIB_NAME} PUBLIC includes)
