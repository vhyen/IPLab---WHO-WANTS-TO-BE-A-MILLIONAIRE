#ifndef EN_DE_MSG_H
#define EN_DE_MSG_H
#include <iostream>
#include "message.hpp"

#include <map>
#include <vector>
#include <tuple>
#include <string>

using namespace std;
// just for test, replace by the real question class in game, này có rồi
struct question
{
string content;
string a_ans;
string b_ans;
string c_ans;
string d_ans;
string correct_ans;
} ;
class encoder
{
public:
    class server
    {
    public:
        static message encode_ask_nickname_message();
        static message encode_nickname_ok_message();
        static message encode_game_start_message(int, vector<string>); // test en-de
        static message encode_question_message(question); // test en-de
        static message encode_points_message(map<string, int>); //test en-de
        static message encode_game_finish_message(vector<string>); // test en-de
        static message encode_one_player_result(string);
    };
    class client
    {
    public:
        static message encode_answer_nickname_message(string); // test en-de
        static message encode_answer_message(string);
    };
};

message encoder::server::encode_ask_nickname_message()
{
    return message(ASK_NICKNAME, "");
}

message encoder::server::encode_nickname_ok_message()
{
    return message(NICKNAME_OK, "");
}

message encoder::server::encode_game_start_message(int nums_of_question, vector<string> players)
{
    string body = to_string(nums_of_question);
    for (auto player : players)
    {
        body += SEPERATOR;
        body += player;
    }
    return message(GAME_START, body);
} 
message encoder::server::encode_question_message(question question)
{
    return message(QUESTION, question.content + SEPERATOR + question.a_ans + SEPERATOR + question.b_ans
                            + SEPERATOR + question.c_ans + SEPERATOR + question.d_ans );
}
message encoder::server::encode_points_message(map<string, int> points)
{
    string body;
    // for (const auto [name, point] : points)
    // {
    //     if (body.length() > 0)
    //         body += SEPERATOR;
    //     body += name;
    //     body += SEPERATOR;
    //     body += to_string(point);
    // }
    for (auto it = points.begin(); it != points.end(); ++it) 
    {
    string name = it->first;
    int point = it->second;
    if (body.length() > 0)
            body += SEPERATOR;
        body += name;
        body += SEPERATOR;
        body += to_string(point);
    }
    return message(RESULTS_OF_SET, body);
}

message encoder::server::encode_game_finish_message(vector<string> winners)
{
    string body = "";
    for (auto player : winners)
    {
        if (body.length() > 0)
            body += SEPERATOR;
        body += player;
    }
    return message(GAME_FINISH, body);
}
message encoder::server::encode_one_player_result(string result)
{
    string body = "";
    body += result;
    return message(YOU_WIN, body);
}


message encoder::client::encode_answer_nickname_message(string nickname)
{
    return message(ANSWER_NICKNAME, nickname);
}

message encoder::client::encode_answer_message(string ans)
{
    return message(ANSWER_QUESTION, ans);
}


class decoder
{
public:
    class server
    {
    public:
        static bool decode_nickname_answer_message(message, string &);
        static bool decode_answer_message(message, string &);
    };
    class client
    {
    public:
        static bool decode_game_start_message(message, int &, vector<string> &);
        static bool decode_question_message(message, question &);
        static bool decode_results_of_set(message, map<string, int> &);
        static bool decode_game_finish_message(message, vector<string> &);
        static bool decode_one_player_result (message,string &);
    };
};

bool decoder::client::decode_game_start_message(message msg, int &nums_of_question, vector<string> &players)
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

bool decoder::client::decode_question_message(message msg, question &question)
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
        question.content = content;
        question.a_ans = a_ans;
        question.b_ans = b_ans;
        question.c_ans = c_ans;
        question.d_ans = d_ans;

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