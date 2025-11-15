# Digital Notebook - Full Stack Application

A modern digital notebook application with login/registration system, built using HTML, CSS, JavaScript for frontend and C++ for backend with OOP and DSA principles.

## Features

- **User Authentication**: Secure login and registration system with JWT tokens
- **Digital Notebook**: Create, edit, delete, and manage notes
- **AI Integration**: Text summarization and improvement using OpenAI/Google AI APIs
- **Data Structures**: 
  - HashMap for user management (O(1) lookup)
  - Linked List for notes storage (efficient insertion/deletion)
- **OOP Design**: Clean class-based architecture (User, Note, NoteList, HashMap)
- **Responsive UI**: Modern, mobile-friendly interface

## Project Structure

```
digital-notebook/
├── frontend/
│   ├── index.html          # Login page
│   ├── register.html       # Registration page
│   ├── notebook.html       # Main notebook dashboard
│   ├── css/
│   │   └── style.css      # Responsive styling
│   └── js/
│       ├── auth.js        # Authentication logic
│       └── notebook.js    # Notebook functionality
├── backend/
│   ├── server.cpp         # HTTP server with REST API
│   ├── User.h/cpp         # User class (OOP)
│   ├── Note.h/cpp         # Note class (OOP)
│   ├── HashMap.h          # HashMap data structure (DSA)
│   ├── NoteList.h/cpp     # Linked List for notes (DSA)
│   └── AIIntegration.h/cpp # AI API integration
├── config.json            # Configuration file
├── CMakeLists.txt         # Build configuration
└── README.md              # This file
```

## Requirements

### C++ Dependencies
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- OpenSSL library (for password hashing)
- libcurl (for HTTP requests)
- cpp-httplib (HTTP server library)
- nlohmann/json (JSON parsing)
- jwt-cpp (JWT token generation)

### Installation

#### Windows (using vcpkg)
```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat

# Install dependencies
./vcpkg install openssl curl cpp-httplib nlohmann-json jwt-cpp

# Build project
cd digital-notebook
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

#### Linux/Mac
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libssl-dev libcurl4-openssl-dev cmake g++

# Install C++ libraries
git clone https://github.com/yhirose/cpp-httplib.git
git clone https://github.com/nlohmann/json.git
git clone https://github.com/Thalhammer/jwt-cpp.git

# Build project
cd digital-notebook
mkdir build && cd build
cmake ..
make
```

## Configuration

1. **Edit config.json**:
   - Add your OpenAI API key for AI features
   - Add your Google AI API key (optional)
   - Change JWT secret for production

```json
{
  "ai": {
    "openai_api_key": "sk-your-actual-api-key",
    "google_ai_api_key": "your-google-key"
  },
  "jwt": {
    "secret": "your-strong-secret-key"
  }
}
```

## Running the Application

### Local Development

1. **Start the C++ backend server**:
```bash
cd build
./notebook_server
# Server runs on http://localhost:8080
```

2. **Serve the frontend**:
```bash
cd frontend
# Using Python
python -m http.server 3000

# Using Node.js
npx http-server -p 3000

# Or use any web server
```

3. **Access the application**:
   - Open browser: `http://localhost:3000`
   - Register a new account
   - Login and start using the notebook

### Global Server Deployment

#### Deploy Backend (C++ Server)

**Option 1: VPS/Cloud Server (AWS, DigitalOcean, etc.)**
```bash
# Build on server
cmake .. -DCMAKE_BUILD_TYPE=Release
make

# Run with systemd
sudo nano /etc/systemd/system/notebook.service
# Add service configuration
sudo systemctl enable notebook
sudo systemctl start notebook
```

**Option 2: Docker**
```dockerfile
# Create Dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y build-essential cmake libssl-dev libcurl4-openssl-dev
COPY . /app
WORKDIR /app/build
RUN cmake .. && make
CMD ["./notebook_server"]
```

```bash
docker build -t digital-notebook .
docker run -p 8080:8080 digital-notebook
```

#### Deploy Frontend

**Option 1: Static Hosting (Netlify, Vercel, GitHub Pages)**
- Upload `frontend/` folder
- Configure environment variables for API URL

**Option 2: Nginx**
```nginx
server {
    listen 80;
    server_name yourdomain.com;
    
    location / {
        root /var/www/digital-notebook/frontend;
        index index.html;
    }
    
    location /api {
        proxy_pass http://localhost:8080;
    }
}
```

## API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/register` | Register new user |
| POST | `/api/login` | Login user |
| GET | `/api/notes` | Get all notes (authenticated) |
| POST | `/api/notes` | Create new note |
| PUT | `/api/notes/:id` | Update note |
| DELETE | `/api/notes/:id` | Delete note |
| POST | `/api/ai/summarize` | Summarize text with AI |
| POST | `/api/ai/improve` | Improve text with AI |

## OOP & DSA Implementation

### Object-Oriented Programming
- **User class**: Encapsulates user data and authentication
- **Note class**: Manages note data with timestamps
- **AIIntegration class**: Handles AI API requests
- **Inheritance & Polymorphism**: Extensible design

### Data Structures & Algorithms
- **HashMap**: O(1) average user lookup by username
- **Linked List**: Efficient note insertion/deletion
- **Hash Function**: Custom implementation for string keys
- **JWT**: Secure token-based authentication

## Security Features

- Password hashing with SHA-256
- JWT token authentication
- CORS protection
- Input validation
- Secure session management

## AI Integration

The application integrates with:
- **OpenAI GPT**: Text summarization and improvement
- **Google AI (Gemini)**: Alternative AI provider

To enable AI features, add your API keys to `config.json`.

## Future Enhancements

- [ ] Database persistence (SQLite/PostgreSQL)
- [ ] File attachments
- [ ] Note sharing
- [ ] Rich text editor
- [ ] Dark mode
- [ ] Mobile app

## License

MIT License - Feel free to use for learning and projects!

## Author

Created with modern web technologies and C++ backend architecture.
