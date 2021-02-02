
# is this interface library defined yet?

if (NOT TARGET sparkfun_qwiic_rfid)
    add_library(sparkfun_qwiic_rfid INTERFACE)

    # library source files
    target_sources(sparkfun_qwiic_rfid INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/qwiic_rfid.cpp
    )

    # Where is this include files for this library. Note, this include directory uses
    # the directory structure in the include directory to set a "sparkfun/<library>"
    # pattern for the sparkfun sdk.
    target_include_directories(sparkfun_qwiic_rfid INTERFACE ${CMAKE_CURRENT_LIST_DIR})

    # libraries this library depends on. 
    target_link_libraries(sparkfun_qwiic_rfid INTERFACE
        sparkfun_qwiic_core
    	pico_stdlib
    	hardware_i2c
    )

endif()