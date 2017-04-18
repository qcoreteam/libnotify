
set(NOTIFY_SRC
    EventLoop.cpp
    EventMgr.cpp
    Version.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED ${NOTIFY_SRC})