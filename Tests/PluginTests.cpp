#include <iostream>
#include "../UnityAnalyzer/Plugins/ModuleLoader.h"

void TestPluginLoading() {
    std::cout << "Running Plugin Loading Test..." << std::endl;
    UnityAnalyzer::Plugins::ModuleLoader::Instance().LoadPlugins("plugins");
}

int main() {
    TestPluginLoading();
    std::cout << "Plugin Tests Finished." << std::endl;
    return 0;
}
