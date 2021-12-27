set(SOURCE_FILES    main.cpp
)

add_executable(raytracer ${SOURCE_FILES})

check_ipo_supported(RESULT ipo_result)
if(ipo_result)
	set_property(TARGET raytracer PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(raytracer PUBLIC m)

# All users of this library will need at least C++11
target_compile_features(raytracer PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
	TREE "${PROJECT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${HEADER_LIST}
)
