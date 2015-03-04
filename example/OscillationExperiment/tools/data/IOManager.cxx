/* 
 * File:   IOManager.cxx
 * Author: winckler
 * 
 * Created on February 17, 2015, 9:40 AM
 */

#include "IOManager.h"

IOManager::IOManager() : fGenericDesc("Generic options"), fConfigDesc("Configuration"), fHiddenDesc("Hidden options"),
                         fCmdline_options("Cmd line options"), fConfig_file_options("Cfg file options"), fVisible_options("Allowed options"),
                         fPrint(true), fVerboseLvl(0), fConfigFile()
{
    // define generic options
    fGenericDesc.add_options()
    ("help", "produce help")
    ("verbose", po::value<int>(&fVerboseLvl)->default_value(1), "Verbose level")
    ("config,c", po::value<std::string>(&fConfigFile)->default_value("configfile.cfg"), "Path to configuration file")
    ;
    
    // define config options
    // note : maybe the required semantic is too rigid
    //fConfigDesc.add_options()
    //("input-file-name", po::value<std::string>(&fConfigFile)->required(), "Path to config")
    //;
    
    // define hidden options
    fHiddenDesc.add_options()
    ("input.dir.list", po::value< std::vector<std::string> >()->composing(), "input directory list")
    ("input.file.list", po::value< std::vector<std::string> >()->composing(), "input file list")
    ("output.dir.list", po::value< std::vector<std::string> >()->composing(), "output directory list")
    ("output.file.list", po::value< std::vector<std::string> >()->composing(), "output file list")
    ;
    
}

IOManager::~IOManager() 
{
    
}


int IOManager::ParseCmdLine(const int argc, char** argv, const po::options_description& desc, po::variables_map& varmap, bool AllowUnregistered)
{
    // /////////////////////////////////////////////
    //Todo : handle positional option
    //       to allow direct parameter passing 
    //       (i.e without prog --key=val)
    // po::positional_options_description p;
    // p.add("input-file", -1);
    // store(po::command_line_parser(argc, argv).options(fCmdline_options).positional(p).run(), fvarmap);
    // /////////////////////////////////////////////
    
    
    // get options from cmd line and store in variable map
    // here we use command_line_parser instead of parse_command_line, to allow unregistered and positional options
    
    if(AllowUnregistered)
    {
        //po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), varmap);
        po::command_line_parser parser{argc, argv};
        parser.options(desc).allow_unregistered();
        po::parsed_options parsedOptions = parser.run();
        po::store(parsedOptions,varmap);
    }
    else
        po::store(po::parse_command_line(argc, argv, desc), varmap);
    
    // fix options in varmap 
    po::notify(varmap);
    return 0;
}

    

int IOManager::ParseCfgFile(const std::string& filename, const po::options_description& desc, po::variables_map& varmap, bool AllowUnregistered)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs)
    {
        std::cout << "can not open config file: " << filename << "\n";
        return -1;
    }
    else
    {
        po:store(parse_config_file(ifs, desc, AllowUnregistered), varmap);
        po::notify(varmap);
    }
    return 0;
    
    //parse_config_file(std::basic_istream< charT > &, const options_description &, bool allow_unregistered = false);
}


int IOManager::ParseAll(const int argc, char** argv, bool AllowUnregistered)
{
    
    ParseCmdLine(argc,argv,fGenericDesc,fvarmap,AllowUnregistered);
    ParseCfgFile(fConfigFile,fConfigDesc,fvarmap,AllowUnregistered);
    PrintConfig();
    return 0;
}

int IOManager::PrintConfig()
{
    for (const auto& p : fvarmap) 
    {
        std::cout << p.first.c_str() << " = ";
        auto& value = p.second.value();
        
        if(auto v0 = boost::any_cast< std::vector<std::string> >(&value))
            std::cout << *v0;
        else 
            if(auto v1 = boost::any_cast<std::string>(&value))
                std::cout << *v1;
            else
                if(auto v2 = boost::any_cast<int>(&value))
                    std::cout << *v2;
                else
                    if(auto v3 = boost::any_cast<double>(&value))
                        std::cout << *v3;
                    else
                    {
                        std::cout << "Error while reading the variable map";
                        return -1;
                    }
        std::cout<<"\n";
    }
    return 0;
}


