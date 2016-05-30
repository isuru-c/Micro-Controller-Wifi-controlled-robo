#define DEBUG true 

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,2000\r\n",1000,DEBUG); // turn on server on port 80
}

void loop(){
  if(Serial1.available()){
    String input = Serial1.readString();
    Serial.println(input);
  }  
}

String sendData(String command, const int timeout, boolean debug)
{
  Serial1.print(command);
  
  String response = "";
  long int time = millis();

  while( (time+timeout) > millis())
  {
    while(Serial1.available())
    {
      char c = Serial1.read();
      response+=c;
    }  
  }

  if(debug)
  {
    Serial.print(response);
  }

  return response;
}
