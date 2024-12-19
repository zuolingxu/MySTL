cmake_minimum_required(VERSION 3.29)

project(MySTL)

set(CMAKE_CXX_STANDARD 20)

add_executable(MySTL
		MemoryPool.h
		MyList.h
		MyString.h
		MyStack.h
		MyDeque.h
		MyBinaryTree.h
		STLTest.cpp
		MyVector.h
		MySort.h
		SurfVector.h
		Matrix.h
		MyGraph.h
)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
