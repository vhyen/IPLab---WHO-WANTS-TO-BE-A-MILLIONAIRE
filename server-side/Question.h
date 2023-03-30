#include <stdlib.h>

struct Question {
    public:
    std::string question;
    std::vector<std::string> choices;
    std::string answer;

    Question();
    Question(std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD, std::string answer);
};