#include "XMNData.h"
//#include "FormatDouble.h"
#include "aJSON.h"

//Constructeur
XMNData::XMNData(unsigned int iteration) {
	_iteration = iteration;
	_sonMin=0;
	_sonMax=0;
	_sonMoy=0;
	_gaz=0;
	_lumiere=0;
	_temperature=0;
	_humidity=0;
}

/* sonMin */
void XMNData::setSonMin(int sonMin) {
	_sonMin = sonMin;
}

/* sonMax */
void XMNData::setSonMax(int sonMax) {
	_sonMax = sonMax;
}

/* sonMoy */
void XMNData::setSonMoy(int sonMoy) {
	_sonMoy = sonMoy;
}

/* sonGaz */
void XMNData::setGaz(int gaz) {
	_gaz = gaz;
}

/* lumiere */
void XMNData::setLumiere(int lumiere) {
	_lumiere = lumiere;
}

/* setTemperature */
void XMNData::setTemperature(float temperature) {
	_temperature = temperature;
}
/* setHumidity */
void XMNData::setHumidity(float humidity) {
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
 	aJson.addNumberToObject(root,"sonMin",_sonMin);
 	aJson.addNumberToObject(root,"sonMax",_sonMax);
 	aJson.addNumberToObject(root,"sonMoy",_sonMoy);
 	aJson.addNumberToObject(root,"gaz",_gaz);
 	aJson.addNumberToObject(root,"lumiere",_lumiere);
 	aJson.addNumberToObject(root,"temperature",_temperature);
 	aJson.addNumberToObject(root,"humidity",_humidity);
	char* buf = aJson.print(root);
 	aJson.deleteItem(root);
  	return buf;
}
 