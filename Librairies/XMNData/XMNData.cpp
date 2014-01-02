#include "XMNData.h"
//#include "FormatDouble.h"
#include "aJSON.h"

//Constructeur
XMNData::XMNData(unsigned int iteration) {
	_iteration = iteration;
	_sonMin=0;
	_sonMax=0;
	_sonMoy=0;
	_gaz=3;
	_lumiere=120;
	_temperature=21;
	_humidity=60;
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




/* sonMin */
int XMNData::getSonMin() {
	return _sonMin;
}

/* sonMax */
int XMNData::getSonMax() {
	return _sonMax;
}

/* sonMoy */
int XMNData::getSonMoy() {
	return _sonMoy;
}

/* sonGaz */
int XMNData::getGaz() {
	return _gaz;
}

/* lumiere */
int XMNData::getLumiere() {
	return _lumiere;
}

/* getTemperature */
float XMNData::getTemperature() {
	return _temperature;
}
/* getHumidity */
float XMNData::getHumidity() {
	return _humidity;
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

/*  Suppression des valeurs trop haute ou basse 
	On les remplace par les dernieres valeurs capturées
	Si les valeurs sont bonnes alors on les sauvegardes comme valeurs precedemment capturées / ou en valeurs max
	NB: la sonde de temperature et humidité propose des -1000 par moment... d'ou ce code.
*/
void XMNData::appliquerSeuil(XMNData* previousXMNData) {
	  //Patch car certaine mesures etaient a -1000
  if ((_temperature > 60) or (_temperature < -15)) _temperature =  previousXMNData->getTemperature();
  else  previousXMNData->setTemperature(_temperature);
  
  if ((_humidity > 100) or (_humidity < 0)) _humidity = previousXMNData->getHumidity();
  else previousXMNData->setHumidity(_humidity);
  
  if (_lumiere > 200) _lumiere = 200;
  if (_lumiere < 0) _lumiere = 0;
  
  if (_sonMin > 80) _sonMin = 80;
  if (_sonMin < 0) _sonMin = 0;

  if (_sonMax > 80) _sonMax = 80;
  if (_sonMax < 0) _sonMax = 0;

  if (_sonMoy > 80) _sonMoy = 80;
  if (_sonMoy < 0) _sonMoy = 0;
}
 