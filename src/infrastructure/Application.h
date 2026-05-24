//
// Created by hseyi on 17.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_APPLICATION_H
#define SE_TEAMPROJECT_2026_APPLICATION_H

class AnalysisEngine;

class Application {
private:
    AnalysisEngine* engine;

public:
    Application();
    ~Application();

    void run();
    void shutdown();
};


#endif //SE_TEAMPROJECT_2026_APPLICATION_H

