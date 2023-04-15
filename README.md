# Weather Station

![Plant, the weather station](https://i.imgur.com/nYFJtBd.png)

Esp8266 with DHT22 temperature & humidity sensor

## Intro

Plant is a weather station that I tried to make as useful as possible. It is designed to work on a LithiumIon battery that can be charged through a micro usb, and then placed somewhere in your room where it
looks good and can be used any time through your network a site you can always connect to and check
the Temperature and humidity of the room in which the plant was placed on.

## Hardware

![hardware](https://i.imgur.com/wGmoTqq.png)

![more hardware](https://i.imgur.com/mRySDWx.png)


## Schematic

![schematic](https://i.imgur.com/1uq5HiX.png)

This schematic shows how the all the pieces are connected. Unfortunately, so far, I have not managed to
make all the components work together. For now, it works well with the humidity and temperature
sensors, though the BMP sensor can get the humidity and air pressure, I figured it wasn’t really that
important, though I included it either way in the schematic.

## Implementation

Hotspot but could also be local network
```c
const char* ssid = "HUAWEI-P30-lite"; 
const char* password = "******";
```
Will store last time DHT was updated
```c
float t = 0.0, h = 0.0;
unsigned long previousMillis = 0;  
```
Updates DHT readings every 10 seconds
```c
const long interval = 10000;
```
Main loop:
```c
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
  //print data to the serial monitor
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
   ... // webpagecode
  delay(5000); // wait 5 s
  }
   ```

## Connection
   
I am using the board to connect to the WIFI and then
have the board be assigned an IP address where the users connected to the same network have access
to the information from the sensors. This can be easily seen on my phone hotspot. I can see the ESP
clearly on the network created between them and the IP I need to connect to:

![](https://i.imgur.com/r1hW0S3.png)

## Website

![](https://i.imgur.com/4j8LHvz.png)

## Final State
Here is the final product, where I introduced the components in a plastic plant that is a great case for
this project in my opinion:

![](https://i.imgur.com/9VPwOBZ.png)

