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

		int getSonMin();
		int getSonMax();
		int getSonMoy();
		int getGaz();
		int getLumiere();
		float getTemperature();
		float getHumidity();
		
		char* getJSON();
		void appliquerSeuil(XMNData* previousXMNData);
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