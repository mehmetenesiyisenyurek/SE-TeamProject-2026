//
// Created by hseyi on 2.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_VALIDATIONRESULT_H
#define SE_TEAMPROJECT_2026_VALIDATIONRESULT_H

#include <string>


class ValidationResult {
private:
    bool valid;
    std::string errorMessage;


public:
    ValidationResult(bool valid,const std::string& errorMessage);

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] std::string getErrorMessage() const;
};


#endif //SE_TEAMPROJECT_2026_VALIDATIONRESULT_H
