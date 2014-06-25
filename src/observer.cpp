#include "observer.h"

Observer::Observer () {}
Observer::~Observer () {}

void Observer::runObserver (void *param)
{
	int f, g, h, i, j;
	/* *************************************************************************
		Casting parameter
	*/
	Common::obsParam_t *aux = static_cast<Common::obsParam_t*> (param);

	Observer *thisObs = aux->observer;
	Sensor *sensor = aux->sensor;
	std::string *sysModelfilename = aux->sysModelfilename; 

	/* *************************************************************************
		Parsing the system model XML file and resizing matrices appropriately
	*/
	SysModelParser sysModel (*sysModelfilename);
	
	thisObs->x.resize (sysModel.process.x_dim);

	int xd = sysModel.process.x_dim;
	int vd = sysModel.noise.v_dim;
	int wd = sysModel.noise.w_dim;
	int yd = sysModel.measure.y_dim;

	int augStateDim = xd + vd + wd;

	MatrixXd Sigma (augStateDim, 2*augStateDim+1 +1);

	MatrixXd P_augXaug (augStateDim, augStateDim);
	P_augXaug.setZero();
	P_augXaug.diagonal().setConstant(1);
	P_augXaug.block(0,0,4,4).diagonal().setConstant(0);
	MatrixXd sqrt_P (P_augXaug);
	
	VectorXd y (yd);
	MatrixXd Gy (yd,2*augStateDim+1 +1);

	MatrixXd CovGy_ydXyd (yd,yd);
	MatrixXd CovSigGy_augXyd (augStateDim,yd);
	MatrixXd L_augXyd (augStateDim, yd);

	VectorXd u(sysModel.process.u_dim);
	u.setZero();

	/* *************************************************************************
		Creaing a math expressions parser and setting variables, constants and
		functions
	*/

	mu::Parser p;
	std::ostringstream varName;
//	for (int i=0; i < xd; i++)
//	{
//		varName << "x" << i+1;
//		p.DefineVar(varName.str(), &xa(i));
//		varName.str(""); // clear the string stream
//	}
//	for (int i=0; i < sysModel.noise.v_dim; i++)
//	{
//		varName << "v" << i+1;
//		p.DefineVar(varName.str(), &xa(i+xd));
//		varName.str(""); // clear the string stream
//	}
//	for (int i=0; i < sysModel.noise.w_dim; i++)
//	{
//		varName << "w" << i+1;
//		p.DefineVar(varName.str(), &xa(i+xd+
//				sysModel.noise.v_dim));
//		varName.str(""); // clear the string stream
//	}
	// Set intial values
	for (h=0; h < xd; h++)
	{
		p.SetExpr(sysModel.process.xt0[h]);
		Sigma(h,0) = p.Eval();
	}
	for (g=0; g < vd; g++)
	{
		p.SetExpr(sysModel.noise.Ev[g]);
		Sigma(g+h,0) = p.Eval();
	}
	for (f=0; f < wd; f++)
	{
		p.SetExpr(sysModel.noise.Ew[f]);
		Sigma(f+g+h,0) = p.Eval();
	}

	for (i=0; i < sysModel.process.u_dim; i++)
	{
		varName << "u" << i+1;
		p.DefineVar(varName.str(), &u(i));
		varName.str(""); // clear the string stream
	}
	for (i=0; i < yd; i++)
	{
		varName << "y" << i+1;
		p.DefineVar(varName.str(), &y(i));
		varName.str(""); // clear the string stream
	}
    double t, dt=0.015; //TODO cte 
	p.DefineVar("T", &dt);
	p.DefineVar("dT", &dt);
	p.DefineVar("dt", &dt);
	p.DefineFun("finvsqrt", Common::finvsqrt);
	p.DefineFun("fsqrt", Common::fsqrt);
	// TODO import new functions created by user


//	MatrixXd Q (sysModel.noise.v_dim,sysModel.noise.v_dim);
//	MatrixXd R (sysModel.noise.w_dim,sysModel.noise.w_dim);
//	MatrixXd Pxv (xd,sysModel.noise.v_dim);
//	MatrixXd Pxw (xd,sysModel.noise.w_dim);
//	MatrixXd Pvw (sysModel.noise.v_dim,sysModel.noise.w_dim);
	
	// TODO
	const double W0 = -1;
	const float W_ = ((1-W0)/2.0/augStateDim);


    double t0;
	do
	{
		thisObs->getMeasurement (sensor); // thisObs->y is updated
		t0 = t = Common::getRealTime();
	}while (sensor->online() != true);

	/* *************************************************************************
		Update the state estimate at each iteration
	*/
	while (sensor->online() == true)
	{

		/* *********************************************************************
        	SENSOR MEASUREMENT ACQUISITION
        */
		// Paralelizar TODO, ler os sensores numa nova thread e esperar a thread
		// quando o valor for necessario.
		thisObs->getMeasurement (sensor); // thisObs->y is updated
		
		// Compute Elapsed Time
        dt = Common::getRealTime() - t; // seconds
        t = dt + t;
//		std::cout << "tempo " << dt << std::endl;

		/* *********************************************************************
        	SELECT SIGMA POINTS
        */
		sqrt_P = P_augXaug.llt().matrixL();
		sqrt_P = Common::finvsqrt((1-W0)/augStateDim) * sqrt_P;
		std::cout << sqrt_P << std::endl;
		Sigma.block(0,1,augStateDim,augStateDim) = sqrt_P.colwise() + Sigma.col(0);
		Sigma.block(0,1+augStateDim,augStateDim,augStateDim) =
				(-1*sqrt_P).colwise() + Sigma.col(0);

		std::cout << Sigma << std::endl;
		getchar();

		/* *********************************************************************
        	PROJECT AHEAD (TIME UPDATE)
        */

		for (j=0; j < 2*augStateDim+1; j++)
		{
			for (i=0; i < xd; i++)
			{
				varName << "x" << i+1;
				p.DefineVar(varName.str(), &Sigma(i,j));					
				varName.str(""); // clear the string stream
			}
			for (i=0; i < vd; i++)
			{
				varName << "v" << i+1;
				p.DefineVar(varName.str(), &Sigma(xd+i,j));
				varName.str(""); // clear the string stream
			}
			for (i=0; i < wd; i++)
			{
				varName << "w" << i+1;
				p.DefineVar(varName.str(), &Sigma(vd+xd+i,j));
				varName.str(""); // clear the string stream
			}
			for (h=0; h < xd; h++)
			{
				p.SetExpr(sysModel.process.f_fun[h]);
				Sigma(h,j) = p.Eval();
			}
			/* TODO otimizar essa parte fazendo uma verficação inicial da
			precença de simbolos nas expressoes, pq se nao tiver simbolos não
			precisa atualzar, o x_k é constante (Ex: os erros serão quase sempre
			constantes) */
			for (g=0; g < sysModel.noise.v_dim; g++)
			{
				p.SetExpr(sysModel.noise.Ev[g]);
				Sigma(g+h,j) = p.Eval();
			}
			for (f=0; f < sysModel.noise.w_dim; f++)
			{
				p.SetExpr(sysModel.noise.Ew[f]);
				Sigma(f+g+h,j) = p.Eval();
			}
		}
		// Sigma_bar
		Sigma.col(2*augStateDim+1) = W0*Sigma.col(0) +
				W_*Sigma.block(0,1,augStateDim,augStateDim*2).rowwise().sum();

//		std::cout << Sigma << std::endl;
//		getchar();

		for (i=0; i < augStateDim; i++)
		{
			varName << "x" << i+1;
			p.DefineVar(varName.str(), &Sigma(i,0));					
			varName.str(""); // clear the string stream
		}
		P_augXaug.block(0,0,xd,xd) =
				W0*
				(Sigma.block(0,0,xd,1) -
				Sigma.block(0,2*augStateDim+1,xd,1))*((Sigma.block(0,0,xd,1) -
				Sigma.block(0,2*augStateDim+1,xd,1)).transpose()) +
				W_*
				(Sigma.block(0,0,xd,augStateDim*2).colwise() -
				Sigma.col(2*augStateDim+1).head(xd)).matrix()*
				((Sigma.block(0,0,xd,augStateDim*2).colwise() -
				Sigma.col(2*augStateDim+1).head(xd)).transpose().matrix());
		for (i=0; i < xd; i++)
		{
			for (j=0; j < vd; j++)
			{
				p.SetExpr(sysModel.crosscorr.Pxv[i*vd+j]);
				P_augXaug(i,xd+j) = p.Eval();
			}
			for (j=0; j < wd; j++)
			{
				p.SetExpr(sysModel.crosscorr.Pxw[i*wd+j]);
				P_augXaug(i,vd+xd+j) = p.Eval();
			}
		}
		for (i=0; i < vd; i++)
		{
			for (j=0; j < wd; j++)
			{
				p.SetExpr(sysModel.crosscorr.Pvw[i*wd+j]);
				P_augXaug(xd+i,vd+xd+j) = p.Eval();
			}
		}
		P_augXaug.block(xd,0,vd,xd) = P_augXaug.block(0,xd,xd,vd).transpose();
		P_augXaug.block(xd+vd,0,wd,xd) = P_augXaug.block(0,xd+vd,xd,wd).transpose();
		P_augXaug.block(xd+vd,xd,wd,vd) = P_augXaug.block(xd,xd+vd,vd,wd).transpose();
		for (i=0; i < vd; i++)
		{
			for (j=0; j < vd; j++)
			{
				p.SetExpr(sysModel.noise.Q[i*vd+j]);
				P_augXaug(xd+i,xd+j) = p.Eval();
			}
		}

		for (i=0; i < wd; i++)
		{
			for (j=0; j < wd; j++)
			{
				p.SetExpr(sysModel.noise.R[i*wd+j]);
				P_augXaug(xd+vd+i,xd+vd+j) = p.Eval();
			}
		}
//		std::cout << P_augXaug << std::endl << std::endl;


		/* *********************************************************************
        	MEASUREMENT UNSCENTED TRANSFORMATION
        */

		// Gy_bar TODO incluir controle u
		for (j=0; j < 2*augStateDim+1; j++)
		{
			for (i=0; i < xd; i++)
			{
				varName << "x" << i+1;
				p.DefineVar(varName.str(), &Sigma(i,j));					
				varName.str(""); // clear the string stream
			}
			for (i=0; i < vd; i++)
			{
				varName << "v" << i+1;
				p.DefineVar(varName.str(), &Sigma(xd+i,j));
				varName.str(""); // clear the string stream
			}
			for (i=0; i < wd; i++)
			{
				varName << "w" << i+1;
				p.DefineVar(varName.str(), &Sigma(vd+xd+i,j));
				varName.str(""); // clear the string stream
			}
			for (h=0; h < yd; h++)
			{
				p.SetExpr(sysModel.measure.g_fun[h]);
				Gy(h,j) = p.Eval();
			}
		}

		Gy.col(2*augStateDim+1) = W0*Gy.col(0) +
				W_*Gy.block(0,1,yd,augStateDim*2).rowwise().sum();

		//CovGy

		CovGy_ydXyd = P_augXaug.block(xd+vd,xd+vd,wd,wd) + W0*(Gy.col(0)-Gy.col(2*augStateDim+1))*
				((Gy.col(0)-Gy.col(2*augStateDim+1)).transpose()) +
				W_*(Gy.block(0,1,yd,augStateDim*2).colwise() -
				Gy.col(2*augStateDim+1))*((Gy.block(0,1,yd,augStateDim*2).colwise() -
				Gy.col(2*augStateDim+1)).transpose());

		//CovSigGy

		CovSigGy_augXyd = W0*(Sigma.col(0) - Sigma.col(2*augStateDim+1))*
				((Gy.col(0)-Gy.col(2*augStateDim+1)).transpose()) +
				W_*(Sigma.block(0,1,augStateDim,augStateDim*2).colwise() -
				Sigma.col(2*augStateDim+1)) *
				((Gy.block(0,1,yd,augStateDim*2).colwise() -
				Gy.col(2*augStateDim+1)).transpose());

		// L = CovSigGy*CovGy.inverser();

		L_augXyd = CovSigGy_augXyd*CovGy_ydXyd.inverse();

		std::cout << "L:\n" << L_augXyd << std::endl;

		std::cout << "Y:\n" << thisObs->y << std::endl;

		Sigma.col(0) += L_augXyd * ( thisObs->y - Gy.col(0) );

		std::cout << "ASDFASDF:\n" << Sigma.col(0) << std::endl;
		P_augXaug -= L_augXyd*CovSigGy_augXyd.transpose();
		
		// carry Sigma.col(0) thought the tr function

		for (i=0; i < xd; i++)
		{
			varName << "x" << i+1;
			p.DefineVar(varName.str(), &Sigma(i,0));					
			varName.str(""); // clear the string stream
		}

		// pragma parallel for bla bla bla
//		for (i=0; i < xd; i++)
//		{
//			p.SetExpr(sysModel.process.tr_fun[i]);
//			thisObs->x(i) = p.Eval();
//		}

		thisObs->x = Sigma.col(0).head(xd);

//		Sigma.col(0) = Sigma.col(2*augStateDim+1);

//		foreach_(std::string & iExp, sysModel.measure.g_fun)
//		{
//			
//		}

		std::cout << "X:\n" << thisObs->x << std::endl;
		std::cout << "Sigma.col(0):\n" << Sigma.col(0) << std::endl;
		std::cout << "TEMPO###" << dt << std::endl;
//		std::cout << sqrt_P << std::endl<< std::endl<< std::endl;
//		std::cout << sqrt_P*sqrt_P.transpose() << std::endl<< std::endl<< std::endl;
//		thisObs->processFunc ();
//		thisObs->processNoiseCovMatrix ();
//		thisObs->measurementNoiseCovMatrix ();
//		thisObs->measurementFunc ();
		// filtro
	}
}

//MatrixXd Observer::processFunc ()
//{
//	
//}
//MatrixXd Observer::measurementFunc ();
//void Observer::processNoiseCovMatrix ();
//void Observer::measurementNoiseCovMatrix ();

void Observer::getMeasurement (const Sensor * sensor)
{
	this->y = sensor->getMeasurement();
}

	
//	std::cout << "x1_{k+1} = " << sysModel.process.f_fun[0] << std::endl;
//	std::cout << "x2_{k+1} = " << sysModel.process.f_fun[1] << std::endl;
//	std::cout << "x3_{k+1} = " << sysModel.process.f_fun[2] << std::endl;
//	std::cout << "x4_{k+1} = " << sysModel.process.f_fun[3] << std::endl;

//	std::cout << "x_dim " << sysModel.process.x_dim << std::endl;
//	std::cout << "u_dim " << sysModel.process.u_dim << std::endl;
//	std::cout << "y_dim " << yd << std::endl;
//	std::cout << "v_dim " << sysModel.noise.v_dim << std::endl;
//	std::cout << "w_dim " << sysModel.noise.w_dim << std::endl;

//	thisObs->u.resize (sysModel.process.u_dim);
//	thisObs->v.resize (sysModel.noise.v_dim);
//	thisObs->Q.resize (sysModel.noise.v_dim, sysModel.noise.v_dim);
//	thisObs->y.resize (yd);
//	thisObs->w.resize (sysModel.noise.w_dim);
//	thisObs->R.resize (sysModel.noise.w_dim, sysModel.noise.w_dim);

//	thisObs->x.setConstant(2);
//	thisObs->u.setConstant(1);
//	thisObs->w.setConstant(2);
//	double x1;
//	try
//	{
//		p.SetExpr(sysModel.process.f_fun[0]);
//		x1 = p.Eval();
//		std::cout << "x_1: " << x1 << std::endl;

//		p.SetExpr(sysModel.process.f_fun[1]);
//		std::cout << "x_2: " << p.Eval() << std::endl;
//	}
//	catch (mu::Parser::exception_type &e)
//	{
//		std::cout << e.GetMsg() << std::endl;
//	}

//	thisObs->getMeasurement (sensor); // y is correctly set


//		for(i=0; i < sysModel.process.x_dim; i++)
//		{
//			p.SetExpr(sysModel.process.f_fun[i]);
//			for(int j=0; j < 2*augStateDim+1; j++)
//			{
//				for(int k=0; k < augStateDim; k++)
//				{
//					varName << "x" << k+1;
//					p.DefineVar(varName.str(), &Sigma(k,j));					
//					varName.str(""); // clear the string stream
//				}
//				Sigma(i,j) = p.Eval();
//			}
//		}
//		for(i=0; i < sysModel.noise.v_dim; i++)
//		{
//			p.SetExpr(sysModel.noise.Ev[i]);
//			for(int j=0; j < 2*augStateDim+1; j++)
//			{
//				for(int k=0; k < augStateDim; k++)
//				{
//					varName << "x" << k+1;
//					p.DefineVar(varName.str(), &Sigma(k,j));					
//					varName.str(""); // clear the string stream
//				}
////				std::cout << "x"<<i+1 << "_{k+1} = " << iExp << std::endl;
////				std::cout	<< "1. Sigma(" <<i<<","<<j<<"):"<< Sigma(i,j) << std::endl;
//				Sigma(i+sysModel.process.x_dim,j) = p.Eval();
////				std::cout	<< "2. Sigma(" <<i<<","<<j<<"):"<< Sigma(i,j) << std::endl;
//			}
//		}
//		for(i=0; i < sysModel.noise.w_dim; i++)
//		{
//			p.SetExpr(sysModel.noise.Ew[i]);
//			for(int j=0; j < 2*augStateDim+1; j++)
//			{
//				for(int k=0; k < augStateDim; k++)
//				{
//					varName << "x" << k+1;
//					p.DefineVar(varName.str(), &Sigma(k,j));					
//					varName.str(""); // clear the string stream
//				}
////				std::cout << "x"<<i+1 << "_{k+1} = " << iExp << std::endl;
////				std::cout	<< "1. Sigma(" <<i<<","<<j<<"):"<< Sigma(i,j) << std::endl;
//				Sigma(i+sysModel.process.x_dim+sysModel.noise.v_dim,j) = p.Eval();
////				std::cout	<< "2. Sigma(" <<i<<","<<j<<"):"<< Sigma(i,j) << std::endl;
//			}
//		}


//		P_augXaug.block(0,xd,) = 

//		P_augXaug = W0*(Sigma.col(0) - Sigma.col(2*augStateDim+1))*
//				((Sigma.col(0) - Sigma.col(2*augStateDim+1)).transpose());
//		P_augXaug += W_*(Sigma.block(0,1,augStateDim,augStateDim*2).colwise() - 
//				Sigma.col(2*augStateDim+1))*
//				((Sigma.block(0,1,augStateDim,augStateDim*2).colwise() - 
//				Sigma.col(2*augStateDim+1)).transpose())

