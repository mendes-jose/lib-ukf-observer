/*!
 * Author:  José Magno MENDES FILHO
 * Site:    http://www.ensta-paristech.fr/~mendesfilho/public/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/
 */
#ifndef SENSOR_H
#define SENSOR_H

#include "common.h"

// body file: sensor.cpp

class Observer;

class Sensor
{
private:
	Eigen::Matrix<double, 2, 1> y;
public:
	Sensor ();
	~Sensor ();

	inline bool online () {return true;}
	inline const Eigen::Matrix<double, 2, 1>& getMeasurement () const
	{
		return this->y;
	}
	
};

#endif // SENSOR_H
