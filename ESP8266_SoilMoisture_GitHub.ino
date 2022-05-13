#include <ESP8266WiFi.h>

// 設定無線基地台SSID跟密碼
const char* ssid     = "WiFi名稱";     
const char* password = "WiFi密碼";     

 
void setup(void)
{
  Serial.begin(115200);  // 設定速率 感測器
  // 連接無線基地台
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(A0, INPUT);
}
 
void loop(void)
{
  delay(1000);

  int sensorValue=analogRead(A0); //讀取感測器回傳值
  
  // 檢查兩個值是否為空值
  if (isnan(sensorValue)) {
     Serial.println("Failed to read from DHT sensor!");
     return;
  }
  Serial.println("土壤濕度:");
  Serial.println(sensorValue);

  //執行API Request
  WiFiClient client;
  const uint16_t port = 80;
  const char * host = "IP位置"; 
  client.connect(host, port);
  
  // 檢查連線是否成功
  if (!client.connect(host, port)) {
  Serial.println("connection failed!");
  return;
  }else{
    Serial.println("Success");
    
    String postStr = "";
    postStr +="SoilMoistureCode=";
    postStr += String("1");
    postStr +="&SoilMoistureData=";
    postStr += String(sensorValue);
      
    String url = "/api/ESP8266SoilMoisture?" + postStr;
    client.print(String("GET ")  + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +"Connection:close\r\n\r\n");

     //查看回傳值
     while(client.available()){
     String line = client.readStringUntil('\r');
     Serial.print(line);
    }
  }


  Serial.println("連線關閉");
  client.stop();


} 
