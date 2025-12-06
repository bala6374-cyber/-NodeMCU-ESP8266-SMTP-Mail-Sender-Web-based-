# NodeMCU ESP8266 SMTP Mail Sender (Web-based)

This project provides a web-based email sending dashboard hosted on a NodeMCU ESP8266.  
It allows sending email through SMTP directly from the browser using credentials you enter.

## ✨ Features
- Web UI hosted on ESP8266
- Works with Gmail, Outlook, Yahoo, Zoho, custom SMTP
- Supports SSL/TLS
- Sends plain text or HTML email
- Configurable SMTP server, port, username, password

## 🛠 Requirements
- NodeMCU ESP8266 board
- Arduino IDE
- ESP8266 board package installed
- WiFi network

## 🌐 Default AP Mode
SSID: `ESP-Mail-AP`  
Password: `12345678`

## 🔧 How to Upload
1. Open Arduino IDE  
2. Load SMTPMailer.ino  
3. Upload Sketch  
4. Upload `/data` folder using “ESP8266 Sketch Data Upload”  

## 📄 License
MIT
