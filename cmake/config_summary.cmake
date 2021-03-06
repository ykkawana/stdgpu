function(stdgpu_print_configuration_summary)
    message(STATUS "")
    message(STATUS "************************ stdgpu Configuration Summary *************************")
    message(STATUS "")

    message(STATUS "General:")
    message(STATUS "  Version                                   :   ${stdgpu_VERSION}")
    message(STATUS "  System                                    :   ${CMAKE_SYSTEM_NAME}")
    message(STATUS "  Build type                                :   ${CMAKE_BUILD_TYPE}")

    message(STATUS "")

    message(STATUS "Build:")
    message(STATUS "  STDGPU_SETUP_COMPILER_FLAGS               :   ${STDGPU_SETUP_COMPILER_FLAGS} (depends on usage method)")

    message(STATUS "")

    message(STATUS "Configuration:")
    message(STATUS "  STDGPU_ENABLE_AUXILIARY_ARRAY_WARNING     :   ${STDGPU_ENABLE_AUXILIARY_ARRAY_WARNING}")
    message(STATUS "  STDGPU_ENABLE_CONTRACT_CHECKS             :   ${STDGPU_ENABLE_CONTRACT_CHECKS} (depends on build type)")
    message(STATUS "  STDGPU_ENABLE_MANAGED_ARRAY_WARNING       :   ${STDGPU_ENABLE_MANAGED_ARRAY_WARNING}")
    message(STATUS "  STDGPU_USE_32_BIT_INDEX                   :   ${STDGPU_USE_32_BIT_INDEX}")
    message(STATUS "  STDGPU_USE_FAST_DESTROY                   :   ${STDGPU_USE_FAST_DESTROY}")
    message(STATUS "  STDGPU_USE_FIBONACCI_HASHING              :   ${STDGPU_USE_FIBONACCI_HASHING}")

    message(STATUS "")

    message(STATUS "Tests:")
    message(STATUS "  STDGPU_BUILD_TESTS                        :   ${STDGPU_BUILD_TESTS}")

    message(STATUS "")

    message(STATUS "Examples:")
    message(STATUS "  STDGPU_BUILD_EXAMPLES                     :   ${STDGPU_BUILD_EXAMPLES}")

    message(STATUS "")

    message(STATUS "Documentation:")
    if(STDGPU_HAVE_DOXYGEN)
        message(STATUS "  Doxygen                                   :   YES")
    else()
        message(STATUS "  Doxygen                                   :   NO")
    endif()

    message(STATUS "")
    message(STATUS "*******************************************************************************")
    message(STATUS "")
endfunction()
