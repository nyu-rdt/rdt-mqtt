add_executable(buildtest receiver.cpp)

target_link_libraries(buildtest -L/usr/local/lib/)
target_link_libraries(buildtest -lpaho-mqttpp3)
target_link_libraries(buildtest -lpaho-mqtt3a)
