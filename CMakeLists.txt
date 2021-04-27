cmake_minimum_required(VERSION 3.10)

project(structure)

# Standard set to 17 to have mandatory RVO
set(CMAKE_CXX_STANDARD 17)


# EXECUTABLE SECTION
add_executable(mainEntry main.cpp)
target_include_directories(mainEntry PRIVATE include)
# Class A is not a library: its source file needs to be included in the target sources
target_sources(mainEntry PRIVATE src/LBAMTTdevice.cpp
                                 src/LBAMTTcadSVG.cpp
                                 )
target_link_libraries(mainEntry)

# TESTS SECTION
# Basically makes CMake read the CMakeLists.txt file contained in tests
add_subdirectory(tests)


# Explain what the PRIVATE, INTERFACE, PUBLIC keyword all mean
#[=[
   - When A links in B as *PRIVATE*, it is saying that A uses B in its
   implementation, but B is not used in any part of A's public API. Any code
   that makes calls into A would not need to refer directly to anything from
   B. An example of this could be a networking library A which can be built to
   use one of a number of different SSL libraries internally (which B
   represents). A presents a unified interface for client code which does not
   reference any of the internal SSL data structures or functions. Client code
   would have no idea what SSL implementation (B) is being used by A, nor does
   that client code need to care.
   - When A links in B as *INTERFACE*, it is saying that A does not use B
   in its implementation, but B is used in A's public API. Code that calls
   into A may need to refer to things from B in order to make such calls. One
   example of this is an interface library which simply forwards calls along
   to another library but doesn't actually reference the objects on the way
   through other than by a pointer or reference. Another example is where A is
   defined in CMake as an interface library, meaning it has no actual
   implementation itself, it is effectively just a collection of other
   libraries (I'm probably over-simplifying here, but you get the picture).
   - When A links in B as *PUBLIC*, it is essentially a combination of
   PRIVATE and INTERFACE. It says that A uses B in its implementation and B is
   also used in A's public API.
   - When A links in B as *PRIVATE*, it is saying that A uses B in its
   implementation, but B is not used in any part of A's public API. Any code
   that makes calls into A would not need to refer directly to anything from
   B. An example of this could be a networking library A which can be built to
   use one of a number of different SSL libraries internally (which B
   represents). A presents a unified interface for client code which does not
   reference any of the internal SSL data structures or functions. Client code
   would have no idea what SSL implementation (B) is being used by A, nor does
   that client code need to care.
   - When A links in B as *INTERFACE*, it is saying that A does not use B
   in its implementation, but B is used in A's public API. Code that calls
   into A may need to refer to things from B in order to make such calls. One
   example of this is an interface library which simply forwards calls along
   to another library but doesn't actually reference the objects on the way
   through other than by a pointer or reference. Another example is where A is
   defined in CMake as an interface library, meaning it has no actual
   implementation itself, it is effectively just a collection of other
   libraries (I'm probably over-simplifying here, but you get the picture).
   - When A links in B as *PUBLIC*, it is essentially a combination of
   PRIVATE and INTERFACE. It says that A uses B in its implementation and B is
   also used in A's public API
#]=]
