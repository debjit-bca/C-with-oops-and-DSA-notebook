const API_URL = 'http://localhost:8080/api';
let currentNoteId = null;
let notes = [];

window.addEventListener('load', () => {
    const token = localStorage.getItem('token');
    const username = localStorage.getItem('username');
    
    if (!token) {
        window.location.href = 'index.html';
        return;
    }
    
    document.getElementById('userGreeting').textContent = `Welcome, ${username}!`;
    loadNotes();
    
    document.getElementById('noteContent').addEventListener('input', updateWordCount);
});

async function loadNotes() {
    try {
        const response = await fetch(`${API_URL}/notes`, {
            headers: {
                'Authorization': `Bearer ${localStorage.getItem('token')}`
            }
        });
        
        if (response.ok) {
            notes = await response.json();
            displayNotes();
        }
    } catch (error) {
        console.error('Error loading notes:', error);
    }
}

function displayNotes() {
    const notesList = document.getElementById('notesList');
    notesList.innerHTML = notes.map(note => `
        <div class="note-item ${note.id === currentNoteId ? 'active' : ''}" onclick="loadNote(${note.id})">
            <h3>${note.title || 'Untitled'}</h3>
            <p>${new Date(note.updated_at).toLocaleDateString()}</p>
        </div>
    `).join('');
}

function createNewNote() {
    currentNoteId = null;
    document.getElementById('noteTitle').value = '';
    document.getElementById('noteContent').value = '';
    updateWordCount();
}

async function loadNote(noteId) {
    currentNoteId = noteId;
    const note = notes.find(n => n.id === noteId);
    
    if (note) {
        document.getElementById('noteTitle').value = note.title;
        document.getElementById('noteContent').value = note.content;
        updateWordCount();
        displayNotes();
    }
}

async function saveNote() {
    const title = document.getElementById('noteTitle').value;
    const content = document.getElementById('noteContent').value;
    
    if (!title && !content) {
        alert('Please add a title or content');
        return;
    }
    
    try {
        const url = currentNoteId ? `${API_URL}/notes/${currentNoteId}` : `${API_URL}/notes`;
        const method = currentNoteId ? 'PUT' : 'POST';
        
        const response = await fetch(url, {
            method,
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${localStorage.getItem('token')}`
            },
            body: JSON.stringify({ title, content })
        });
        
        if (response.ok) {
            const savedNote = await response.json();
            currentNoteId = savedNote.id;
            document.getElementById('lastSaved').textContent = `Last saved: ${new Date().toLocaleTimeString()}`;
            loadNotes();
        }
    } catch (error) {
        console.error('Error saving note:', error);
        alert('Failed to save note');
    }
}

async function deleteNote() {
    if (!currentNoteId) {
        alert('No note selected');
        return;
    }
    
    if (!confirm('Are you sure you want to delete this note?')) {
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/notes/${currentNoteId}`, {
            method: 'DELETE',
            headers: {
                'Authorization': `Bearer ${localStorage.getItem('token')}`
            }
        });
        
        if (response.ok) {
            createNewNote();
            loadNotes();
        }
    } catch (error) {
        console.error('Error deleting note:', error);
    }
}

async function aiSummarize() {
    const content = document.getElementById('noteContent').value;
    
    if (!content) {
        alert('No content to summarize');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/ai/summarize`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${localStorage.getItem('token')}`
            },
            body: JSON.stringify({ text: content })
        });
        
        if (response.ok) {
            const data = await response.json();
            alert('AI Summary:\n\n' + data.summary);
        }
    } catch (error) {
        console.error('Error with AI:', error);
        alert('AI service unavailable');
    }
}

async function aiImprove() {
    const content = document.getElementById('noteContent').value;
    
    if (!content) {
        alert('No content to improve');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/ai/improve`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${localStorage.getItem('token')}`
            },
            body: JSON.stringify({ text: content })
        });
        
        if (response.ok) {
            const data = await response.json();
            if (confirm('Replace current content with improved version?')) {
                document.getElementById('noteContent').value = data.improved_text;
                updateWordCount();
            }
        }
    } catch (error) {
        console.error('Error with AI:', error);
        alert('AI service unavailable');
    }
}

function updateWordCount() {
    const content = document.getElementById('noteContent').value;
    const words = content.trim() ? content.trim().split(/\s+/).length : 0;
    document.getElementById('wordCount').textContent = `Words: ${words}`;
}

function logout() {
    localStorage.removeItem('token');
    localStorage.removeItem('username');
    window.location.href = 'index.html';
}
