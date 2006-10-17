/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <activemq/connector/openwire/marshal/v2/ReplayCommandMarshaller.h>

#include <activemq/connector/openwire/commands/ReplayCommand.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::util;
using namespace activemq::connector::openwire::marshal::v2;

///////////////////////////////////////////////////////////////////////////////
DataStructure* ReplayCommandMarshaller::createObject() const {
    return new ReplayCommand();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char ReplayCommandMarshaller::getDataStructureType() const {
    return ReplayCommand::ID_REPLAYCOMMAND;
}

///////////////////////////////////////////////////////////////////////////////
void ReplayCommandMarshaller::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) {
   BaseCommandMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

    ReplayCommand* info =
        dynamic_cast<ReplayCommand*>( dataStructure );
    info->setFirstNakNumber( dataIn->readInt() );
    info->setLastNakNumber( dataIn->readInt() );
}

///////////////////////////////////////////////////////////////////////////////
int ReplayCommandMarshaller::tightMarshal1( OpenWireFormat& wireFormat, DataStructure* dataStructure, BooleanStream& bs ) {

    ReplayCommand* info =
        dynamic_cast<ReplayCommand*>( dataStructure );

    int rc = BaseCommandMarshaller::tightMarshal1( wireFormat, dataStructure, bs );

    return rc + 8;
}

///////////////////////////////////////////////////////////////////////////////
void ReplayCommandMarshaller::tightMarshal2( OpenWireFormat& wireFormat, DataStructure* dataStructure, DataOutputStream& dataOut, BooleanStream& bs ) {

    BaseCommandMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

    ReplayCommand* info =
        dynamic_cast<ReplayCommand*>( dataStructure );
    dataOut->write( info->getFirstNakNumber() );
    dataOut->write( info->getLastNakNumber() );
}

///////////////////////////////////////////////////////////////////////////////
void ReplayCommandMarshaller::looseUnmarshal( OpenWireFormat& wireFormat, DataStructure* dataStructure, DataInputStream& dataIn ) {
    BaseCommandMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
    ReplayCommand* info = 
        dynamic_cast<ReplayCommand*>( dataStructure );
    info->setFirstNakNumber( dataIn->readInt() );
    info->setLastNakNumber( dataIn->readInt() );
}

///////////////////////////////////////////////////////////////////////////////
void ReplayCommandMarshaller::looseMarshal( OpenWireFormat& wireFormat, DataStructure* dataStructure, DataOutputStream& dataOut ) {
    ReplayCommand* info =
        dynamic_cast<ReplayCommand*>( dataStructure );
    BaseCommandMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );

    dataOut->write( info->getFirstNakNumber() );
    dataOut->write( info->getLastNakNumber() );
}

