/*!
 * Author:  José Magno MENDES FILHO
 * Site:    http://www.ensta-paristech.fr/~mendesfilho/public/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/
 */
#ifndef COMMON_H
#define COMMON_H

#include <string>

#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#define foreach_ BOOST_FOREACH

class Observer;
class Sensor;

namespace Common
{
	typedef struct _obsParam_t
	{
		Observer *observer;
		Sensor *sensor;
		std::string *sysModelfilename;
	} obsParam_t;

	/**
     * @brief
     * Return the Fast Inverse Square Root.
     */
    double finvsqrt (double number);

	/**
     * @brief
     * Return Fast Square Root.
     */
    inline double fsqrt (double number) {return 1.0/finvsqrt(number);}

	/**
     * @brief
     * Returns the real time, in seconds, or -1.0 if an error occurred.
     *
     * Time is measured since an arbitrary and OS-dependent start time.
     * The returned real time is only useful for computing an elapsed time
     * between two calls to this function.
     */
    double getRealTime ();
}

#endif // COMMON_H
