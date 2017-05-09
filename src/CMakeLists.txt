set(NOTIFY_SRC
      kernel/Global.cpp
      EventLoop.cpp
      eventmgr/EventMgr.cpp
      thread/Atomic.cpp
      utils/Version.cpp)

notify_add_library(${CMAKE_PROJECT_NAME} SHARED ${NOTIFY_SRC} ${NOTIFY_HEADERS})
