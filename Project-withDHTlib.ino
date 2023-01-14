#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 0  // Pin for connecting the DHT sensor - I believe its the GPIO 0 which is equal to pin D3 on the board
#define DHTTYPE DHT22
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "HUAWEI-P30-lite";
const char* password = "******";

float t = 0.0, h = 0.0;
unsigned long previousMillis = 0;  // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;

String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return String(t);
  } else if (var == "HUMIDITY") {
    return String(h);
  }
  return String();
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  dht.begin();
  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  // Print the values to the serial monitor
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();

    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      t = newT;
    }
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      h = newH;
    }
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  //Handle client connections
  WiFiClient client = server.available();
  if (client) {
    // Print the values to the web page
   client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>ESP8266 DHT22</title>");
    client.println("<style>");
    client.println("body {background-color: #f2f2f2;}");
    client.println("h1 {text-align: center; font-family: Arial, sans-serif;color:white;}");
    client.println(".sensor-value {font-size: 60px; background-color:black; opacity:0.8;width:50%; color:white;text-align:center;}");
    client.println("img.icon { width: 60px; height: 60px; margin-right: 20px; vertical-align: middle;}");
    client.println(".temperature { background-repeat: no-repeat; padding-left: 40px; }");
    client.println(".humidity { background-repeat: no-repeat; padding-left: 40px;}");
    client.println("body { background-image: url('https://wallpaper.dog/large/427491.jpg'); background-repeat: no-repeat;background-size: cover;}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>DHT22 Sensor Data</h1>");
    client.println("<div class='sensor-value temperature'><img class='icon' src='https://icons.iconarchive.com/icons/graphicloads/folded/48/c-temperature-folded-icon.png'><span>Temperature: ");
    client.print(t);
    client.println(" *C</span></div>");
    client.println("<div class='sensor-value humidity'><img class='icon' src='https://icons.iconarchive.com/icons/custom-icon-design/lovely-weather-2/64/Humidity-icon.png'><span>Humidity: ");
    client.print(h);
    client.println(" %</span></div>");
    client.println("</body>");
    client.println("</html>");
    client.stop();
  }

  delay(5000);
}