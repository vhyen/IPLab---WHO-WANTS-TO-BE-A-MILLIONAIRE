// test for encode decode in case u want to see how to use 

// #include <iostream>
// #include "message.h"
// #include "en_de_msg.h"
// using namespace std;
// int main()
// {
//     // cout << "Enter a question: ";
//     // question Q;
//     // getline(cin, Q.content);
//     // getline(cin, Q.a_ans);
//     // getline(cin, Q.b_ans);
//     // getline(cin, Q.c_ans);
//     // getline(cin, Q.d_ans);

//     // map<string, int> points =
//     //     {
//     //         {"Alice", 10},
//     //         {"Bob", 20},
//     //         {"Charlie", 15},
//     //         {"David", 5},
//     //         {"Eve", 30}};

//     // vector<string> players = {"Alice", "Bob", "Charlie", "David", "Eve"};

//     string name = "nguyen hoai nam phuong";






//     message msg;
//     // msg = encoder::server::encode_question_message(Q);
//     msg = encoder::client::encode_answer_message("5");
//     // Display the encoded message
//     cout << "Encoded message: ";
//     for (int i = 0; i < msg.length(); i++)
//     {
//         cout << msg.data()[i];
//     }
//     cout << endl;

//     // Decode the message by creating a new message object from the encoded char array
//     // question q;
//     bool check;
//     int nums;
//     // check = decoder::client::decode_question_message(msg,q);
//     // cout << q.content <<endl;
//     // cout << q.a_ans <<endl;
//     // cout << q.b_ans <<endl;
//     // cout << q.c_ans <<endl;
//     // cout << q.d_ans <<endl;

// //     map<string, int> p;
// //     check = decoder::client::decode_results_of_set(msg,p);
// //     for (auto it = points.begin(); it != points.end(); ++it) {
// //     cout << it->first << " => " << it->second << endl;
// //   }
//     int a;
//     string n;
//     vector<string> p;
//     check = decoder::server::decode_answer_message(msg,n);
//     // cout << nums <<endl;
//     // for (int i =0;i<n.size();i++)
//     // {
//     //         cout << n[i] << endl;
//     // }
//     cout << n <<endl;
//     return 0;
// }
