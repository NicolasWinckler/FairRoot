/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <string>
#include "FairMQDevice.h"
#include "runFairMQDevice.h"

////////////////////////// DEFINE YOUR CLASS (OPTIONAL)
class MyDevice : public FairMQDevice
{
public:
    MyDevice() : FairMQDevice() {}
    virtual ~MyDevice(){}
    void Print(const std::string& id, const std::string& text)
    {
        LOG(INFO) << "[" << id << "] Sending \"" << text << "\"";
    }
};

////////////////////////// DEFINE YOUR OPTIONS
void addCustomOptions(boost::program_options::options_description& options)
{
    options.add_options()
        ("text", boost::program_options::value<std::string>()->default_value("Hello"), "Text to send out");
}

////////////////////////// DEFINE YOUR RUN FUNCTION : Macro argument is the type of the device
WRITE_RUN_START(MyDevice)
    // rule : we have to use the device object (of given type) and config object (FairMQProgOptions)
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    std::string text = config.GetValue<std::string>("text");
    std::string id = device.fId;  // access to private data member of FairMQDevice parent class
    device.Print(id,text);        // access to derived class methods
    std::unique_ptr<FairMQMessage> msg(device.NewSimpleMessage(text));
    device.Send(msg, "data");
    return true;
WRITE_RUN_END()

////////////////////////// BUILD MAIN FUNCTION : Macro argument is the type of the device
BUILD_MAIN(MyDevice)






// NOTE :
////////////////////////// the WRITE_RUN_START .. WRITE_RUN_END macro above expands to :
/*
    template<>
    std::function<void(FairMQDevice&, FairMQProgOptions&)> DeviceManagerCallback<FairMQDevice>() 
    {
        return [] (FairMQDevice& device, FairMQProgOptions& config) 
        {
            device.SetRun([&device,&config]()
            {   
                std::string text = config.GetValue<std::string>("text");
                boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
                LOG(INFO) << "[" << device.fId << "] Sending \"" << text << "\"";
                std::unique_ptr<FairMQMessage> msg(device.NewSimpleMessage(text));
                device.Send(msg, "data");
                return true;
            });
        };
    }
*/

////////////////////////// the BUILD_MAIN macro expands to :
/*
    int main( int argc, char* argv[] )     
    {                                      
        DeviceManager<MyDevice> man;     
        man.StartApp(argc,argv);           
        return 0;                          
    }
*/





