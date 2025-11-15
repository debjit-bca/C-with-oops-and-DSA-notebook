# Quick Setup Guide - Digital Notebook

## Step-by-Step Installation

### For Windows Users

#### 1. Install Build Tools
```powershell
# Install Visual Studio Build Tools
# Download from: https://visualstudio.microsoft.com/downloads/
# Select "Desktop development with C++"

# Install CMake
# Download from: https://cmake.org/download/
```

#### 2. Install vcpkg (Package Manager)
```powershell
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

#### 3. Install C++ Dependencies
```powershell
cd C:\vcpkg
.\vcpkg install openssl:x64-windows
.\vcpkg install curl:x64-windows
.\vcpkg install cpp-httplib:x64-windows
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install jwt-cpp:x64-windows
```

#### 4. Build the Project
```powershell
cd "C:\Users\Debjit kundu\OneDrive\Desktop\programming\digital-notebook"
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

#### 5. Configure API Keys
Edit `config.json`:
```json
{
  "ai": {
    "openai_api_key": "sk-proj-your-key-here"
  }
}
```

Get OpenAI API key: https://platform.openai.com/api-keys

#### 6. Run the Server
```powershell
cd build\Release
.\notebook_server.exe
```

#### 7. Run the Frontend
Open a new terminal:
```powershell
cd "C:\Users\Debjit kundu\OneDrive\Desktop\programming\digital-notebook\frontend"
python -m http.server 3000
```

Or install Node.js and run:
```powershell
npx http-server -p 3000
```

#### 8. Access the App
Open browser: `http://localhost:3000`

---

### For Linux/Mac Users

#### 1. Install Dependencies
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libssl-dev libcurl4-openssl-dev

# macOS
brew install cmake openssl curl
```

#### 2. Install C++ Libraries
```bash
cd ~
git clone https://github.com/yhirose/cpp-httplib.git
git clone https://github.com/nlohmann/json.git
git clone https://github.com/Thalhammer/jwt-cpp.git

sudo cp cpp-httplib/httplib.h /usr/local/include/
sudo cp json/single_include/nlohmann/json.hpp /usr/local/include/nlohmann/
sudo cp -r jwt-cpp/include/jwt-cpp /usr/local/include/
```

#### 3. Build the Project
```bash
cd ~/digital-notebook
mkdir build && cd build
cmake ..
make
```

#### 4. Run the Application
```bash
# Terminal 1: Backend
./notebook_server

# Terminal 2: Frontend
cd ../frontend
python3 -m http.server 3000
```

---

## Quick Start Without Compilation (JavaScript Only)

If you want to test the frontend without C++ backend:

1. **Use a mock server** or modify `frontend/js/auth.js` and `frontend/js/notebook.js` to use localStorage:

```javascript
// Mock authentication for testing
const users = JSON.parse(localStorage.getItem('users')) || [];
const notes = JSON.parse(localStorage.getItem('notes')) || [];
```

2. **Run frontend only**:
```bash
cd frontend
python -m http.server 3000
```

---

## Deployment to Cloud

### Deploy to Heroku (Backend)

1. **Create Procfile**:
```
web: ./notebook_server
```

2. **Use Heroku buildpack**:
```bash
heroku create your-notebook-app
heroku buildpacks:add https://github.com/heroku/heroku-buildpack-apt
git push heroku main
```

### Deploy Frontend to Netlify

1. Drag and drop `frontend/` folder to Netlify
2. Update API_URL in JavaScript files to your Heroku backend URL

### Deploy to AWS

**Backend (EC2)**:
```bash
# SSH into EC2
ssh -i key.pem ubuntu@your-ec2-ip

# Build and run
cmake .. && make
./notebook_server
```

**Frontend (S3 + CloudFront)**:
- Upload `frontend/` to S3 bucket
- Enable static website hosting
- Use CloudFront for CDN

---

## Troubleshooting

### "Cannot find OpenSSL"
```bash
# Windows
.\vcpkg install openssl:x64-windows

# Linux
sudo apt-get install libssl-dev
```

### "httplib.h not found"
Download manually: https://github.com/yhirose/cpp-httplib

### Port 8080 already in use
Change port in `server.cpp`:
```cpp
svr.listen("0.0.0.0", 9000); // Use different port
```

### CORS errors
Already configured in server.cpp. Ensure backend is running on port 8080.

---

## Testing the Application

1. **Register a new user**
   - Username: testuser
   - Email: test@example.com
   - Password: password123

2. **Login** with credentials

3. **Create notes**
   - Click "New Note"
   - Add title and content
   - Click "Save"

4. **Test AI features** (requires API key)
   - Add content to note
   - Click "AI Summary" or "AI Improve"

---

## Need Help?

- Check README.md for detailed documentation
- Ensure all dependencies are installed
- Verify config.json has correct API keys
- Check server logs for errors

Happy coding! 🚀
