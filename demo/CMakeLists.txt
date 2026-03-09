add_executable(onion_threadpool_demo
    "threadpool_demo.cpp"
)

target_link_libraries(onion_threadpool_demo
    PRIVATE
        onion::threadpool
)

target_compile_features(onion_threadpool_demo PRIVATE cxx_std_20)

set_target_properties(onion_threadpool_demo PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
