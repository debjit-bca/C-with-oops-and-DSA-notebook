#include <httplib.h>
#include <nlohmann/json.hpp>
#include "User.h"
#include "Note.h"
#include "HashMap.h"
#include "NoteList.h"
#include <iostream>
#include <fstream>
#include <jwt-cpp/jwt.h>

using json = nlohmann::json;
using namespace httplib;

HashMap<std::string, User*> userDatabase;
NoteList noteDatabase;
int nextUserId = 1;
int nextNoteId = 1;

const std::string JWT_SECRET = "your_secret_key_here_change_in_production";

std::string generateToken(const std::string& username) {
    auto token = jwt::create()
        .set_issuer("digital-notebook")
        .set_type("JWT")
        .set_payload_claim("username", jwt::claim(username))
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours{24})
        .sign(jwt::algorithm::hs256{JWT_SECRET});
    return token;
}

std::string verifyToken(const std::string& token) {
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{JWT_SECRET})
            .with_issuer("digital-notebook");
        verifier.verify(decoded);
        return decoded.get_payload_claim("username").as_string();
    } catch (const std::exception& e) {
        return "";
    }
}

void setupCORS(Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

int main() {
    Server svr;
    
    svr.Options("/(.*)", [](const Request& req, Response& res) {
        setupCORS(res);
        res.set_content("", "text/plain");
    });
    
    svr.Post("/api/register", [](const Request& req, Response& res) {
        setupCORS(res);
        json data = json::parse(req.body);
        
        std::string username = data["username"];
        std::string email = data["email"];
        std::string password = data["password"];
        
        if (userDatabase.exists(username)) {
            json response = {{"success", false}, {"message", "Username already exists"}};
            res.set_content(response.dump(), "application/json");
            return;
        }
        
        User* newUser = new User(nextUserId++, username, email, User::hashPassword(password));
        userDatabase.insert(username, newUser);
        
        json response = {{"success", true}, {"message", "Registration successful"}};
        res.set_content(response.dump(), "application/json");
    });
    
    svr.Post("/api/login", [](const Request& req, Response& res) {
        setupCORS(res);
        json data = json::parse(req.body);
        
        std::string username = data["username"];
        std::string password = data["password"];
        
        User* user = nullptr;
        if (!userDatabase.find(username, user) || !user) {
            json response = {{"success", false}, {"message", "Invalid credentials"}};
            res.set_content(response.dump(), "application/json");
            return;
        }
        
        if (!user->validatePassword(password)) {
            json response = {{"success", false}, {"message", "Invalid credentials"}};
            res.set_content(response.dump(), "application/json");
            return;
        }
        
        std::string token = generateToken(username);
        json response = {{"success", true}, {"token", token}, {"username", username}};
        res.set_content(response.dump(), "application/json");
    });
    
    svr.Get("/api/notes", [](const Request& req, Response& res) {
        setupCORS(res);
        std::string authHeader = req.get_header_value("Authorization");
        
        if (authHeader.substr(0, 7) != "Bearer ") {
            res.status = 401;
            json response = {{"success", false}, {"message", "Unauthorized"}};
            res.set_content(response.dump(), "application/json");
            return;
        }
        
        std::string token = authHeader.substr(7);
        std::string username = verifyToken(token);
        
        if (username.empty()) {
            res.status = 401;
            json response = {{"success", false}, {"message", "Invalid token"}};
            res.set_content(response.dump(), "application/json");
            return;
        }
        
        User* user = nullptr;
        userDatabase.find(username, user);
        
        auto userNotes = noteDatabase.getNotesByUserId(user->getId());
        json notesArray = json::array();
        
        for (auto note : userNotes) {
            json noteJson = {
                {"id", note->getId()},
                {"title", note->getTitle()},
                {"content", note->getContent()},
                {"updated_at", note->getUpdatedAt()}
            };
            notesArray.push_back(noteJson);
        }
        
        res.set_content(notesArray.dump(), "application/json");
    });
    
    svr.Post("/api/notes", [](const Request& req, Response& res) {
        setupCORS(res);
        std::string authHeader = req.get_header_value("Authorization");
        std::string token = authHeader.substr(7);
        std::string username = verifyToken(token);
        
        if (username.empty()) {
            res.status = 401;
            return;
        }
        
        User* user = nullptr;
        userDatabase.find(username, user);
        
        json data = json::parse(req.body);
        std::string title = data["title"];
        std::string content = data["content"];
        
        Note* newNote = new Note(nextNoteId++, user->getId(), title, content);
        noteDatabase.addNote(newNote);
        
        json response = {
            {"success", true},
            {"id", newNote->getId()},
            {"title", newNote->getTitle()},
            {"content", newNote->getContent()},
            {"updated_at", newNote->getUpdatedAt()}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    svr.Put("/api/notes/(\\d+)", [](const Request& req, Response& res) {
        setupCORS(res);
        int noteId = std::stoi(req.matches[1]);
        
        json data = json::parse(req.body);
        Note* note = noteDatabase.findNoteById(noteId);
        
        if (note) {
            note->update(data["title"], data["content"]);
            json response = {{"success", true}};
            res.set_content(response.dump(), "application/json");
        } else {
            res.status = 404;
            json response = {{"success", false}, {"message", "Note not found"}};
            res.set_content(response.dump(), "application/json");
        }
    });
    
    svr.Delete("/api/notes/(\\d+)", [](const Request& req, Response& res) {
        setupCORS(res);
        int noteId = std::stoi(req.matches[1]);
        
        if (noteDatabase.deleteNoteById(noteId)) {
            json response = {{"success", true}};
            res.set_content(response.dump(), "application/json");
        } else {
            res.status = 404;
            json response = {{"success", false}, {"message", "Note not found"}};
            res.set_content(response.dump(), "application/json");
        }
    });
    
    svr.Post("/api/ai/summarize", [](const Request& req, Response& res) {
        setupCORS(res);
        json response = {
            {"success", true},
            {"summary", "AI summarization feature - integrate with Google AI or OpenAI API"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    svr.Post("/api/ai/improve", [](const Request& req, Response& res) {
        setupCORS(res);
        json data = json::parse(req.body);
        json response = {
            {"success", true},
            {"improved_text", data["text"].get<std::string>() + "\n\n[AI Improvement - integrate with Google AI or OpenAI API]"}
        };
        res.set_content(response.dump(), "application/json");
    });
    
    std::cout << "Server starting on http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}
