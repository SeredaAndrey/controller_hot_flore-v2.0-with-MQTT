void Activate_init ()
{
  if ( currentMillis - ActivateRefreshMillis  >= ActivateRefresh  ) 
  { 
    ActivateRefreshMillis =  currentMillis;  
    if      ( Mode == 1 && Activate == 0 && invertActivate == 0 ) { digitalWrite( PinRelay, LOW  ); }
    else if ( Mode == 1 && Activate == 0 && invertActivate == 1 ) { digitalWrite( PinRelay, HIGH ); } 
    else if ( Mode == 1 && Activate == 1 && invertActivate == 0 ) { digitalWrite( PinRelay, HIGH ); }
    else if ( Mode == 1 && Activate == 1 && invertActivate == 1 ) { digitalWrite( PinRelay, LOW  ); } 
    else if ( Mode == 2 &&                  invertActivate == 0 ) { digitalWrite( PinRelay, HIGH ); }
    else if ( Mode == 2 &&                  invertActivate == 1 ) { digitalWrite( PinRelay, LOW  ); }  
    else if ( Mode == 3 &&                  invertActivate == 0 ) { digitalWrite( PinRelay, LOW  ); }
    else if ( Mode == 3 &&                  invertActivate == 1 ) { digitalWrite( PinRelay, HIGH ); } 
    yield();
  } 
}

void Led_init()
{
  int StatusLed;
  if      ( Mode == 1 ) StatusLed = 3;
  else if ( Mode == 2 ) StatusLed = 4;
  else if ( Mode == 3 ) StatusLed = 0;
  else                  StatusLed = 6;
  if ( StatusLed !=0 ) 
  {
    static unsigned long prevTime;
    static int blink_loop; 
    byte blink_mode = modes [ StatusLed ];
    if ( millis () - prevTime > 125 )
    {
      prevTime=millis();
      if ( blink_mode & 1 << ( blink_loop&0x07 ) ) digitalWrite ( PinLed, LOW ); // Режим светодиода ищем по битовой маске
      else  digitalWrite ( PinLed, HIGH );
      blink_loop++; 
      if ( blink_loop > 8 ) 
      {
        blink_loop = 0;
        StatusLed = 0;
        digitalWrite( PinLed, HIGH );  
      }
    }
  } 
}
