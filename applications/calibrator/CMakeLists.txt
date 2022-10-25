cmake_minimum_required (VERSION 3.8)


if (QT_VERSION)
	add_subdirectory(localTransform)
endif()

add_subdirectory(timestampShifter)
