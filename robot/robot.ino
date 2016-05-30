#define DEBUG true 

int led = 12;
int pwm_led = 13;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(pwm_led, OUTPUT);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,2000\r\n",1000,DEBUG); // turn on server on port 80
}

void loop(){
  if(Serial1.available()){
    String input = Serial1.readString();
    String data = getData(input);
    int val = pwmTest(data);
    Serial.println(val);
  }  
}

/**
 Get the input data and run the corresponding 
 function for that input.
*/
void processData(String data){
  if(data[0] == '0'){
    digitalWrite(led, LOW);
  }else if(data[0] == '1'){
    digitalWrite(led, HIGH);
  }
}

/**
 Test the output of pwm output
 To use this option, the output should have been set as pwm output
*/
int pwmTest(String data){
  int val = data.toInt();
  val = val % 256;
  analogWrite(pwm_led, val);
  return val;
}

/**
 Get the transmitted data by removing other characters.
 Basic reciving format is +IPD,0,5:Apple
 Here 1st number is the connection number
 The second number is the length of te input string
*/
String getData(String input){
  for(int i=0; i<input.length(); ++i){
    if(input[i] == ':'){
      String tmp = input.substring(9, i);
      int len = tmp.toInt();
      String data = input.substring(i+1, i+1+len);
      return data;
    }
  }
  return "";
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
