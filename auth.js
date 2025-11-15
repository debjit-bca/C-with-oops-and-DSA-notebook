const API_URL = 'http://localhost:8080/api';

document.getElementById('loginForm')?.addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    
    try {
        const response = await fetch(`${API_URL}/login`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ username, password })
        });
        
        const data = await response.json();
        
        if (response.ok) {
            localStorage.setItem('token', data.token);
            localStorage.setItem('username', username);
            showMessage('Login successful!', 'success');
            setTimeout(() => window.location.href = 'notebook.html', 1000);
        } else {
            showMessage(data.message || 'Login failed', 'error');
        }
    } catch (error) {
        showMessage('Server connection error', 'error');
    }
});

document.getElementById('registerForm')?.addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('reg-username').value;
    const email = document.getElementById('reg-email').value;
    const password = document.getElementById('reg-password').value;
    const confirmPassword = document.getElementById('confirm-password').value;
    
    if (password !== confirmPassword) {
        showMessage('Passwords do not match', 'error');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/register`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ username, email, password })
        });
        
        const data = await response.json();
        
        if (response.ok) {
            showMessage('Registration successful! Redirecting to login...', 'success');
            setTimeout(() => window.location.href = 'index.html', 2000);
        } else {
            showMessage(data.message || 'Registration failed', 'error');
        }
    } catch (error) {
        showMessage('Server connection error', 'error');
    }
});

function showMessage(text, type) {
    const messageDiv = document.getElementById('message');
    messageDiv.textContent = text;
    messageDiv.className = `message ${type}`;
    messageDiv.style.display = 'block';
}
