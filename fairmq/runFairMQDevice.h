/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "boost/program_options.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"

void addCustomOptions(boost::program_options::options_description&);

// class template - use template specialization to customize
template<typename T>
struct DeviceManager
{


    int main_imp(int argc, char** argv)
    {

        boost::program_options::options_description customOptions("Custom options");
        addCustomOptions(customOptions);

        FairMQProgOptions config;
        config.AddToCmdLineOptions(customOptions);
        config.ParseAll(argc, argv);

        T device;
        runStateMachine(device, config);
        return 0;
    }
};



// policy based design - use policy to customize

// 1) main default policy
template<typename T>
struct MainDefaultPolicy
{
    int main_imp(int argc, char** argv, FairMQProgOptions& config, T& device)
    {
        boost::program_options::options_description customOptions("Custom options");
        addCustomOptions(customOptions);
        config.AddToCmdLineOptions(customOptions);
        config.ParseAll(argc, argv);
        runStateMachine(device, config);
        return 0;
    }
};

// 2) Run default policy
template<typename T>
struct RunDefaultPolicy
{
    protected:
    template<typename F>
    void DefineSetRun(F&& func)
    {
        fCallback = std::forward<F>(func);
    }

    void SetRun(T& device)
    {
        fCallback(device);
    }

    std::function<void(T&)> fCallback;
};

// 3) Host class deriving from policy
template<   typename DeviceType, 
            typename Run = RunDefaultPolicy<DeviceType>,
            typename Main = MainDefaultPolicy<DeviceType> 
        >
struct DeviceManager2 : protected Main, protected Run
{


    template<typename F>
    void DefineSetRun(F&& func)
    {
        Run::DefineSetRun(std::forward<F>(func));
    }

    int StartApp(int argc, char** argv)
    {
        DeviceType device;
        FairMQProgOptions config;
        Run::SetRun(device);
        return Main::main_imp(argc,argv,config,device);
    }

};

// template function - use overload to customize
template<typename DeviceType>
int DeviceManager3(int argc, char** argv, DeviceType& device)
{
    boost::program_options::options_description customOptions("Custom options");
    addCustomOptions(customOptions);
    FairMQProgOptions config;
    config.AddToCmdLineOptions(customOptions);
    config.ParseAll(argc, argv);
    runStateMachine(device, config);

    return 0;
}

#define BUILD_MAIN(DeviceName)             \
int main( int argc, char* argv[] )         \
    {                                      \
        DeviceManager2<DeviceName> man2;   \
        man2.StartApp(argc,argv);          \
        return 0;                          \
    }                                      \

#define BUILD_MAIN_CUSTOM_RUN(DeviceName,Runfunc) \
int main( int argc, char* argv[] )                \
    {                                             \
        DeviceManager2<DeviceName> man2;          \
        man2.DefineSetRun(Runfunc);               \
        man2.StartApp(argc,argv);                 \
        return 0;                                 \
    }                                             \


#define BUILD_MAIN_CUSTOM_RUN2(Runfunc) \
int main( int argc, char* argv[] )                \
    {                                             \
        DeviceManager2<FairMQDevice> man2;          \
        man2.DefineSetRun(Runfunc);               \
        man2.StartApp(argc,argv);                 \
        return 0;                                 \
    }                                             \


/*
#define BUILD_MAIN_CUSTOM_RUN_AND_MAIN(DeviceName,RunType, MainType) \
int main( int argc, char* argv[] )                                   \
    {                                                                \
        DeviceManager2<DeviceName, RunType, MainType> man2;          \
        man2.StartApp(argc,argv);                                    \
        return 0;                                                    \
    }                                                                \
*/







