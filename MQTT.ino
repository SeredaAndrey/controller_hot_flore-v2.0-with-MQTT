// mosquitto_pub -h localhost -t "SmartHome/Living-Rooms/Room-Controller/Temperature" -m 21.0 сделать публикацию в топике
// mosquitto_sub -h localhost -t "SmartHome/Living-Rooms/Room-Controller/Activate"            подписаться на топик              

void handleMQTT ()
{
  IPAddress ip_MQTTip ( MQTTip[0], MQTTip[1], MQTTip[2], MQTTip[3] ); 
  String _topic = "/SmartHome"; _topic += pubtopic; _topic += subtopic;
  String _topicActiv = _topic + "/Activate";
  if ( WiFi.status () == WL_CONNECTED )
  {
    client.set_server ( ip_MQTTip );
    //Serial.println ( "Connect to WiFi - ACCEPT" );
    if ( !client.connected () )
    {
      //Serial.println ( "Connect to MQTT - ACCEPT" );
      if ( client.connect ( SSDP_Name ) )
      {
        client.set_callback ( callback );
        client.subscribe ( _topicActiv );
      }
    }
    else flagFailMQTTconnect = 1;
    if ( client.connected () )
    {
      client.loop ();
    }
    SendData ();
  }
}

void MQTTcall ()
{
  String _Activate;
//  String _Mode, _invertActivate;
  String _topic = "/SmartHome"; _topic += pubtopic; _topic += subtopic;
  String _topicTemp = _topic + "/temperature";
  String _topicActiv = _topic + "/Activate"; 
        if      ( Activate == 0 )       _Activate = "Device inactive"; 
        else if ( Activate == 1 )       _Activate = "Device active"; 
        else                            _Activate = "wrong status active";
//  String _topicMode = _topic + "/Mode"; 
//        if      ( Mode == 1 )           _Mode = "Automatic mode"; 
//        else if ( Mode == 2 )           _Mode = "Always on"; 
//        else if ( Mode == 3 )           _Mode = "Always off"; 
//        else                            _Mode = "wrong status mode";
//  String _topicInvAct = _topic + "/InvertActivate"; 
//        if      ( invertActivate == 0 ) _invertActivate = "Invert mode inactive"; 
//        else if ( invertActivate == 1 ) _invertActivate = "Invert mode active"; 
//        else                            _invertActivate = "wrong status invertactive";
  Serial.print ( "Connect to MQTT with SSDP_Name " ); Serial.print ( SSDP_Name ); Serial.println ( " - ACCEPT" ); flagFailMQTTconnect = 0;
  client.publish ( _topicTemp, _Tnow ); Serial.print ( "Publish " ); Serial.print ( _topicTemp ); Serial.print ( " " ); Serial.println ( _Tnow );
  client.publish ( _topicActiv, _Activate ); Serial.print ( "Publish " ); Serial.print ( _topicActiv ); Serial.print ( " " ); Serial.println ( _Activate );
  //client.publish ( _topicMode, _Mode ); Serial.print ( "Publish " ); Serial.print ( _topicMode ); Serial.print ( " " ); Serial.println ( _Mode );
  //client.publish ( _topicInvAct, _invertActivate ); Serial.print ( "Publish " ); Serial.print ( _topicInvAct ); Serial.print ( " " ); Serial.println ( _invertActivate );
  client.set_callback ( callback );
  client.subscribe ( _topicActiv );
//  Serial.println ( client.subscribe ( _topicActiv ) );
  flagFailMQTTconnect = 0;
}


void callback(const MQTT::Publish& pub) 
{
  String _topic = "/SmartHome"; _topic += pubtopic; _topic += subtopic;
  String _topicActiv = _topic + "/Activate"; 
  String payload = pub.payload_string();
  if ( String ( pub.topic () ) == _topicActiv )      // Проверяем из нужного ли нам топика пришли данные
  {
    String Variable = payload;                       // Преобразуем полученные данные 
    Serial.print ( "Incoming mesage " ); Serial.print ( Variable );                     // Отправляем данные по COM порту 
    if      ( Variable == ComandON   ) 
    { 
      Activate = 1; 
      jsonWrite ( Variables, "Activate", Activate ); 
      saveVariables (); 
      Serial.println (); 
    }
    else if ( Variable == ComandOFF ) 
    { 
      Activate = 0; 
      jsonWrite ( Variables, "Activate", Activate ); 
      saveVariables ();
      Serial.println ( Variable );  
    }
  }
}
