/* 
 * File:   OscAnaManager.h
 * Author: winckler
 *
 * Created on February 22, 2015, 7:56 PM
 */

#ifndef OSCANAMANAGER_H
#define	OSCANAMANAGER_H

#include <string>
#include "IOManager.h"

class OscAnaManager : public IOManager
{
public:
    OscAnaManager();
    virtual ~OscAnaManager();
    
    
    
    virtual int ParseAll(const int argc, char** argv);
    
    
private:

    po::options_description fObservable_options;
    po::options_description fModel_options;
    po::options_description fBAT_options;
    po::options_description fSim_options;
    
    void InitOptionDescription();
    
    
};

#endif	/* OSCANAMANAGER_H */

