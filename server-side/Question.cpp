#include "Question.hpp"
#include <string>

Question::Question() {
    question = "";
    answer = "";
};

Question::Question(std::string quest, std::string optionA, std::string optionB, std::string optionC, std::string optionD, std::string ans) {
    question = quest;
    options.push_back(optionA);
    options.push_back(optionB);
    options.push_back(optionC);
    options.push_back(optionD);
    answer = ans;
};


void Question::toMessageBody(char str[]) {
    std::string message = "";
    message += question;
    for (std::string each : options) {
        message += "|";
        message += each;
    }
    strcpy(str, message.c_str());
}