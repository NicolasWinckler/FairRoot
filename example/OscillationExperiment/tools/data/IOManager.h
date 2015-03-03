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
            return fvarmap[key].as<T>();
        }
    
    
    int PrintConfig();
    
    int ParseCmdLine(const int argc, char** argv, const po::options_description& desc, po::variables_map& varmap);
    int ParseCfgFile(const std::string& filename, const po::options_description& desc, po::variables_map& varmap);
    
    
    
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
