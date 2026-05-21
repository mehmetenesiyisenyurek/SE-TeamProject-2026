//
// Created by hseyi on 17.05.2026.
//

#include "Application.h"

#include "../analysis_engine/AnalysisEngine.h"

#include <iostream>

Application::Application() : engine(nullptr){

}

Application::~Application() {
    shutdown();
}

void Application::run() {
    if (engine == nullptr) {
        engine =new AnalysisEngine();
    }

    std::cout << "CodeSentinel uygulamasi bastatildi." << std::endl;

}

void Application::shutdown() {
    if (engine != nullptr) {
        delete engine;
        engine = nullptr;
    }

    std::cout << "CodeSentinel uygulamasi kapatildi." << std::endl;
}
