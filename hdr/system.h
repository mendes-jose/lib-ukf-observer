/*!
 * Author:  José Magno MENDES FILHO
 * Site:    http://www.ensta-paristech.fr/~mendesfilho/public/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/
 */
#ifndef SYSTEM_H
#define SYSTEM_H

class Observer
{
private:
	MatrixXd x, x_hist;
	MatrixXd y;
	MatrixXd Q, R;
	inline virtual MatrixXd processFunc ();
	inline virtual MatrixXd measurementFunc ();
	inline virtual void processNoiseCovMatrix ();
	inline virtual void measurementNoiseCovMatrix ();
	inline virtual void getMeasurement ();

protected:

public:
	System ();
	virtual ~System ()=0;

	virtual void runObserver ();
	inline MatrixXd getState () {return x};
}

#endif // SYSTEM_H
