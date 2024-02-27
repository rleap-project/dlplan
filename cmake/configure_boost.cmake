
macro(configure_boost)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(BOOST_MIN_VERSION "1.74.0")

    if (DEFINED ENV{BOOST_ROOT})
        set(Boost_NO_SYSTEM_PATHS ON)
        set(BOOST_ROOT $ENV{BOOST_ROOT})
    endif()
endmacro()
