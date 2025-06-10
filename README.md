# PortSync | [Patreon Article](https://www.patreon.com/posts/portsync-esp32-130918146)

## Key elements of the project:

1. Dual Web Servers: Two independent web servers running on the same ESP32, on ports 80 and 81.

2. Password Protection: Users must log in with a predefined password.

3. Shared Chat History: Both servers read from and write to the same chat history string.

4. Web Interface: A modern, responsive web interface for sending and receiving messages.

5. Memory Management: The chat history is limited to prevent memory overflow.

## Setup and Usage:

- Step 1: Connect the ESP32 to WiFi (SSID and password set in code).

- Step 2: Access Server 1 at http://<ESP_IP> and Server 2 at http://<ESP_IP>:81.

- Step 3: Log in with the password.

- Step 4: Start chatting. Messages from Server 1 appear in blue (right-aligned), from Server 2 in green (left-aligned).
