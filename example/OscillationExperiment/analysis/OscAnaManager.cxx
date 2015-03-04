/* 
 * File:   OscAnaManager.cxx
 * Author: winckler
 * 
 * Created on February 22, 2015, 7:56 PM
 */

#include "OscAnaManager.h"

OscAnaManager::OscAnaManager() : IOManager(), 
                                fObservable_options("Observable Options"),
                                fModel_options("Model Options"),
                                fBAT_options("BAT Options"),
                                fSim_options("Simulation Options")
{
    InitOptionDescription();
    
    fConfigDesc.add(fObservable_options)
               .add(fModel_options)
               .add(fBAT_options)
               .add(fSim_options);
    
    fCmdline_options.add(fGenericDesc)
                    .add(fHiddenDesc);
    
    fConfig_file_options.add(fConfigDesc)
                        .add(fHiddenDesc);
    
    fVisible_options.add(fGenericDesc)
                    .add(fConfigDesc);
}


OscAnaManager::~OscAnaManager() 
{
}

int OscAnaManager::ParseAll(const int argc, char** argv, bool AllowUnregistered)
{
    
    IOManager::ParseCmdLine(argc,argv,fCmdline_options,fvarmap,AllowUnregistered);
    
    if (fvarmap.count("help")) 
    {
        std::cout << fVisible_options << "\n";
        return 0;
    }
    
    if (fvarmap.count("version")) 
    {
        std::cout << "alpha version 0.0\n";
        return 0;
    }
    
    if(IOManager::ParseCfgFile(fConfigFile,fConfig_file_options,fvarmap,AllowUnregistered)<0)
        return -1;
    
    
    
    PrintConfig();
    return 0;
}

void OscAnaManager::InitOptionDescription()
{
    fGenericDesc.add_options()
    ("version", "Version of the executable")
    ("exec", po::value<std::string>()->default_value("Hello-World"), "execute task")
    ;
    
    fConfigDesc.add_options()
    //("input.file-name", po::value<std::string>(&fConfigFile)->required(), "Path to config")
    ("input.data.file.name", po::value<std::string>(&fConfigFile)->default_value("path/to/inputfile.txt"), "Path to config")
    ("input.data.name", po::value< std::string >()->default_value("mydataset"), "Name of the data set.")
    ("input.data.detector", po::value< std::string >()->default_value("detector"), "Detector type")
    ("input.data.preprocess", po::value< std::string >()->default_value("auto"), "Pre-analysis of the data")
    ("input.data.file.format", po::value< std::string >()->default_value("txt"), "Data file format e.g. : root, txt")
    ("input.data.file.tree.name", po::value< std::string >()->default_value("treename"), "Name of tree (if root format)")
    ("input.data.file.branch.name", po::value< std::string >()->default_value("branchname"), "Name of the branch (if root format)")
    ("output.directory", po::value< std::string >()->default_value(""), "Path to the ouput directory.")
    ("output.data.file.name", po::value<std::string>(&fConfigFile)->default_value("path/to/inputfile.txt"), "Path to config")
    ("output.data.file.format", po::value< std::string >()->default_value("txt"), "output Data file format e.g. : root, txt")
    ("output.data.file.tree.name", po::value< std::string >()->default_value("outputtreename"), "output Name of tree (if root format)")
    ("output.data.file.branch.name", po::value< std::string >()->default_value("outputbranchname"), "output Name of the branch (if root format)")
    ;
    
    fObservable_options.add_options()
    ("obs.name", po::value< std::string >()->default_value("Decay time"), "Name of the observable")
    ("obs.xunit", po::value< double >()->default_value(1.0), "xunit")
    ("obs.xoffset", po::value< double >()->default_value(0.0), "xoffset")
    ("obs.xmin", po::value< double >()->default_value(0.0), "xmin")
    ("obs.xmax", po::value< double >()->default_value(100.0), "xmax")
    ("obs.binning", po::value< int >()->default_value(100), "Binning of the observable")
    ("obs.XRangeOption", po::value< std::string >()->default_value("FALSE"), "FALSE //XRangeAuto")
    ;
    
    fModel_options.add_options()
    ("par.lambda.init", po::value< double >()->default_value(0.01), "lambda Init")
    ("par.lambda.min", po::value< double >()->default_value(0.0), "lambda min")
    ("par.lambda.max", po::value< double >()->default_value(1.0), "lambda max")
    ("par.lambda.bin", po::value< int >()->default_value(400), "Binning of lambda")
    ("par.amp.init", po::value< double >()->default_value(1.0), "amp Init")
    ("par.amp.min", po::value< double >()->default_value(-0.25), "a min")
    ("par.amp.max", po::value< double >()->default_value(0.25), "a max")
    ("par.amp.bin", po::value< int >()->default_value(400), "Binning of amplitude")
    ("par.omega.init", po::value< double >()->default_value(1.0), "omega Init")
    ("par.omega.min", po::value< double >()->default_value(0.0), "omega min")
    ("par.omega.max", po::value< double >()->default_value(3.0), "omega max")
    ("par.omega.bin", po::value< int >()->default_value(400), "Binning of omega")
    ("par.phi.init", po::value< double >()->default_value(0.0), "phi Init")
    ("par.phi.min", po::value< double >()->default_value(-3.14), "phi min")
    ("par.phi.max", po::value< double >()->default_value(3.14), "phi max")
    ("par.phi.bin", po::value< int >()->default_value(400), "Binning of phi")
    ;
    
    fBAT_options.add_options()
    ("bat.output.directory", po::value< std::string >()->default_value(""), "Path to the bat ouput directory.")
    ("bat.output.log-file", po::value< std::string >()->default_value("LogOscAnalysis.txt"), "(BAT output) Log file name")
    ("bat.output.PostpdfsM0", po::value< std::string >()->default_value("PostPdfM0.pdf"), "(BAT output) file Post Pdf M0")
    ("bat.output.PostpdfsM1", po::value< std::string >()->default_value("PostPdfM1.pdf"), "(BAT output) file Post Pdf M1")
    ("bat.output.SummaryM0", po::value< std::string >()->default_value("SummaryM0.pdf"),  "(BAT output) file Summary M0")
    ("bat.output.SummaryM1", po::value< std::string >()->default_value("SummaryM1.pdf"),  "(BAT output) file Summary M1")
    ("bat.MCMCPrecision", po::value< std::string >()->default_value("Low"), "MCMC precison flags : 'Low', 'Medium' 'High', 'VeryHigh'")
    ("bat.IntegrationMethod", po::value< std::string >()->default_value("Cuba"), "Integration method\n" "   flags : \n" 
                                                                                "      Default\n" 
                                                                                "      Cuba\n" 
                                                                                "      MonteCarlo\n" 
                                                                                "      Grid\n" 
                                                                                "      CubaVegas")
    ("bat.MarginalizationMethod", po::value< std::string >()->default_value("Metropolis"), "Marginalization method flags : 'Default', Metropolis', 'MonteCarlo', 'Grid'")
    ("bat.CubaRelPrecision", po::value< double >()->default_value(1.0e-3), "Relative precision of the integration")
    ("bat.CubaAbsPrecision", po::value< double >()->default_value(1.0e-17), "Absolute precision of the integration")
    ("bat.CubaNIterationsMin", po::value< int >()->default_value(1000), "Min iteration of Cuba MC integrator")
    ("bat.CubaNIterationsMax", po::value< int >()->default_value(100000000), "Min iteration of Cuba MC integrator")
    ("bat.M0PriorSet", po::value< std::string >()->default_value("Constant"), "name of parameter prior set e.g. Constant, gaussian pdf")
    ("bat.M1PriorSet", po::value< std::string >()->default_value("Constant"), "name of parameter prior set e.g. Constant, gaussian pdf")
    ;
    
    fSim_options.add_options()
    ("sim.event.number", po::value< int >()->default_value(1000), "Number of data point for MC simulation (data sample size)")
    ("sim.iteration.offset", po::value< int >()->default_value(0), "Index offset in data")
    ("sim.iteration.number", po::value< int >()->default_value(1000), "Number of iteration to compute a distribution (stat sample size )")
    ("sim.file.input", po::value< std::string >()->default_value("SimInput.root"), "name of simulation input file")
    ("sim.file.input.dataname", po::value< std::string >()->default_value("SimData"), "name of simulated data simulation input file")
    ("sim.file.output", po::value< std::string >()->default_value("SimOutput.root"), "name of simulation output file")
    ("sim.file.output.dir", po::value< std::string >()->default_value("SimInput.root"), "name of simulation input file")
    ("sim.file.output.tree", po::value< std::vector<std::string> >()->composing(), "name of simulation output file")
    ("sim.file.output.branch", po::value< std::vector<std::string> >()->composing(), "name of simulation input file")
    ;
}