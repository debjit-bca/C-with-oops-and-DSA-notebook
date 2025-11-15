#include "NoteList.h"

NoteList::NoteList() : head(nullptr), size(0) {}

NoteList::~NoteList() {
    clear();
}

void NoteList::addNote(Note* note) {
    if (!note) return;
    
    note->setNext(head);
    head = note;
    size++;
}

Note* NoteList::findNoteById(int id) {
    Note* current = head;
    while (current) {
        if (current->getId() == id) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

bool NoteList::deleteNoteById(int id) {
    if (!head) return false;
    
    if (head->getId() == id) {
        Note* temp = head;
        head = head->getNext();
        delete temp;
        size--;
        return true;
    }
    
    Note* current = head;
    while (current->getNext()) {
        if (current->getNext()->getId() == id) {
            Note* temp = current->getNext();
            current->setNext(temp->getNext());
            delete temp;
            size--;
            return true;
        }
        current = current->getNext();
    }
    return false;
}

std::vector<Note*> NoteList::getAllNotes() {
    std::vector<Note*> notes;
    Note* current = head;
    while (current) {
        notes.push_back(current);
        current = current->getNext();
    }
    return notes;
}

std::vector<Note*> NoteList::getNotesByUserId(int userId) {
    std::vector<Note*> userNotes;
    Note* current = head;
    while (current) {
        if (current->getUserId() == userId) {
            userNotes.push_back(current);
        }
        current = current->getNext();
    }
    return userNotes;
}

void NoteList::clear() {
    while (head) {
        Note* temp = head;
        head = head->getNext();
        delete temp;
    }
    size = 0;
}
