#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#include <TimeLib.h>

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "OLEDDisplayUi.h"

unsigned long previousMillis;
String country, cases, todayCases, deaths, todayDeaths, recovered, active, critical, updated;
unsigned long t_unix_date;

SSD1306Wire  display(0x3c, 5, 4);

OLEDDisplayUi ui     ( &display );

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(30 + x, 10 + y, "COVID-19");
  display->drawString(30 + x, 26 + y, country);
  //display->setFont(ArialMT_Plain_10);
  display->drawString(0 + x, 42 + y, String(year(t_unix_date)) + "/" + String(month(t_unix_date)) + "/" + String(day(t_unix_date)) + " "+ String(hour(t_unix_date))+":" + String(minute(t_unix_date))+":" + String(second(t_unix_date)));
}
void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(0 + x, 10 + y, "country: " + String(country));
  display->drawString(0 + x, 26 + y, "cases: " + String(cases)) ;
  display->drawString(0 + x, 42 + y, "todayCases: " + String(todayCases));
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(0 + x, 10 + y, "deaths: " + String(deaths));
  display->drawString(0 + x, 26 + y, "todayDeaths: " + String(todayDeaths)) ;
  display->drawString(0 + x, 42 + y, "recover: " + String(recovered));
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(0 + x, 10 + y, "active: " + String(active));
  display->drawString(0 + x, 26 + y, "critical: " + String(critical)) ;
  display->drawString(0 + x, 42 + y, "update: " + String(year(t_unix_date)) + "/" + String(month(t_unix_date)) + "/" + String(day(t_unix_date)));
}


FrameCallback frames[] = {drawFrame1, drawFrame2, drawFrame3, drawFrame4};
int frameCount = 4;

void setup() {

  int i;
  ui.setTargetFPS(30);
  ui.setIndicatorPosition(RIGHT);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_DOWN);
  ui.setFrames(frames, frameCount);
  ui.init();
  display.clear();
  //display.flipScreenVertically();

  display.init();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    display.drawString(0 + i, 10, ".");
    i += 5;
    display.display();
  }
  delay(1000);
  display.drawString(0, 30, "WiFi connected");
  display.display();
  httpget();
  delay(2000);
}


void loop() {
  if (millis() - previousMillis >= 10000)
  {
    previousMillis = millis();
    httpget();
  }

  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    delay(remainingTimeBudget);
  }
}

void httpget()
{
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    http.begin("https://corona.lmao.ninja/countries/THA"); //Specify the URL
    int httpCode = http.GET();                             //Make the request
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      String Data[45];
      int count = 0, lastIndex = 0;
      for (int i = 0; i < payload.length(); i++)
      {
        if (payload.substring(i, i + 1) == "\"")
        {
          Data[count] = payload.substring(lastIndex, i);
          lastIndex = i + 1;
          count++;
        }
        else
        {
          Data[count] = payload.substring(lastIndex, payload.length());
        }

        country = Data[3];
        updated = Data[26].substring(1, Data[26].length() - 4);
        cases = Data[28].substring(1, Data[28].length() - 1);
        todayCases = Data[30].substring(1, Data[30].length() - 1);
        deaths = Data[32].substring(1, Data[32].length() - 1);
        todayDeaths = Data[34].substring(1, Data[34].length() - 1);
        recovered = Data[36].substring(1, Data[36].length() - 1);
        active = Data[38].substring(1, Data[38].length() - 1);
        critical = Data[40].substring(1, Data[40].length() - 1);

        t_unix_date = atol(updated.c_str());
      }
    }
    else {
    }
    http.end(); //Free the resources
  }
}
