#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server1(80);  // First server on port 80
WebServer server2(81);  // Second server on port 81
String chatHistory = "";
const char* loginPassword = "SET_PASSWORD";

bool isLoggedIn1 = false;  // Login state for server1
bool isLoggedIn2 = false;  // Login state for server2

const char* loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Login</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            background: #f5f7fa;
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
        }
        .login-box {
            background: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
        }
        input[type="password"] {
            width: 100%;
            padding: 10px;
            margin-bottom: 10px;
            border-radius: 20px;
            border: 1px solid #ccc;
        }
        input[type="submit"] {
            width: 100%;
            padding: 10px;
            background: #1a73e8;
            color: white;
            border: none;
            border-radius: 20px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <div class="login-box">
        <h2>Login</h2>
        <form action="/login" method="POST">
            <input type="password" name="password" placeholder="Enter password" required>
            <input type="submit" value="Login">
        </form>
    </div>
</body>
</html>
)rawliteral";

const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Server Messager | Chirrenthen</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #f5f7fa;
            margin: 0;
            padding: 0;
        }
        .container {
            max-width: 600px;
            margin: 40px auto;
            background: white;
            border-radius: 8px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
            padding: 20px;
        }
        h1 {
            color: #1a73e8;
            text-align: center;
        }
        #chat {
            border: 1px solid #ddd;
            padding: 10px;
            height: 400px;
            overflow-y: scroll;
            margin-bottom: 10px;
            border-radius: 4px;
            background: #fafafa;
            display: flex;
            flex-direction: column;
        }
        .msg {
            margin: 8px 0;
            padding: 8px 12px;
            border-radius: 20px;
            max-width: 80%;
            word-wrap: break-word;
        }
        .web1 {
            background: #e8f0fe;
            align-self: flex-end;
            text-align: right;
        }
        .web2 {
            background: #d1e7dd;
            align-self: flex-start;
            text-align: left;
        }
        form {
            display: flex;
            gap: 10px;
            margin-top: 10px;
        }
        input[type="text"] {
            flex: 1;
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 20px;
        }
        input[type="submit"] {
            background: #1a73e8;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 20px;
            cursor: pointer;
        }
        .top-buttons {
            display: flex;
            justify-content: space-between;
            margin-bottom: 10px;
        }
        button {
            padding: 8px 16px;
            border: none;
            border-radius: 20px;
            cursor: pointer;
        }
        .logout {
            background: #e53935;
            color: white;
        }
        .clear {
            background: #fbc02d;
            color: white;
        }
    </style>
    <script>
        const currentPort = window.location.port || '80';

        function fetchChat() {
            fetch(`http://${window.location.hostname}:${currentPort}/get`)
                .then(response => response.text())
                .then(data => {
                    document.getElementById('chat').innerHTML = data;
                    document.getElementById('chat').scrollTop = document.getElementById('chat').scrollHeight;
                });
        }

        document.addEventListener('DOMContentLoaded', () => {
            const form = document.querySelector('form');
            form.action = `http://${window.location.hostname}:${currentPort}/message`;
        });

        setInterval(fetchChat, 1000);
        window.onload = fetchChat;
    </script>
</head>
<body>
    <div class="container">
        <h1>Messages</h1>
        <div class="top-buttons">
            <button class="logout" onclick="location.href='/logout'">Logout</button>
            <button class="clear" onclick="location.href='/clear'">Clear Chat</button>
        </div>
        <div id="chat"></div>
        <form method="POST">
            <input type="text" name="message" placeholder="Type your message">
            <input type="submit" value="Send">
        </form>
    </div>
</body>
</html>
)rawliteral";

// ========== Server 1 Handlers (Port 80) ==========
void handleRoot1() {
if (isLoggedIn1) {
server1.send(200, "text/html", htmlContent);
} else {
server1.send(200, "text/html", loginPage);
}
}

void handleLogin1() {
if (isLoggedIn1) {
server1.send(200, "text/html", "<h2>Already logged in!</h2>");
return;
}
if (server1.method() == HTTP_POST) {
String pass = server1.arg("password");
if (pass == loginPassword) {
isLoggedIn1 = true;
server1.sendHeader("Location", "/");
server1.send(303);
} else {
server1.send(200, "text/html", "<h2>Wrong Password!</h2><a href='/'>Try Again</a>");
}
}
}

void handleLogout1() {
isLoggedIn1 = false;
server1.sendHeader("Location", "/");
server1.send(303);
}

void handleGetChat1() {
if (isLoggedIn1) {
server1.send(200, "text/html", chatHistory);
} else {
server1.sendHeader("Location", "/");
server1.send(303);
}
}

void handleClearChat1() {
if (isLoggedIn1) {
chatHistory = "";
server1.sendHeader("Location", "/");
server1.send(303);
}
}

void handleMessage1() {
if (!isLoggedIn1) {
server1.sendHeader("Location", "/");
server1.send(303);
return;
}
if (server1.method() == HTTP_POST) {
String message = server1.arg("message");
if (message.length() > 0) {
chatHistory += "<div class='msg web1'>" + message + "</div><br>";
limitChatMemory();
}
}
server1.sendHeader("Location", "/");
server1.send(303);
}

// ========== Server 2 Handlers (Port 81) ==========
void handleRoot2() {
if (isLoggedIn2) {
server2.send(200, "text/html", htmlContent);
} else {
server2.send(200, "text/html", loginPage);
}
}

void handleLogin2() {
if (isLoggedIn1) {
server1.send(200, "text/html", "<h2>Already logged in!</h2>");
return;
}
if (server2.method() == HTTP_POST) {
String pass = server2.arg("password");
if (pass == loginPassword) {
isLoggedIn2 = true;
server2.sendHeader("Location", "/");
server2.send(303);
} else {
server2.send(200, "text/html", "<h2>Wrong Password!</h2><a href='/'>Try Again</a>");
}
}
}

void handleLogout2() {
isLoggedIn2 = false;
server2.sendHeader("Location", "/");
server2.send(303);
}

void handleGetChat2() {
if (isLoggedIn2) {
server2.send(200, "text/html", chatHistory);
} else {
server2.sendHeader("Location", "/");
server2.send(303);
}
}

void handleClearChat2() {
if (isLoggedIn2) {
chatHistory = "";
server2.sendHeader("Location", "/");
server2.send(303);
}
}

void handleMessage2() {
if (!isLoggedIn2) {
server2.sendHeader("Location", "/");
server2.send(303);
return;
}
if (server2.method() == HTTP_POST) {
String message = server2.arg("message");
if (message.length() > 0) {
chatHistory += "<div class='msg web2'>" + message + "</div><br>";
limitChatMemory();
}
}
server2.sendHeader("Location", "/");
server2.send(303);
}

void limitChatMemory() {
if (chatHistory.length() > 3000) {
chatHistory = chatHistory.substring(chatHistory.length() - 2000);
}
}

void setup() {
Serial.begin(115200);
WiFi.begin(ssid, password);

Serial.print("Connecting");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("\n✅ WiFi connected!");
Serial.print("==== Dual Server Access ====");
Serial.print("Server 1: http\://");
Serial.println(WiFi.localIP());
Serial.print("Server 2: http\://");
Serial.print(WiFi.localIP());
Serial.println(":81");
Serial.print("===========================");

// Server 1 routes
server1.on("/", handleRoot1);
server1.on("/login", handleLogin1);
server1.on("/logout", handleLogout1);
server1.on("/get", handleGetChat1);
server1.on("/clear", handleClearChat1);
server1.on("/message", handleMessage1);
server1.begin();

// Server 2 routes
server2.on("/", handleRoot2);
server2.on("/login", handleLogin2);
server2.on("/logout", handleLogout2);
server2.on("/get", handleGetChat2);
server2.on("/clear", handleClearChat2);
server2.on("/message", handleMessage2);
server2.begin();
}

void loop() {
server1.handleClient();
server2.handleClient();
}
