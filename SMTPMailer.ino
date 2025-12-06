/********************************************************************
 *  ESP8266 MAIL SENDER (SMTP + Web UI)
 *  Author: miniBotix / Kavinmayil
 *  Board: NodeMCU ESP8266
 ********************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <FS.h>

// Web server on port 80
ESP8266WebServer server(80);

// Default AP mode
const char* ap_ssid = "ESP-Mail-AP";
const char* ap_pass = "12345678";

// Email form parameters
String smtp_server = "";
int smtp_port = 465;  
String smtp_user = "";
String smtp_passw = "";
String email_to = "";
String email_subject = "";
String email_body = "";

// -------- SEND EMAIL FUNCTION --------
bool sendEmail() {
  WiFiClientSecure client;
  client.setInsecure(); // Allow self-signed servers

  Serial.println("Connecting to SMTP...");

  if (!client.connect(smtp_server, smtp_port)) {
    Serial.println("SMTP Connection failed");
    return false;
  }

  auto sendLine = [&](String cmd, bool hide = false) {
    client.println(cmd);
    client.flush();
    delay(200);
    String resp = client.readStringUntil('\n');
    Serial.println(hide ? "[HIDDEN]" : resp);
  };

  sendLine("EHLO esp8266");
  sendLine("AUTH LOGIN");

  sendLine(base64::encode(smtp_user), true);
  sendLine(base64::encode(smtp_passw), true);

  sendLine("MAIL FROM: <" + smtp_user + ">");
  sendLine("RCPT TO: <" + email_to + ">");
  sendLine("DATA");

  client.println("From: ESP8266 <" + smtp_user + ">");
  client.println("To: <" + email_to + ">");
  client.println("Subject: " + email_subject);
  client.println("Content-Type: text/html");
  client.println("");
  client.println(email_body);
  client.println(".");
  client.println("QUIT");

  Serial.println("Mail sent!");
  return true;
}

// -------- API ENDPOINT TO SEND MAIL --------
void handleSendMail() {
  smtp_server = server.arg("smtp");
  smtp_port   = server.arg("port").toInt();
  smtp_user   = server.arg("user");
  smtp_passw  = server.arg("pass");
  email_to    = server.arg("to");
  email_subject = server.arg("sub");
  email_body  = server.arg("body");

  bool ok = sendEmail();

  if (ok) server.send(200, "text/plain", "Email Sent Successfully!");
  else    server.send(500, "text/plain", "Failed to send email.");
}

// -------- SERVE SPIFFS FILES --------
bool serveFile(String path) {
  if (!SPIFFS.exists(path)) return false;
  File file = SPIFFS.open(path, "r");
  server.streamFile(file, "text/html");
  file.close();
  return true;
}

void handleRoot() {
  serveFile("/index.html");
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();

  // AP Mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", handleRoot);
  server.on("/send", handleSendMail);

  server.begin();
}

void loop() {
  server.handleClient();
}
