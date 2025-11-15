#include "Note.h"

Note::Note() : id(0), userId(0), createdAt(time(nullptr)), updatedAt(time(nullptr)), next(nullptr) {}

Note::Note(int id, int userId, const std::string& title, const std::string& content)
    : id(id), userId(userId), title(title), content(content), 
      createdAt(time(nullptr)), updatedAt(time(nullptr)), next(nullptr) {}

void Note::update(const std::string& newTitle, const std::string& newContent) {
    title = newTitle;
    content = newContent;
    updatedAt = time(nullptr);
}
