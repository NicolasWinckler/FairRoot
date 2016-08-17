/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <string>

#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"

using namespace boost::program_options;
using namespace std;

int main(int argc, char** argv)
{
    try
    {
        string text;

        FairMQProgOptions config;
        config.GetCmdLineOptions().add_options()
            ("text", value<string>(&text)->default_value("Hello"), "Text to send out");
        config.ParseAll(argc, argv);

        FairMQDevice sampler;

        sampler.SetRun([&sampler, &text]()
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

            unique_ptr<FairMQMessage> msg(sampler.NewSimpleMessage(text));

            LOG(INFO) << "Sending \"" << text << "\"";

            sampler.Send(msg, "data");

            return true;
        });

        runStateMachine(sampler, config);
    }
    catch (exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}
