#include "sysmodelparser.h"

SysModelParser::SysModelParser (const std::string & XMLFILEPATH)
{
	// Create an empty property tree object
	using boost::property_tree::ptree;
	ptree sysmodel;

	// Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
	// TODO change to try catch blabla
	read_xml (XMLFILEPATH, sysmodel);

	this->process.x_dim = sysmodel.get<int> ("SystemModel.Process.x_dim");
	this->process.u_dim = sysmodel.get<int> ("SystemModel.Process.u_dim");
	this->measure.y_dim = sysmodel.get<int> ("SystemModel.Measurement.y_dim");
	this->noise.v_dim = sysmodel.get<int> ("SystemModel.Noise.v_dim");
	this->noise.w_dim = sysmodel.get<int> ("SystemModel.Noise.w_dim");

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Process.x_t0"))
	{
		this->process.xt0.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Process.u_t0"))
	{
		this->process.ut0.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Process.f"))
	{
		this->process.f_fun.push_back(v.second.data());
	}
	
	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Process.tr"))
	{
		this->process.tr_fun.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Measurement.g"))
	{
		this->measure.g_fun.push_back(v.second.data());
	}
	
	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Noise.Ev"))
	{
		this->noise.Ev.push_back(v.second.data());
	}
	
	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Noise.Q"))
	{
		this->noise.Q.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Noise.Ew"))
	{
		this->noise.Ew.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.Noise.R"))
	{
		this->noise.R.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.CrossCorrelation.Pxv"))
	{
		this->crosscorr.Pxv.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.CrossCorrelation.Pxw"))
	{
		this->crosscorr.Pxw.push_back(v.second.data());
	}

	foreach_(ptree::value_type &v,
			sysmodel.get_child("SystemModel.CrossCorrelation.Pvw"))
	{
		this->crosscorr.Pvw.push_back(v.second.data());
	}
	write_xml ("./config/auto_gen_xmlfile.xml", sysmodel);
}

SysModelParser::~SysModelParser () {}
