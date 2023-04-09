#ifndef EN_DE_MSG_H
#define EN_DE_MSG_H
#include <iostream>

#include <map>
#include <vector>
#include <tuple>
#include <string>

#include "message.hpp"
#include "Question.hpp"

using namespace std;



message encode_answer_nickname_message(string nickname)
{
    return message(ANSWER_NICKNAME, nickname);
}

message encode_answer_message(string ans)
{
    return message(ANSWER_QUESTION, ans);
}


bool decode_game_start_message(message msg, int &nums_of_question, vector<string> &players)
{
    players.clear();
    if (msg.header() != GAME_START)
    {
        return false;
    }
    try
    {
        int idx = 1;
        string len = "";
        for (; idx < msg.length() && msg.data()[idx] != SEPERATOR && msg.data()[idx] != END_OF_MSG; idx++)
        {
            len += msg.data()[idx];
        }
        nums_of_question = stoi(len);
        idx += 1;
        string cur;
        for (; idx < msg.length() && msg.data()[idx] != END_OF_MSG; idx++)
        {
            if (msg.data()[idx] == SEPERATOR)
            {
                players.push_back(cur);
                cur = "";
            }
            else
            {
                cur += msg.data()[idx];
            }
        }
        if (cur.length() > 0)
            players.push_back(cur);
    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}

bool decode_question_message(message msg, Question &question)
{
    if (msg.header() != QUESTION)
    {
        return false;
    }
    try
    {
        string content = "";
        string a_ans = "";
        string b_ans = "";
        string c_ans = "";
        string d_ans = "";
        int idx = 1;
        string operand1 = "";
        for (; idx < msg.length() && msg.data()[idx] != SEPERATOR && msg.data()[idx] != END_OF_MSG; idx++)
        {
            content += msg.data()[idx];
        }
        idx++;
        for (; idx < msg.length() && msg.data()[idx] != SEPERATOR && msg.data()[idx] != END_OF_MSG; idx++)
        {
            a_ans += msg.data()[idx];
        }
        idx++;
        for (; idx < msg.length() && msg.data()[idx] != SEPERATOR && msg.data()[idx] != END_OF_MSG; idx++)
        {
            b_ans += msg.data()[idx];
        }
        idx++;
        for (; idx < msg.length() && msg.data()[idx] != SEPERATOR && msg.data()[idx] != END_OF_MSG; idx++)
        {
            c_ans += msg.data()[idx];
        }
        idx++;
        for (; idx < msg.length() && msg.data()[idx] != SEPERATOR && msg.data()[idx] != END_OF_MSG; idx++)
        {
            d_ans += msg.data()[idx];
        }
        question.question = content;
        question.optionA= a_ans;
        question.optionB = b_ans;
        question.optionC = c_ans;
        question.optionD = d_ans;

    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}

bool decoder::client::decode_results_of_set(message msg, map<string, int> &points)
{
    points.clear();
    if (msg.header() != RESULTS_OF_SET)
    {
        return false;
    }
    try
    {
        vector<string> temp;
        string cur;
        int idx = 1;
        for (; idx < msg.length() && msg.data()[idx] != END_OF_MSG; idx++)
        {
            if (msg.data()[idx] == SEPERATOR)
            {
                temp.push_back(cur);
                cur = "";
            }
            else
            {
                cur += msg.data()[idx];
            }
        }
        if (cur.length() > 0)
            temp.push_back(cur);
        // assert(temp.size() % 2 == 0);
        for (int i = 0; i < (int)temp.size(); i += 2)
        {
            points[temp[i]] = stoi(temp[i + 1]);
        }
    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}

bool decoder::client::decode_game_finish_message(message msg, vector<string> &winners)
{
    winners.clear();
    if (msg.header() != GAME_FINISH)
    {
        return false;
    }
    try
    {
        string cur;
        int idx = 1;
        for (; idx < msg.length() && msg.data()[idx] != END_OF_MSG; idx++)
        {
            if (msg.data()[idx] == SEPERATOR)
            {
                winners.push_back(cur);
                cur = "";
            }
            else
            {
                cur += msg.data()[idx];
            }
        }
        if (cur.length() > 0)
        {
            winners.push_back(cur);
        }
    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}

bool decoder::server::decode_nickname_answer_message(message msg, string &nickname)
{
    if (msg.header() != ANSWER_NICKNAME)
    {
        return false;
    }
    try
    {
        nickname = "";
        for (int i = 1; i < msg.length() && msg.data()[i] != END_OF_MSG; i++)
        {
            nickname += msg.data()[i];
        }
    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}

bool decoder::server::decode_answer_message(message msg, string &ans)
{
    ans="";
    if (msg.header() != ANSWER_QUESTION)
    {
        return false;
    }
    try
    {
        ans += msg.data()[1];
    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}
bool decoder::client::decode_one_player_result (message msg,string &result)
{
    
    if (msg.header() != YOU_WIN)
    {
        return false;
    }
    try
    {
        result ="";
        result += msg.data()[1];
    }
    catch (exception &e)
    {
        cerr << "Error decoding message " << e.what() << endl;
        return false;
    }
    return true;
}


#endif




// struct question
// {
// string content;
// string a_ans;
// string b_ans;
// string c_ans;
// string d_ans;
// string correct_ans;
// } ;






