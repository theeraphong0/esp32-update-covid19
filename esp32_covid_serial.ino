#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#include <TimeLib.h>

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    http.begin("https://corona.lmao.ninja/v2/countries/THA"); //Specify the URL
    int httpCode = http.GET();                             //Make the request
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);

      String Data[45];
      String country, cases, todayCases, deaths, todayDeaths, recovered, active, critical, updated;
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

        updated = Data[2].substring(1, Data[2].length() - 4);
        country = Data[5];
        cases = Data[28].substring(1, Data[28].length() - 1);
        todayCases = Data[30].substring(1, Data[30].length() - 1);
        deaths = Data[32].substring(1, Data[32].length() - 1);
        todayDeaths = Data[34].substring(1, Data[34].length() - 1);
        recovered = Data[36].substring(1, Data[36].length() - 1);
        active = Data[38].substring(1, Data[38].length() - 1);
        critical = Data[40].substring(1, Data[40].length() - 1);
        
      }
      Serial.print("country"); Serial.print(" = ") ; Serial.println(country);
      Serial.print("cases"); Serial.print(" = ") ; Serial.println(cases);
      Serial.print("todayCases"); Serial.print(" = ") ; Serial.println(todayCases);
      Serial.print("deaths"); Serial.print(" = ") ; Serial.println(deaths);
      Serial.print("todayDeaths"); Serial.print(" = ") ; Serial.println(todayDeaths);
      Serial.print("recovered"); Serial.print(" = ") ; Serial.println(recovered);
      Serial.print("active"); Serial.print(" = ") ; Serial.println(active);
      Serial.print("critical"); Serial.print(" = ") ; Serial.println(critical);

      unsigned long t_unix_date = atol(updated.c_str())+25200;
      Serial.print("updated"); Serial.print(" = ") ; Serial.print(year(t_unix_date)); Serial.print("/"); Serial.print(month(t_unix_date));  Serial.print("/");   Serial.print(day(t_unix_date));  Serial.print("\t");     Serial.print(hour(t_unix_date)); Serial.print(":");       Serial.print(minute(t_unix_date));  Serial.print(":");      Serial.println(second(t_unix_date));
      Serial.println();
      /*
      for (int i = 0; i < 45; i++)
      {
        Serial.print(i);
        Serial.print("=");
        Serial.println( Data[i]);
      }
      */
    }

    else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(10000);

}
