#pragma once

#include <string>

// Hata seviyelerini sınıflandıran arayüz
class SeverityClassifier {
public:
    virtual ~SeverityClassifier() = default;
    virtual std::string classify(int severityLevel) = 0;
};
