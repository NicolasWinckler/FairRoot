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

    std::string execute=config->GetPar<std::string>("exec");
    if(execute=="Hello-World")
    {
        std::cout<<"Hello-World\n";
        config->PrintHelp();
        delete config;
        return 0;
    }
    
    
    std::string dataname=config->GetPar<std::string>("sim.file.input.dataname");
    std::string filename=config->GetPar<std::string>("sim.file.input");
    
    double SampleSize=config->GetPar<int>("sim.event.number");
    double Iteration=config->GetPar<int>("sim.iteration.number");
    
    
    if(execute=="sim.generate")
    {
        OscSimulation* sim = new OscSimulation(config);
        sim->GenerateData(dataname,SampleSize,Iteration);
        sim->SaveSimData(filename);
        delete sim;
    }
    

    if(execute=="sim.dist.LRT")
    {
        OscSimulation* sim = new OscSimulation(config);
        sim->LoadSimData(filename,dataname);
        std::string simoutdir=config->GetPar<std::string>("sim.file.output.dir");
        std::string simDistOutFile(simoutdir);
        simDistOutFile+="DistributionTest2.root";
        sim->ComputeMLEDistribution(simDistOutFile,SampleSize,Iteration,true);
        delete sim;
    }
    

    delete config;
    return 0;
}


