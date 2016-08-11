/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * runExample1Sampler.cxx
 *
 * @since 2013-04-23
 * @author D. Klein, A. Rybalchenko
 */

#include <string>

#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"

using namespace boost::program_options;

int main(int argc, char** argv)
{
    try
    {
        std::string text;

        options_description samplerOptions("Sampler options");
        samplerOptions.add_options()
            ("text", value<std::string>(&text)->default_value("Hello"), "Text to send out");

        FairMQProgOptions config;
        config.AddToCmdLineOptions(samplerOptions);
        config.ParseAll(argc, argv);

        FairMQDevice sampler;

        sampler.SetRun([&sampler, &text]()
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

            std::unique_ptr<FairMQMessage> msg(sampler.NewSimpleMessage(text));

            LOG(INFO) << "Sending \"" << text << "\"";

            sampler.Send(msg, "data");

            return true;
        });

        runStateMachine(sampler, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: "
                   << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}
