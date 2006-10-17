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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V2_ACTIVEMQTOPICMARSHALLER_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V2_ACTIVEMQTOPICMARSHALLER_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/marshal/V2/ActiveMQDestinationMarshaller.h>

#include <activemq/io/DataInputStream.h>
#include <activemq/io/DataOutputStream.h>
#include <activemq/io/IOException.h>
#include <activemq/connector/openwire/commands/DataStructure.h>
#include <activemq/connector/openwire/util/BooleanStream.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace marshal{
namespace v2{

    /**
     * Marshalling code for Open Wire Format for ActiveMQTopicMarshaller
     *
     *  NOTE!: This file is autogenerated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-openwire-generator module
     */
    class ActiveMQTopicMarshaller : public ActiveMQDestinationMarshaller
    {
    public:

        ActiveMQTopicMarshaller() {};
        virtual ~ActiveMQTopicMarshaller() {};

        /**
         * Creates a new instance of this marshalable type.
         * @return new DataStructure object pointer caller owns it.
         */
        virtual DataStructure* createObject() const;

        /**
         * Get the Data Structure Type that identifies this Marshaller
         * @return byte holding the data structure type value
         */
        virtual unsigned char getDataStructureType() const;

        /**
         * Un-marshal an object instance from the data input stream
         * @param wireFormat - describs the wire format of the broker
         * @param o - Object to be un-marshaled
         * @param dataIn - BinaryReader that provides that data
         * @param bs - BooleanStream
         */
        virtual void tightUnmarshal( OpenWireFormat* wireFormat,
                                     commands::DataStructure* dataStructure,
                                     io::DataInputStream* dataIn,
                                     util::BooleanStream* bs ) throws( io::IOException );

        /**
         * Write the booleans that this object uses to a BooleanStream
         * @param wireFormat - describis the wire format of the broker
         * @param o - Object to be marshaled
         * @param bs - BooleanStream
         * @returns int
         */
        virtual int tightMarshal1( OpenWireFormat* wireFormat,
                                   commands::DataStructure* dataStructure,
                                   BooleanStream* bs ) throws( io::IOException );

        /**
         * Write a object instance to data output stream
         * @param wireFormat - describs the wire format of the broker
         * @param o - Object to be marshaled
         * @param dataOut - BinaryReader that provides that data sink
         * @param bs - BooleanStream
         */
        virtual void tightMarshal2( OpenWireFormat* wireFormat,
                                    commands::DataStructure* dataStructure,
                                    io::DataOutputStream* dataOut,
                                    BooleanStream* bs ) throws( io::IOException );

        /**
         * Un-marshal an object instance from the data input stream
         * @param wireFormat - describs the wire format of the broker
         * @param o - Object to be marshaled
         * @param dataIn - BinaryReader that provides that data source
         */
        virtual void looseUnmarshal( OpenWireFormat* wireFormat,
                                     commands::DataStructure* dataStructure,
                                     io::DataInputStream* dataIn ) throws( io::IOException );

        /**
         * Write a object instance to data output stream
         * @param wireFormat - describs the wire format of the broker
         * @param o - Object to be marshaled
         * @param dataOut - BinaryWriter that provides that data sink
         */
        virtual void looseMarshal( OpenWireFormat* wireFormat,
                                   commands::DataStructure* dataStructure,
                                   io::DataOutputStream* dataOut ) throws( io::IOException );

    };

}}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V2_ACTIVEMQTOPICMARSHALLER_H_*/

