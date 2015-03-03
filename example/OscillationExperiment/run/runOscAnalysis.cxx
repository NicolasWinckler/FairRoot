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
    std::cout<<"OK\n";
    std::string execute=config->GetPar<std::string>("exec");
    std::cout<<"OK1\n";
    if(execute=="Hello-World")
    {
        std::cout<<"Hello-World\n";
        config->PrintHelp();
        delete config;
        return 0;
    }
    
    
    std::string dataname="test";
    std::string filename=config->GetPar<std::string>("sim.file.output");
    
    double SampleSize=config->GetPar<int>("sim.event.number");
    double Iteration=config->GetPar<int>("sim.iteration.number");
    std::cout<<"OK2\n";
    std::cout<<"exec = = "<<execute<<std::endl;
    if(execute=="sim.generate")
    {std::cout<<"exec = = "<<execute<<std::endl;std::cout<<"OK2-a\n";
        OscSimulation* sim = new OscSimulation(config);
        sim->GenerateData(dataname,SampleSize,Iteration);std::cout<<"OK2-b\n";
        sim->SaveSimData(filename);std::cout<<"OK2-3\n";
        delete sim;
    }
    
    std::cout<<"OK3\n";
    if(execute=="sim.dist.LRT")
    {
        OscSimulation* sim = new OscSimulation(config);
        sim->LoadSimData(filename,dataname);
        std::string simoutdir=config->GetPar<std::string>("sim.file.output.dir");
        std::string simDistOutFile(simoutdir);
        simDistOutFile+="DistributionTest.root";
        sim->ComputeMLEDistribution(simDistOutFile,SampleSize,Iteration,true);
        delete sim;
    }
    
    std::cout<<"OK4\n";
    delete config;
    return 0;
}


