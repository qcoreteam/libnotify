add_custom_target(UnitTests)
set_target_properties(UnitTests PROPERTIES FOLDER "UnitTests")
set(TEST_SRCS
    EventMgrTest.cpp
    FlagsTest.cpp
    VersionTest.cpp
    thread/AtomicIntegerTest.cpp)
notify_add_unittest(UnitTests KernelTest ${TEST_SRCS})