#include "AIIntegration.h"
#include <iostream>

AIIntegration::AIIntegration(const std::string& googleKey, const std::string& openaiKey)
    : googleApiKey(googleKey), openaiApiKey(openaiKey) {}

size_t AIIntegration::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string AIIntegration::makeHttpRequest(const std::string& url, const std::string& postData, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string authHeader = "Authorization: Bearer " + apiKey;
        headers = curl_slist_append(headers, authHeader.c_str());
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    
    return readBuffer;
}

std::string AIIntegration::summarizeText(const std::string& text) {
    if (openaiApiKey.empty()) {
        return "Please configure OpenAI API key in config.json";
    }
    
    json requestBody = {
        {"model", "gpt-3.5-turbo"},
        {"messages", {
            {{"role", "system"}, {"content", "You are a helpful assistant that summarizes text concisely."}},
            {{"role", "user"}, {"content", "Summarize the following text:\n\n" + text}}
        }},
        {"max_tokens", 150}
    };
    
    std::string response = makeHttpRequest(
        "https://api.openai.com/v1/chat/completions",
        requestBody.dump(),
        openaiApiKey
    );
    
    try {
        json responseJson = json::parse(response);
        return responseJson["choices"][0]["message"]["content"];
    } catch (const std::exception& e) {
        return "Error processing AI response";
    }
}

std::string AIIntegration::improveText(const std::string& text) {
    if (openaiApiKey.empty()) {
        return "Please configure OpenAI API key in config.json";
    }
    
    json requestBody = {
        {"model", "gpt-3.5-turbo"},
        {"messages", {
            {{"role", "system"}, {"content", "You are a helpful writing assistant that improves text clarity and grammar."}},
            {{"role", "user"}, {"content", "Improve the following text:\n\n" + text}}
        }},
        {"max_tokens", 500}
    };
    
    std::string response = makeHttpRequest(
        "https://api.openai.com/v1/chat/completions",
        requestBody.dump(),
        openaiApiKey
    );
    
    try {
        json responseJson = json::parse(response);
        return responseJson["choices"][0]["message"]["content"];
    } catch (const std::exception& e) {
        return "Error processing AI response";
    }
}

std::string AIIntegration::askQuestion(const std::string& question, const std::string& context) {
    if (googleApiKey.empty() && openaiApiKey.empty()) {
        return "Please configure AI API keys in config.json";
    }
    
    json requestBody = {
        {"model", "gpt-3.5-turbo"},
        {"messages", {
            {{"role", "system"}, {"content", "You are a helpful assistant that answers questions based on the provided context."}},
            {{"role", "user"}, {"content", "Context: " + context + "\n\nQuestion: " + question}}
        }},
        {"max_tokens", 200}
    };
    
    std::string response = makeHttpRequest(
        "https://api.openai.com/v1/chat/completions",
        requestBody.dump(),
        openaiApiKey
    );
    
    try {
        json responseJson = json::parse(response);
        return responseJson["choices"][0]["message"]["content"];
    } catch (const std::exception& e) {
        return "Error processing AI response";
    }
}
