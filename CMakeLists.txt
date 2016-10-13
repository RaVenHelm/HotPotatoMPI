cmake_minimum_required(VERSION 2.8)
project(hot-potato)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(BOOST_ROOT ~/boost-build/boost_1_62_0/staging)

find_package(Boost 1.62.0 REQUIRED COMPONENTS mpi serialization)
find_package(MPI REQUIRED)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(${MPI_INCLUDE_PATH})

add_executable(hot-potato app.cpp)
target_link_libraries(hot-potato
	${Boost_LIBRARIES}
	${MPI_LIBRARIES}
)


if(MPI_COMPILE_FLAGS)
	set_target_properties(hot-potato PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
	message("MPI Compile flags: ${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
	set_target_properties(hot-potato PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
	message("MPI Link flags: ${MPI_LINK_FLAGS}")
endif()
