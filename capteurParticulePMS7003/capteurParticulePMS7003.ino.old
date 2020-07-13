#include "PMS.h"
/*
 * Serial and Serial1 are completely different interfaces - one hasn’t got anything to do with the other!
While Serial (the only one that can be seen on Serial Monitor) communicates via the USB port and a Virtual COM protocol (hence you can actually use any baud rate - 
the speed will always be the same USB speed), Serial1 only communicates via the RX/TX pins and there the baud rate has to match the baud rate of your connected device
 * 
 */
PMS pms(Serial);
PMS::DATA data;

void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
}

void loop()
{
  if (pms.read(data))
  {
    Serial1.print("PM 1.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_1_0);

    Serial1.print("PM 2.5 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_2_5);

    Serial1.print("PM 10.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_10_0);

    Serial1.println();
  }

  // Do other stuff...
}
