//
// Created by hseyi on 1.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_CODEMETRIC_H
#define SE_TEAMPROJECT_2026_CODEMETRIC_H

#include <string>


class CodeMetric {
private:
    std::string name;
    double value;
    std::string unit;
    std::string description;

public:
    CodeMetric(const std::string& name, double value,const std::string& unit,const std::string& description);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] double getValue() const;
    [[nodiscard]] std::string getUnit() const;
    [[nodiscard]] std::string getDescription() const;
};


#endif //SE_TEAMPROJECT_2026_CODEMETRIC_H

