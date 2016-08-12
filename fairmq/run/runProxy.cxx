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
#include "FairMQParts.h"
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

        FairMQDevice proxy;

        if (multipart)
        {
            proxy.SetRun([&proxy]()
            {
                FairMQParts payload;

                if (proxy.Receive(payload, "data-in") >= 0)
                {
                    if (proxy.Send(payload, "data-out") < 0)
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

                return true;
            });
        }
        else
        {
            proxy.SetRun([&proxy]()
            {
                unique_ptr<FairMQMessage> payload(proxy.NewMessage());

                if (proxy.Receive(payload, "data-in") >= 0)
                {
                    if (proxy.Send(payload, "data-out") < 0)
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

                return true;
            });
        }

        runStateMachine(proxy, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: "
                   << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}
