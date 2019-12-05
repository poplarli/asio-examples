
ADD_EXECUTABLE(timer1 timer1.cpp)
TARGET_LINK_LIBRARIES(timer1 boost_system pthread boost_thread)

ADD_EXECUTABLE(daytimecli daytimecli.cpp)
TARGET_LINK_LIBRARIES(daytimecli boost_system pthread boost_thread)

ADD_EXECUTABLE(daytimesrv daytimesrv.cpp)
TARGET_LINK_LIBRARIES(daytimesrv boost_system pthread boost_thread)

ADD_EXECUTABLE(echo_server echo_server.cpp)
TARGET_LINK_LIBRARIES(echo_server boost_system pthread boost_thread)

ADD_EXECUTABLE(echo_client echo_client.cpp)
TARGET_LINK_LIBRARIES(echo_client boost_system pthread boost_thread)

ADD_EXECUTABLE(aync_daytimesrc aync_daytimesrc.cpp)
TARGET_LINK_LIBRARIES(aync_daytimesrc boost_system pthread boost_thread)

