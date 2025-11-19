include(FetchContent)

if(BUILD_TESTING)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
  )
  FetchContent_MakeAvailable(googletest)

  set_property(GLOBAL PROPERTY ENVIRONMENT "GTEST_COLOR=yes")

endif()