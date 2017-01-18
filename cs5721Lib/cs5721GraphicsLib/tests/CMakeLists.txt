include_directories(${CMAKE_SOURCE_DIR}/src)

IF(${Boost_THREAD_FOUND})
  ADD_EXECUTABLE (boostThreadExample
    boostThreadExample.cpp
    )

  TARGET_LINK_LIBRARIES(boostThreadExample sive-rtutil)
  TARGET_LINK_LIBRARIES(boostThreadExample ${Boost_THREAD_LIBRARIES})
  TARGET_LINK_LIBRARIES(boostThreadExample ${Boost_SYSTEM_LIBRARIES})
  TARGET_LINK_LIBRARIES(boostThreadExample pthread)
ENDIF(${Boost_THREAD_FOUND})

# ADD_EXECUTABLE (cpp11ThreadExample
#  cpp11ThreadExample.cpp
#  )
# Need to add -std=c++0x
# SET(CMAKE_CXX_FLAGS "-std=c++11")
# TARGET_LINK_LIBRARIES(cpp11ThreadExample cs5721Graphics)

add_executable(test_Timer
  test_Timer.cpp
)
target_link_libraries(test_Timer sive-rtutil)
target_link_libraries(test_Timer ${Boost_TIMER_LIBRARIES})
target_link_libraries(test_Timer ${Boost_CHRONO_LIBRARIES})
target_link_libraries(test_Timer ${Boost_SYSTEM_LIBRARIES})
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   target_link_libraries(test_Timer rt)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
IF(WIN32)
   target_link_libraries(test_Timer winmm.lib)
ENDIF(WIN32)



add_executable(test_pngWrite
  test_pngWrite.cpp
)
target_link_libraries(test_pngWrite sive-rtutil)
target_link_libraries(test_pngWrite ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_pngWrite ${PNG_LIBRARY})
target_link_libraries(test_pngWrite ${ZLIB_LIBRARY})

add_executable(test_readPNGFile
  test_readPNGFile.cpp
)
target_link_libraries(test_readPNGFile sive-rtutil)
target_link_libraries(test_readPNGFile ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_readPNGFile ${PNG_LIBRARY})
target_link_libraries(test_readPNGFile ${ZLIB_LIBRARY})

add_executable(test_fishEyeGen
  test_fishEyeGen.cpp
)
target_link_libraries(test_fishEyeGen sive-rtutil)
target_link_libraries(test_fishEyeGen ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_fishEyeGen ${PNG_LIBRARY})
target_link_libraries(test_fishEyeGen ${ZLIB_LIBRARY})

add_executable(test_Vector3D
  test_Vector3D.cpp
)
target_link_libraries(test_Vector3D sive-rtutil)

add_executable(test_XMLSceneParse
  test_XMLSceneParse.cpp
)
target_link_libraries(test_XMLSceneParse sive-rtutil)
target_link_libraries(test_XMLSceneParse ${Boost_PROGRAM_OPTIONS_LIBRARIES})
