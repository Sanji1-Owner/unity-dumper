add_executable(ParserTests ParserTests.cpp)
target_link_libraries(ParserTests PRIVATE UnityAnalyzer)
add_test(NAME ParserTests COMMAND ParserTests)

add_executable(PluginTests PluginTests.cpp)
target_link_libraries(PluginTests PRIVATE UnityAnalyzer)
add_test(NAME PluginTests COMMAND PluginTests)
