void setup()
{
    // initialize serial ports
    Serial.begin(9600);    // USB serial port 0
}
void loop()
{
    String msg = "";
    // check for data byte on USB serial port
    if (Serial.available())
    {
        // get byte from USB serial port
        while(Serial.available())
        {
            msg = Serial.readString();// read the incoming data as string
        }
        if( msg == "h")
        {
            //Send data to usb serial port
            Serial.write(" received hello");
        }
        else
        {
            char* msg_ptr = &msg[0];
            //Send data to usb serial port
            Serial.write(msg_ptr);
        }
        //delay(1000);
    }
}