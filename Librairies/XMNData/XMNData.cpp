#include "XMNData.h"
//#include "FormatDouble.h"
#include "aJSON.h"

//Constructeur
XMNData::XMNData(unsigned int iteration) {
	_iteration = iteration;
}

/* setTemperatureEau */
void XMNData::setTemperatureEau(float temperatureEau) {
	_temperatureEau = temperatureEau;
}

/* setTemperature */
void XMNData::setTemperature(double temperature) {
	_temperature = temperature;
}
/* setHumidity */
void XMNData::setHumidity(double humidity) {
	_humidity = humidity;
}

aJsonObject *root;

char* XMNData::getJSON() {
	/*char message[100] = "json";
	

	// http://forum.arduino.cc/index.php/topic,148214.0.html
	// Format a floating point value with number of decimal places.
	// The 'precision' parameter is a number from 0 to 6 indicating the desired decimal places.
	// The 'buf' parameter points to a buffer to receive the formatted string.  This must be
	// sufficiently large to contain the resulting string.  The buffer's length may be
	// optionally specified.  If it is given, the maximum length of the generated string
	// will be one less than the specified value.
  	fmtDouble(_temperatureEau, 2, buf, sizeof buf);
  	strcat(message,buf);
  	strcat(message,"-testEncore");
  	strcat(message,"-test2");*/

 	
 	root=aJson.createObject(); 
 	aJson.addNumberToObject(root,"iteration", (int)_iteration);
 	aJson.addNumberToObject(root,"temperatureEau",_temperatureEau);
 	aJson.addNumberToObject(root,"temperature",_temperature);
 	aJson.addNumberToObject(root,"humidity",_humidity);
	char* buf = aJson.print(root);
 	aJson.deleteItem(root);
  	return buf;
}
 