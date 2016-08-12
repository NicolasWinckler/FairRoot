/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <iostream>

#include <boost/program_options.hpp>

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"

using namespace boost::program_options;

int main(int argc, char** argv)
{
    try
    {
        int multipart;

        FairMQProgOptions config;
        config.GetCmdLineOptions().add_options()
            ("multipart", value<int>(&multipart)->default_value(1), "Handle multipart payloads");
        config.ParseAll(argc, argv);

        FairMQDevice splitter;

        int numOutputs = 0;
        int direction = 0;

        splitter.SetPreRun([&splitter, &numOutputs]()
        {
            numOutputs = splitter.fChannels.at("data-out").size();
        });

        if (multipart)
        {
            splitter.SetRun([&splitter, &numOutputs, &direction]()
            {
                FairMQParts payload;

                if (splitter.Receive(payload, "data-in") >= 0)
                {
                    if (splitter.Send(payload, "data-out", direction) < 0)
                    {
                        LOG(DEBUG) << "Transfer interrupted";
                        return false;
                    }
                }
                else
                {
                    LOG(DEBUG) << "Transfer interrupted";
                    return false;
                }

                ++direction;
                if (direction >= numOutputs)
                {
                    direction = 0;
                }

                return true;
            });
        }
        else
        {
            splitter.SetRun([&splitter, &numOutputs, &direction]()
            {
                unique_ptr<FairMQMessage> payload(splitter.NewMessage());

                if (splitter.Receive(payload, "data-in") >= 0)
                {
                    if (splitter.Send(payload, "data-out", direction) < 0)
                    {
                        LOG(DEBUG) << "Transfer interrupted";
                        return false;
                    }
                }
                else
                {
                    LOG(DEBUG) << "Transfer interrupted";
                    return false;
                }

                ++direction;
                if (direction >= numOutputs)
                {
                    direction = 0;
                }

                return true;
            });
        }

        runStateMachine(splitter, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: "
                   << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}
