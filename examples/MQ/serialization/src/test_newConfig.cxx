/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/*
 * File:   runSamplerRoot.cxx
 * Author: winckler
 */

#include <boost/any.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>
#include <iostream>
#include <memory>
#include <string>

//#include "newconfig_helper.h"
#include "FairMQLogger.h"


// FairRoot - FairMQ
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

 #include "MyDigi.h"
#include "MyHit.h"

#include "Ex1Processor.h"

typedef std::unordered_map<std::string, std::vector<FairMQChannel>> FairMQMap;

void PrintMQParam(const FairMQMap& channels, const FairMQProgOptions& config)
{

    for(const auto& p : channels)
    {
        int index = 0;
        for(const auto& channel : p.second)
        {
            std::string typeKey = p.first + "." + std::to_string(index) + ".type";
            std::string methodKey = p.first + "." + std::to_string(index) + ".method";
            std::string addressKey = p.first + "." + std::to_string(index) + ".address";
            std::string propertyKey = p.first + "." + std::to_string(index) + ".property";
            std::string sndBufSizeKey = p.first + "." + std::to_string(index) + ".sndBufSize";
            std::string rcvBufSizeKey = p.first + "." + std::to_string(index) + ".rcvBufSize";
            std::string rateLoggingKey = p.first + "." + std::to_string(index) + ".rateLogging";
            
            LOG(DEBUG) << "Channel name = "<<p.first;
            LOG(DEBUG) << "key = " << typeKey <<"\t value = " << config.GetValue<std::string>(typeKey);
            LOG(DEBUG) << "key = " << methodKey <<"\t value = " << config.GetValue<std::string>(methodKey);
            LOG(DEBUG) << "key = " << addressKey <<"\t value = " << config.GetValue<std::string>(addressKey);
            LOG(DEBUG) << "key = " << propertyKey <<"\t value = " << config.GetValue<std::string>(propertyKey);
            LOG(DEBUG) << "key = " << sndBufSizeKey << "\t value = " << config.GetValue<int>(sndBufSizeKey);
            LOG(DEBUG) << "key = " << rcvBufSizeKey <<"\t value = " << config.GetValue<int>(rcvBufSizeKey);
            LOG(DEBUG) << "key = " << rateLoggingKey <<"\t value = " << config.GetValue<int>(rateLoggingKey);
        }
    }
}


int main(int argc, char** argv)
{
    try
    {
        // create option manager object
        FairMQProgOptions config;

        // parse command lines, parse json file and init FairMQMap
        config.ParseAll(argc, argv);
        
        // get FairMQMap
        auto map1 = config.GetFairMQMap();
        
        // form keys from map1 and print the value stored in variable map
        PrintMQParam(map1,config);

        // update value in variable map, and propagate the update to the FairMQMap
        config.UpdateValue<std::string>("data-out.0.address","tcp://localhost:1234");
        
        // get the updated FairMQMap
        auto map2 = config.GetFairMQMap();

        // modify one channel value
        map2.at("data-out").at(0).UpdateSndBufSize(500);

        // update the FairMQMap and propagate the change in variable map
        config.UpdateChannelMap(map2);

        // print values stored in variable map 
        PrintMQParam(map2,config);

        Ex1Processor processor;
        processor.CatchSignals();
        processor.SetConfig(config);

        // use the signal slot system to update device
        // first connect slot

        LOG(INFO)<<"Connect 1";
        config.Subscribe<std::string>("data-out.0.address",[&processor](const std::string& key, const std::string& value) 
            {
                LOG(INFO) << "[Lambda] Update parameter " << key << " = " << value; 
                processor.fChannels.at("data-out").at(0).UpdateAddress(value);
            });

        LOG(INFO)<<"Connect 2";
        config.Subscribe<std::string>("data-out.0.method",[&processor](const std::string& key, const std::string& value) 
            {
                LOG(INFO) << "[Lambda] Update parameter " << key << " = " << value; 
                processor.fChannels.at("data-out").at(0).UpdateMethod(value);
            });

        LOG(INFO)<<"Connect 3";
        config.Subscribe<int>("data-out.0.rcvBufSize",[&processor](const std::string& key, int value) 
            {
                LOG(INFO) << "[Lambda] Update parameter " << key << " = " << value; 
                processor.fChannels.at("data-out").at(0).UpdateRcvBufSize(value);
            });
            //*

        LOG(INFO)<<"Connect 4";
        config.Subscribe<double>("my key",[](const std::string& key, double value) 
            {
                LOG(INFO) << "[Lambda] Update parameter " << key << " = " << value; 
            });
            // 
            // */

        LOG(INFO)<<"end Connect ";
        // define some key-value pairs to be updated
        std::string key1("data-out.0.address");
        std::string value1("tcp://localhost:4321");

        std::string key2("data-out.0.rcvBufSize");
        int value2(100);

        std::string key3("data-out.0.method");
        std::string value3("bind");
        


        LOG(INFO)<<"start update";

        //config.EmitUpdate(key,value);
        config.UpdateValue<std::string>(key1,value1);

        LOG(INFO)<<"processor.fChannels.GetAddress = "<<processor.fChannels.at("data-out").at(0).GetAddress();
        LOG(INFO)<<"config.GetValue = "<<config.GetValue<std::string>(key1);

        config.UpdateValue<int>(key2,value2);

        LOG(INFO)<<"processor.fChannels.GetRcvBufSize = "<<processor.fChannels.at("data-out").at(0).GetRcvBufSize();
        LOG(INFO)<<"config.GetValue = "<<config.GetValue<int>(key2);

        config.UpdateValue<std::string>(key3,value3);

        LOG(INFO)<<"processor.fChannels.Method = "<<processor.fChannels.at("data-out").at(0).GetMethod();
        LOG(INFO)<<"config.GetValue = "<<config.GetValue<std::string>(key3);

        LOG(INFO)<<"----------- New test -------------";


    }
    catch (std::exception& e)
    {
        LOG(ERROR)  << "Unhandled Exception reached the top of main: " 
                    << e.what() << ", application will now exit";
        return 1;
    }
    return 0;
}
//*/

