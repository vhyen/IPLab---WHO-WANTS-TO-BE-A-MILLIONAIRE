#include "QuestionManager.hpp"

QuestionManager::QuestionManager() {

}

QuestionManager* QuestionManager::getInstance() {
    if (instance == nullptr) 
        instance = new QuestionManager();
    return instance;
}


// lấy quest từ datafile lưu vào questDB
int QuestionManager::retrieveQuestionsFromFile(std::string filename) {
    std::cout << "Question Mgr retrieve questions\n";

    // std::string filename = "questions.dat";
    std::vector<std::string> questData;
    std::string line;

    // get random questions from files
    std::fstream file(filename, std::ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            questData.push_back(line);
        }
    }
    else
        std::cout << "Could not open the file\n";
    file.close();

    int i = 0;
    while (i < questData.size()) {
        Question question = Question();
        question.question = questData[i];
        for (int k = 1; k < 5; k++) 
            question.options.push_back(questData[i+k]);
        question.answer = questData[i+5];
        questDB.push_back(question);
        i += 6;
    }

    // test
    // std::cout << "Questions retrieved: \n";
    // for (Question each : questDB) {
    //     std::cout << each.question << std::endl;
    // } 
}

    // random number of questions
    // int num = int(players.size());



std::vector<Question> QuestionManager::generateRandomQuestion(int n) {
    srand(static_cast<unsigned int>(time(0)));
    int numQuest = rand() % n + n * 2;
    std::cout << "number of questions: " << numQuest << std::endl;
    std::vector<Question> list;
    list.assign(questDB.begin(), questDB.end());
    std::random_shuffle(list.begin(), list.end());   
    if (numQuest < list.size()) list.resize(numQuest);
    return list;
}