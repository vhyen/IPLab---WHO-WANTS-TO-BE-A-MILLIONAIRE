#ifndef QuestionManager_hpp
#define QuestionManager_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Question.hpp"

class QuestionManager {
    private:
    static QuestionManager* instance;
    std::vector<Question> questDB;
    QuestionManager();

    public:

    QuestionManager(QuestionManager& obj) = delete;

    static QuestionManager* getInstance();
    int retrieveQuestionsFromFile(std::string filename);
    std::vector<Question> generateRandomQuestion(int n);
};

#endif /* QuestionManager_hpp */
