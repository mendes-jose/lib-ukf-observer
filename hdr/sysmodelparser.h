/*!
 * Author:  José Magno MENDES FILHO
 * Site:    http://www.ensta-paristech.fr/~mendesfilho/public/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/
 */
#ifndef SYSMODELPARSER_H
#define SYSMODELPARSER_H

#include "common.h"

class SysModelParser
{
private:
	struct _process
	{
		int x_dim;
		int u_dim;
		std::vector<std::string> xt0;
		std::vector<std::string> ut0;
		std::vector<std::string> f_fun;
		std::vector<std::string> tr_fun;
	};
	struct _measure
	{
		int y_dim;
		std::vector<std::string> g_fun;
	};
	struct _noise
	{
		int v_dim;
		int w_dim;
		std::vector<std::string> Ev;
		std::vector<std::string> Ew;
		std::vector<std::string> Q;
		std::vector<std::string> R;
	};
	struct _crosscorr
	{
		std::vector<std::string> Pxv;
		std::vector<std::string> Pxw;
		std::vector<std::string> Pvw;
	};

public:
	SysModelParser(const std::string &);
	~SysModelParser();

	struct _process process;
	struct _measure measure;
	struct _noise noise;
	struct _crosscorr crosscorr;
};

#endif // SYSMODELPARSER_H
