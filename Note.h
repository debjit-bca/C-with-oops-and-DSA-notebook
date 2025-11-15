#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <ctime>

class Note {
private:
    int id;
    int userId;
    std::string title;
    std::string content;
    time_t createdAt;
    time_t updatedAt;
    Note* next;

public:
    Note();
    Note(int id, int userId, const std::string& title, const std::string& content);
    
    int getId() const { return id; }
    int getUserId() const { return userId; }
    std::string getTitle() const { return title; }
    std::string getContent() const { return content; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }
    Note* getNext() const { return next; }
    
    void setId(int newId) { id = newId; }
    void setUserId(int newUserId) { userId = newUserId; }
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setContent(const std::string& newContent) { content = newContent; updatedAt = time(nullptr); }
    void setNext(Note* nextNote) { next = nextNote; }
    
    void update(const std::string& newTitle, const std::string& newContent);
};

#endif
