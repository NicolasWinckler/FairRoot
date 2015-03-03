/* 
 * File:   test.cxx
 * Author: winckler
 *
 * Created on February 23, 2015, 8:22 PM
 */



#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "OscAnaManager.h"
#include "OscSimulation.h"
#include "OscDataSet.h"

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
using namespace std;


int main(int argc, char** argv)
{
    OscAnaManager* config = new OscAnaManager();
    try 
    {
        config->ParseAll(argc,argv);
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }    
    
    
    
    std::string filename=config->GetPar<std::string>("sim.file.output");
    std::string dataname="test";
    double SampleSize=2000.;
    double Iteration=2.;
    
    {
        OscSimulation* sim = new OscSimulation(config);
        sim->GenerateData(dataname,SampleSize,Iteration);
        sim->SaveSimData(filename);
        delete sim;
    }
    
    {
        OscSimulation* sim = new OscSimulation(config);
        sim->LoadSimData(filename,dataname);
        sim->ComputeMLEDistribution(SampleSize,Iteration,true);
        
        //sim->
        
        delete sim;
    }
    
    
    delete config;
    return 0;
}


