if(Any_ROOT)
    set(Any_INCLUDE_DIRS ${Any_ROOT}/include ${Any_ROOT}/include/any)
    set(Any_FOUND TRUE)
    if(NOT TARGET Any::Any)
      add_library(Any_header_only INTERFACE)
      add_library(Any::Any ALIAS Any_header_only)
      set_target_properties(Any_header_only PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Any_INCLUDE_DIRS}"
      )
      set(Any_VERSION "1.0")
      include(FindPackageHandleStandardArgs)
      find_package_handle_standard_args(Any
        FOUND_VAR Any_FOUND
        REQUIRED_VARS Any_INCLUDE_DIRS
        VERSION_VAR Any_VERSION
      )
    set(Any_VERSION_STRING ${Any_VERSION})
    endif()
endif()
