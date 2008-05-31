/**
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

#include "PrimitiveMapMarshaller.h"

#include <activemq/util/PrimitiveValueTypes.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <activemq/connector/openwire/utils/OpenwireStringSupport.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::marshal;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::marshal( const activemq::util::PrimitiveMap* map,
                                      std::vector<unsigned char>& dest )
                                        throw ( cms::CMSException ) {

    try {

        ByteArrayOutputStream bytesOut( dest );
        DataOutputStream dataOut( &bytesOut );

        if( map == NULL ) {
            dataOut.writeInt( -1 );
        } else {
            dataOut.writeInt( (int)map->size() );

            std::vector<std::string> keys = map->getKeys();
            std::vector<std::string>::const_iterator iter = keys.begin();

            for(; iter != keys.end(); ++iter ) {

                OpenwireStringSupport::writeString( dataOut, &(*iter) );
                PrimitiveValueNode value = map->getValue( *iter );
                marshalPrimitive( dataOut, value );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
PrimitiveMap* PrimitiveMapMarshaller::unmarshal(
    const std::vector<unsigned char>& src )
        throw ( cms::CMSException ) {

    try{

        ByteArrayInputStream bytesIn( src );
        DataInputStream dataIn( &bytesIn );

        int size = dataIn.readInt();

        if( size > 0 ) {
            PrimitiveMap* map = new PrimitiveMap;

            for( int i=0; i < size; i++ ) {
                std::string key = OpenwireStringSupport::readString( dataIn );
                unmarshalPrimitive( dataIn, key, *map );
            }

            return map;
        }

        return NULL;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::unmarshal(
    activemq::util::PrimitiveMap* map,
    const std::vector<unsigned char>& src ) throw ( cms::CMSException ) {

    try {

        if( map == NULL || src.empty() ) {
            return;
        }

        // Clear old data
        map->clear();

        ByteArrayInputStream bytesIn( src );
        DataInputStream dataIn( &bytesIn );

        int size = dataIn.readInt();

        if( size > 0 ) {
            for( int i=0; i < size; i++ ) {
                std::string key = OpenwireStringSupport::readString( dataIn );
                unmarshalPrimitive( dataIn, key, *map );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::marshalPrimitive( io::DataOutputStream& dataOut,
                                               activemq::util::PrimitiveValueNode& value )
                                                    throw ( decaf::io::IOException ) {

    try {

        if( value.getValueType() == BOOLEAN_TYPE ) {

            dataOut.writeByte( BOOLEAN_TYPE );
            dataOut.writeBoolean( value.getBool() );

        } else if( value.getValueType() == BYTE_TYPE ) {

            dataOut.writeByte( BYTE_TYPE );
            dataOut.writeByte( value.getByte() );

        } else if( value.getValueType() == CHAR_TYPE ) {

            dataOut.writeByte( CHAR_TYPE );
            dataOut.writeChar( value.getChar() );

        } else if( value.getValueType() == SHORT_TYPE ) {

            dataOut.writeByte( SHORT_TYPE );
            dataOut.writeShort( value.getShort() );

        } else if( value.getValueType() == INTEGER_TYPE ) {

            dataOut.writeByte( INTEGER_TYPE );
            dataOut.writeInt( value.getInt() );

        } else if( value.getValueType() == LONG_TYPE ) {

            dataOut.writeByte( LONG_TYPE );
            dataOut.writeLong( value.getLong() );

        } else if( value.getValueType() == FLOAT_TYPE ) {

            dataOut.writeByte( FLOAT_TYPE );
            dataOut.writeFloat( value.getFloat() );

        } else if( value.getValueType() == DOUBLE_TYPE ) {

            dataOut.writeByte( DOUBLE_TYPE );
            dataOut.writeDouble( value.getDouble() );

        } else if( value.getValueType() == BYTE_ARRAY_TYPE ) {

            dataOut.writeByte( BYTE_ARRAY_TYPE );

            std::vector<unsigned char> data = value.getByteArray();

            dataOut.writeInt( (int)data.size() );
            dataOut.write( data );

        } else if( value.getValueType() == STRING_TYPE ) {

            std::string data = value.getString();

            // is the string big??
            if( data.size() > 8191 ) {
                dataOut.writeByte( BIG_STRING_TYPE );
            } else {
                dataOut.writeByte( STRING_TYPE );
            }

            OpenwireStringSupport::writeString( dataOut, &data );

        } else {
            throw IOException(
                __FILE__,
                __LINE__,
                "Object is not a primitive: ");
        }
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::unmarshalPrimitive( io::DataInputStream& dataIn,
                                                 const std::string& key,
                                                 activemq::util::PrimitiveMap& map )
                                                    throw ( decaf::io::IOException ) {

    try {

        unsigned char type = dataIn.readByte();

        switch( type )
        {
            case NULL_TYPE:
                map.setString( key, "" );
                break;
            case BYTE_TYPE:
                map.setByte( key, dataIn.readByte() );
                break;
            case BOOLEAN_TYPE:
                map.setBool( key, dataIn.readBoolean() );
                break;
            case CHAR_TYPE:
                map.setChar( key, dataIn.readChar() );
                break;
            case SHORT_TYPE:
                map.setShort( key, dataIn.readShort() );
                break;
            case INTEGER_TYPE:
                map.setInt( key, dataIn.readInt() );
                break;
            case LONG_TYPE:
                map.setLong( key, dataIn.readLong() );
                break;
            case FLOAT_TYPE:
                map.setFloat( key, dataIn.readFloat() );
                break;
            case DOUBLE_TYPE:
                map.setDouble( key, dataIn.readDouble() );
                break;
            case BYTE_ARRAY_TYPE:
            {
                int size = dataIn.readInt();
                std::vector<unsigned char> data;
                data.resize( size );
                dataIn.readFully( data );
                map.setByteArray( key, data );
                break;
            }
            case STRING_TYPE:
            case BIG_STRING_TYPE:
                map.setString(
                    key,
                    OpenwireStringSupport::readString( dataIn ) );
                break;
            default:
                throw IOException(
                    __FILE__,
                    __LINE__,
                    "PrimitiveMapMarshaller::unmarshalPrimitive - "
                    "Unsupported data type: ");
        }
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}
