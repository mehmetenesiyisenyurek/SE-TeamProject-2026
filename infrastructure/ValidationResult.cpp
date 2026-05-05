//
// Created by hseyi on 2.05.2026.
//

#include "ValidationResult.h"

ValidationResult::ValidationResult(bool valid,const std::string& errorMessage) {
    this->valid = valid;
    this->errorMessage = errorMessage;
}

bool ValidationResult::isValid() const {
    return valid;
}

std::string ValidationResult::getErrorMessage() const {
    return errorMessage;
}
