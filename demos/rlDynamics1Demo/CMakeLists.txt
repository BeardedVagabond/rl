find_package(Boost REQUIRED)

add_executable(
	rlDynamics1Demo
	rlDynamics1Demo.cpp
	${rl_BINARY_DIR}/robotics-library.rc
)

target_include_directories(
	rlDynamics1Demo
	PUBLIC
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(
	rlDynamics1Demo
	mdl
)
