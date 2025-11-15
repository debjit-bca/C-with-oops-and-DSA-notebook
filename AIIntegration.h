#ifndef AIINTEGRATION_H
#define AIINTEGRATION_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AIIntegration {
private:
    std::string googleApiKey;
    std::string openaiApiKey;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    std::string makeHttpRequest(const std::string& url, const std::string& postData, const std::string& apiKey);

public:
    AIIntegration(const std::string& googleKey = "", const std::string& openaiKey = "");
    
    std::string summarizeText(const std::string& text);
    std::string improveText(const std::string& text);
    std::string askQuestion(const std::string& question, const std::string& context);
    
    void setGoogleApiKey(const std::string& key) { googleApiKey = key; }
    void setOpenAIApiKey(const std::string& key) { openaiApiKey = key; }
};

#endif
