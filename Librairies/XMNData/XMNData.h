#ifndef XMNData_h
#define XMNData_h

class XMNData {
	public:
		XMNData(unsigned int iteration);
		
		void setTemperatureEau(float temperatureEau);
		void setTemperature(double temperature);
		void setHumidity(double humidity);
		
		char* getJSON();
		
	private:
		unsigned int _iteration;
		float _temperatureEau;
		double _temperature;
		double _humidity;
};


#endif