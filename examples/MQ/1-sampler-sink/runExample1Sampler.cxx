/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <string>


#include "FairMQDevice.h"
//#include "FairMQExample1Sampler.h"
#include "runFairMQDevice.h"
#include <iostream>
#include <future>
#include <thread>

namespace bpo = boost::program_options;
typedef FairMQDevice FairMQExample1Sampler;

void addCustomOptions(bpo::options_description& options)
{
    options.add_options()
        ("text", bpo::value<std::string>()->default_value("Hello"), "Text to send out");
}

void MySetRunner(FairMQExample1Sampler& device)
{
  device.SetRun([&device]()
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

            std::string text("my text");
            LOG(INFO) << "Sending \"" << text << "\"";
            std::unique_ptr<FairMQMessage> msg(device.NewSimpleMessage(text));


            device.Send(msg, "data");
            return true;
      });
}

//BUILD_MAIN(FairMQExample1Sampler)

BUILD_MAIN_CUSTOM_RUN2(MySetRunner)