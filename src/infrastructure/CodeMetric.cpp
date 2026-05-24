//
// Created by hseyi on 1.05.2026.
//

#include "CodeMetric.h"

CodeMetric::CodeMetric(const std::string& name, double value,const std::string& unit,const std::string& description) {
    this->name = name;
    this->value = value;
    this->unit = unit;
    this->description = description;
}

std::string CodeMetric::getName() const {
    return name;
}

double CodeMetric::getValue() const {
    return value;
}

std::string CodeMetric::getUnit() const {
    return unit;
}

std::string CodeMetric::getDescription() const {
    return description;

}
