set(LIB_NAME EasyBMP)
AUX_SOURCE_DIRECTORY(./src ${LIB_NAME}_SRCS)
file(GLOB_RECURSE ${LIB_NAME}_HEADERS
    includes/*.h
)
add_library(${LIB_NAME} STATIC
    ${${LIB_NAME}_SRCS}
    ${${LIB_NAME}_HEADERS}
)
target_include_directories(${LIB_NAME} PUBLIC includes)