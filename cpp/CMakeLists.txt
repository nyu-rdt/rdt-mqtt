cmake_minimum_required(VERSION 3.10)
add_executable(receiver receiver.cpp)
add_executable(sender sender.cpp)
add_executable(cmdsender cmdsender.cpp)

#find_library(paho-mqttpp3 NAMES libpaho-mqttpp3a.so REQUIRED)
#find_library(paho-mqtt3a NAME libpaho-mqtt3a.so REQUIRED)
#
#add_library(pp3 SHARED IMPORTED)
#set_property(TARGET pp3 PROPERTY IMPORTED_LOCATION ${paho-mqttpp3})
#
#add_library(tt3a SHARED IMPORTED)
#set_property(TARGET tt3a PROPERTY IMPORTED_LOCATION ${paho-mqtt3a})
#
#target_link_libraries(receiver -L/usr/local/lib/)
#target_link_libraries(receiver pp3)
#target_link_libraries(receiver tt3a)

target_link_libraries(receiver -L/usr/local/lib/)
target_link_libraries(receiver -lpaho-mqttpp3)
target_link_libraries(receiver -lpaho-mqtt3a)

target_link_libraries(sender -L/usr/local/lib/)
target_link_libraries(sender -lpaho-mqttpp3)
target_link_libraries(sender -lpaho-mqtt3a)

target_link_libraries(cmdsender -L/usr/local/lib/)
target_link_libraries(cmdsender -lpaho-mqttpp3)
target_link_libraries(cmdsender -lpaho-mqtt3a)
