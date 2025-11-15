#ifndef USER_H
#define USER_H

#include <string>
#include <ctime>

class User {
private:
    int id;
    std::string username;
    std::string email;
    std::string passwordHash;
    time_t createdAt;

public:
    User();
    User(int id, const std::string& username, const std::string& email, const std::string& passwordHash);
    
    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getEmail() const { return email; }
    std::string getPasswordHash() const { return passwordHash; }
    time_t getCreatedAt() const { return createdAt; }
    
    void setId(int newId) { id = newId; }
    void setUsername(const std::string& newUsername) { username = newUsername; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPasswordHash(const std::string& newHash) { passwordHash = newHash; }
    
    bool validatePassword(const std::string& password) const;
    static std::string hashPassword(const std::string& password);
};

#endif
