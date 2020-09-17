if(Any_ROOT)
    set(Any_INCLUDE_DIRS ${Any_ROOT}/include ${Any_ROOT}/include/Any)
    set(Any_FOUND TRUE)
    if(NOT TARGET Any::Any)
      add_library(Any_header_only INTERFACE)
      add_library(Any::Any ALIAS Any_header_only)
      set_target_properties(Any_header_only PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Any_INCLUDE_DIRS}"
      )
    endif()
endif()
