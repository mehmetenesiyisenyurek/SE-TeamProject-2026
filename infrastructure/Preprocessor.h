//
// Created by hseyi on 8.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_PREPROCESSOR_H
#define SE_TEAMPROJECT_2026_PREPROCESSOR_H

#include <string>

class Preprocessor {
private:
    std::string rawCode;
    std::string cleanedCode;
    int commentCount;
public:
    //Constructor
    Preprocessor();

    std::string process(const std::string& code);
    [[nodiscard]] int getCommentLineCount() const;
    std::string removeSingleLineComments(const std::string& code);
    std::string removeMultiLineComments(const std::string& code);
    bool isInsideStringLiteral(const std::string& code, int position);

};


#endif //SE_TEAMPROJECT_2026_PREPROCESSOR_H
