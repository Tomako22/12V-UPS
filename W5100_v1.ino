#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_INA219.h>
#include <Wire.h>

#define T1 8  //Defines pins used
#define T2 6
#define T3 7
#define T4 9
#define B1 3
#define B2 4

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
const byte ip[] = { 192, 168, 0, 178 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")

EthernetServer server(80);                             //server port
String readString;

Adafruit_INA219 ina219_1(0x40);
Adafruit_INA219 ina219_2(0x42);
Adafruit_INA219 ina219_3(0x41);
Adafruit_INA219 ina219_4(0x43);
Adafruit_INA219 ina219_5(0x44);

void setup()
{
  //start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  //Open serial communications and wait for port to open:
  //Serial.begin(115200);
  //Serial.print("server is at ");
  //Serial.println(Ethernet.localIP());
  pinMode(T1, OUTPUT);
  pinMode(T2, OUTPUT);
  pinMode(T3, OUTPUT);
  pinMode(T4, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  digitalWrite(T1, 0);
  digitalWrite(T2, 0);
  digitalWrite(T3, 0);
  digitalWrite(T4, 1);
  digitalWrite(B1, 0);
  digitalWrite(B2, 0);
  ina219_1.begin();
  ina219_2.begin();
  ina219_3.begin();
  ina219_4.begin();
  ina219_5.begin();
}

void loop()
{
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
        //if HTTP request has ended
        if (c == '\n') {
          //Serial.println(readString); //print to serial monitor for debuging
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<link rel='stylesheet' type='text/css' href='https://gitcdn.link/repo/Tomako22/12V-UPS/main/W5100_1.0.css' />"); //odkázanie na externý
          client.println("<TITLE>12V-UPS</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.print("<H1>Statistika</H1><br/><table>");
          client.print("<tr><th>Vetva</th><th>Prud</th><th>Napatie</th></tr>");
          client.print("<tr><td>1</td><td>"); client.print(ina219_1.getShuntVoltage_mV() / 0.05); client.print("</td>");
          client.print("<td>"); client.print(ina219_1.getBusVoltage_V()); client.print("</td>"); client.println("</tr>"); //Riadok 1

          client.print("<tr><td>2</td><td>"); client.print(ina219_2.getShuntVoltage_mV() / 0.05); client.print("</td>");
          client.print("<td>"); client.print(ina219_2.getBusVoltage_V()); client.print("</td>"); client.println("</tr>"); //Riadok 2

          client.print("<tr><td>3</td><td>"); client.print(ina219_3.getShuntVoltage_mV() / 0.05); client.print("</td>");
          client.print("<td>"); client.print(ina219_3.getBusVoltage_V()); client.print("</td>"); client.println("</tr>"); //Riadok 3

          client.print("<tr><td>ADJ</td><td>"); client.print(ina219_4.getShuntVoltage_mV() / 0.05); client.print("</td>");
          client.print("<td>"); client.print(ina219_4.getBusVoltage_V()); client.print("</td>"); client.println("</tr>"); //Riadok 4

          client.print("<tr><td>PoE</td><td>"); client.print(ina219_5.getShuntVoltage_mV() / 0.05); client.print("</td>");
          client.print("<td>"); client.print(ina219_5.getBusVoltage_V()); client.print("</td>"); client.println("</tr>"); //Riadok 5
          client.print("</table>");
          client.println("<hr />");
          client.println("<br />");
          client.println("<H1>Ovladanie</H1>");
          client.println("<br />");
          client.print("<a href=\"/?n0\">Vetva PoE ZAPNUTIE");
          client.println("</a>");
          client.print("<a href=\"/?f0\">Vetva PoE VYPNUTIE");
          client.print("</a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<p></p>");
          client.print("<a href=\"/?n1\">Vetva 1 ZAPNUTIE");
          client.println("</a>");
          client.print("<a href=\"/?f1\">Vetva 1 VYPNUTIE");
          client.print("</a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<p></p>");
          client.print("<a href=\"/?n2\">Vetva 2 ZAPNUTIE");
          client.println("</a>");
          client.print("<a href=\"/?f2\">Vetva 2 VYPNUTIE");
          client.print("</a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<p></p>");
          client.print("<a href=\"/?n3\">Vetva 3 ZAPNUTIE");
          client.println("</a>");
          client.print("<a href=\"/?f3\">Vetva 3 VYPNUTIE");
          client.print("</a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<p></p>");
          client.print("<a href=\"/?n4\">Vetva 4 ZAPNUTIE");
          client.println("</a>");
          client.print("<a href=\"/?f4\">Vetva 4 VYPNUTIE");
          client.print("</a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<p></p>");
          client.println("</BODY>");
          client.println("</HTML>");
          delay(1);
          //stopping client
          client.stop();
          //controls the Arduino if you press the buttons
          if (readString.indexOf("?n0") > 0)
            digitalWrite(T4, 1);
          if (readString.indexOf("?f0") > 0)
            digitalWrite(T4, 0);
          if (readString.indexOf("?n1") > 0)
            digitalWrite(T1, 0);
          if (readString.indexOf("?f1") > 0)
            digitalWrite(T1, 1);
          if (readString.indexOf("?n2") > 0)
            digitalWrite(T3, 0);
          if (readString.indexOf("?f2") > 0)
            digitalWrite(T3, 1);
          if (readString.indexOf("?n3") > 0)
            digitalWrite(T2, 0);
          if (readString.indexOf("?f3") > 0)
            digitalWrite(T2, 1);
          if (readString.indexOf("?n4") > 0)
          {
            digitalWrite(B1, 0);
            digitalWrite(B2, 0);
          }
          if (readString.indexOf("?f4") > 0)
          {
            digitalWrite(B1, 1);
            digitalWrite(B2, 1);
          }
          //clearing string for next read
          readString = "";
        }
      }
    }
  }
}
