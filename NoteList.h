#ifndef NOTELIST_H
#define NOTELIST_H

#include "Note.h"
#include <vector>

class NoteList {
private:
    Note* head;
    int size;

public:
    NoteList();
    ~NoteList();
    
    void addNote(Note* note);
    Note* findNoteById(int id);
    bool deleteNoteById(int id);
    std::vector<Note*> getAllNotes();
    std::vector<Note*> getNotesByUserId(int userId);
    int getSize() const { return size; }
    
    void clear();
};

#endif
