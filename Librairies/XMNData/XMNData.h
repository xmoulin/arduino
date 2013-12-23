#ifndef XMNData_h
#define XMNData_h

class XMNData {
	public:
		XMNData(unsigned int iteration);
		
		void setSonMin(int sonMin);
		void setSonMax(int sonMax);
		void setSonMoy(int sonMoy);
		void setGaz(int gaz);
		void setLumiere(int lumiere);
		void setTemperature(float temperature);
		void setHumidity(float humidity);
		
		char* getJSON();
		
	private:
		unsigned int _iteration;
		int _sonMin;
		int _sonMax;
		int _sonMoy;
		int _gaz;
		int _lumiere;
		float _temperature;
		float _humidity;
};


#endif