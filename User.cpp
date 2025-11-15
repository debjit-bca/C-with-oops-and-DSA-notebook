#include "User.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

User::User() : id(0), createdAt(time(nullptr)) {}

User::User(int id, const std::string& username, const std::string& email, const std::string& passwordHash)
    : id(id), username(username), email(email), passwordHash(passwordHash), createdAt(time(nullptr)) {}

std::string User::hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

bool User::validatePassword(const std::string& password) const {
    return hashPassword(password) == passwordHash;
}
