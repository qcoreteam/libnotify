set(NOTIFY_SRC
    EventLoop.cpp
    eventmgr/EventMgr.cpp
    utils/Version.cpp)

notify_add_library(${CMAKE_PROJECT_NAME} SHARED ${NOTIFY_SRC} ${NOTIFY_HEADERS})
