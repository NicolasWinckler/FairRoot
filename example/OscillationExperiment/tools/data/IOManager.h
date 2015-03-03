/* 
 * File:   IOManager.h
 * Author: winckler
 *
 * Created on February 17, 2015, 9:40 AM
 */

#ifndef IOMANAGER_H
#define	IOMANAGER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>

#include <boost/program_options.hpp>
namespace po = boost::program_options;


// A helper function to simplify the main part.
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " ")); 
    return os;
}



class IOManager 
{
public:
    IOManager();
    virtual ~IOManager();
    
    template<typename T>
        T GetPar(const std::string& key)
        {
            T val;
            try
            {
                val=fvarmap[key].as<T>();
            }
            catch(std::exception& e)
            {
                std::cout << "[ERROR] Problem in map for key = " << key << "\n";
                std::cout << e.what() << "\n";
            }   
            
            return val;
        }
    
    
    int PrintConfig();
    
    int ParseCmdLine(const int argc, char** argv, const po::options_description& desc, po::variables_map& varmap);
    int ParseCfgFile(const std::string& filename, const po::options_description& desc, po::variables_map& varmap);
    
    int PrintHelp()
    {
        std::cout << fVisible_options << "\n";
        return 0;
    }
    
    virtual int ParseAll(const int argc, char** argv);
    
    
protected:

    po::variables_map fvarmap;
    
    po::options_description fGenericDesc;
    po::options_description fConfigDesc;
    po::options_description fHiddenDesc;
    
    po::options_description fCmdline_options;
    po::options_description fConfig_file_options;
    po::options_description fVisible_options;
    
    bool fPrint;
    int fVerboseLvl;
    std::string fConfigFile;
};

#endif	/* IOMANAGER_H */
