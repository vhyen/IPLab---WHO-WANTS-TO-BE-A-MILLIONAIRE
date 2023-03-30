#include <Player.h>

Player::Player() {

}

Player::Player(std::string username) {
    this->username = username;
    isQualified = true;
    haveMove = true;
}