#include "Player.hpp"


Player::Player() {
    username = "skyvalue";
    isQualified = true;
    haveMove = true;
}

Player::Player(std::string username) {
    this->username = username;
    isQualified = true;
    haveMove = true;
}
