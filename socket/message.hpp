#ifndef MESSAGE_H
#define MESSAGE_H
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std; 


typedef char message_header;
const message_header ASK_NICKNAME = 0;
const message_header NICKNAME_OK = 1;
const message_header ANSWER_NICKNAME = 2;
const message_header ANSWER_QUESTION = 3;
const message_header QUESTION = 4;
const message_header GAME_START = 5;
const message_header GAME_FINISH = 6;
const message_header RESULTS_OF_SET = 7;
const message_header YOU_WIN = 8;

const char END_OF_MSG = '\n';
const char SEPERATOR = '|';

class message
{
public:
    enum
    {
        header_length = 1 
    };
    enum
    {
        max_body_length = 199
    };
    enum
    {
        max_length = header_length + max_body_length
    };
    message() : body_length_(0) {}

    message(message_header header, std::string msg)
    {
        for (int i = 0; i < max_length; i++)
        {
            data_[i] = END_OF_MSG;
        }
        data_[0] = header;
        body_length_ = msg.size();
        if (body_length_ > max_body_length)
        {
            body_length_ = max_body_length;
        }

        for (int i = 0; i < body_length_; i++)
        {
            data_[i + 1] = msg[i];
        }
    }
    char *data()
    {
        return data_;
    }
    std::size_t length() const
    {
        return max_length;
    }

    /* return message header */
    message_header header()
    {
        return data_[0];
    }
    /*return body string*/
    string body()
    {
        string body = "";
        for (int i = 1; i < max_length && data_[i] != END_OF_MSG; i++)
        {
            body += data_[i];
        }
        return body;
    }

private:
    char data_[max_length];
    size_t body_length_;
};

#endif