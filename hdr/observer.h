/*!
 * Author:  José Magno MENDES FILHO
 * Site:    http://www.ensta-paristech.fr/~mendesfilho/public/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/
 */
#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"
#include "sensor.h"
#include "sysmodelparser.h"

#include <muParser.h>

// body file: observer.cpp

using namespace Eigen;

class Observer
{
private:
	VectorXd x;
	VectorXd y;
	
//	inline virtual MatrixXd processFunc ();
//	inline virtual MatrixXd measurementFunc ();
//	inline virtual void processNoiseCovMatrix ();
//	inline virtual void measurementNoiseCovMatrix ();
	inline void getMeasurement (const Sensor *);

public:
	Observer ();
	~Observer ();

	inline const VectorXd & getState () const {return this->x;}
	static void runObserver (void *);
};

#endif // SYSTEM_H
