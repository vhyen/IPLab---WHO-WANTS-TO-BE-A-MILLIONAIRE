#ifndef Question_hpp
#define Question_hpp

#include <stdio.h>
#include <string>
#include <vector>


class Question {
    public:
    std::string question;
    std::vector<std::string> options;
    std::string answer;

    Question();
    Question(std::string quest, std::string optionA, std::string optionB, std::string optionC, std::string optionD, std::string ans);
};

#endif /* Question_hpp */