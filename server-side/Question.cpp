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