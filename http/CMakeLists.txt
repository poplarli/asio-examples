aux_source_directory(. DIR_SRCS)
ADD_EXECUTABLE(http_server ${DIR_SRCS})
TARGET_LINK_LIBRARIES(http_server boost_system pthread boost_thread)

