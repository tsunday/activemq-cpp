/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <activemq/util/Config.h>

#if !defined(HAVE_WINSOCK2_H) 
    //#include <sys/poll.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <errno.h>    
    extern int errno;
#else
    #include <Winsock2.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#define BSD_COMP /* Get FIONREAD on Solaris2. */
#include <sys/ioctl.h>
#endif

// Pick up FIONREAD on Solaris 2.5.
#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

#include <activemq/network/SocketInputStream.h>
#include <activemq/io/IOException.h>
#include <activemq/exceptions/UnsupportedOperationException.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

using namespace activemq;
using namespace activemq::network;
using namespace activemq::io;
using namespace activemq::exceptions;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SocketInputStream::SocketInputStream( network::Socket::SocketHandle socket )
{
    this->socket = socket;
    debug = false;
}

////////////////////////////////////////////////////////////////////////////////
SocketInputStream::~SocketInputStream()
{
}

////////////////////////////////////////////////////////////////////////////////
int SocketInputStream::available() const throw (activemq::io::IOException){

// The windows version
#if defined(HAVE_WINSOCK2_H) 

    unsigned long numBytes = 0;

    if (::ioctlsocket (socket, FIONREAD, &numBytes) == SOCKET_ERROR){
        throw SocketException( __FILE__, __LINE__, "ioctlsocket failed" );
    }

    return (int)numBytes;

#else // !defined(HAVE_WINSOCK2_H)

    // If FIONREAD is defined - use ioctl to find out how many bytes
    // are available.
	#if defined(FIONREAD)

        int numBytes = 0;
	    if( ::ioctl (socket, FIONREAD, &numBytes) != -1 ){
            return numBytes;
        }
         
	#endif
	
	// If we didn't get anything we can use select.  This is a little
    // less functional.  We will poll on the socket - if there is data
    // available, we'll return 1, otherwise we'll return zero.
	#if defined(HAVE_SELECT)    

        fd_set rd;
        FD_ZERO(&rd);
        FD_SET( socket, &rd );
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int returnCode = ::select(socket+1, &rd, NULL, NULL, &tv);
        if(returnCode == -1){
            throw IOException(__FILE__, __LINE__, ::strerror(errno));
        }
        return (returnCode == 0)? 0 : 1;
        
    #else
    
        return 0;
        
	#endif /* HAVE_SELECT */
	

#endif // !defined(HAVE_WINSOCK2_H)
}

////////////////////////////////////////////////////////////////////////////////
unsigned char SocketInputStream::read() throw (IOException){
   
    unsigned char c;  
    int len = read( &c, 1 );
    if( len != sizeof(c) ){
        throw IOException( __FILE__, __LINE__, 
            "activemq::io::SocketInputStream::read - failed reading a byte");
    }
   
    return c;
}

////////////////////////////////////////////////////////////////////////////////
int SocketInputStream::read( unsigned char* buffer, const int bufferSize ) throw (IOException){
   
    int len = ::recv(socket, (char*)buffer, bufferSize, 0);
    
    // Check for a closed socket.
    if( len == 0 ){
        throw IOException( __FILE__, __LINE__, 
            "activemq::io::SocketInputStream::read - The connection is broken" );
    }
    
    // Check for error.
    if( len == -1 ){
        
        #if !defined(HAVE_WINSOCK2_H) 
         
            // Create the error string.
            char* errorString = ::strerror(errno);
         
        #else
        
            // If the socket was temporarily unavailable - just try again.
            int errorCode = ::WSAGetLastError();
      
            // Create the error string.
            static const int errorStringSize = 512;
            char errorString[errorStringSize];
            memset( errorString, 0, errorStringSize );
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
               0,
               errorCode,
               0,
               errorString,
               errorStringSize - 1,
               NULL);
              
        #endif
        
        // Otherwise, this was a bad error - throw an exception.
        throw IOException( __FILE__, __LINE__, 
                "activemq::io::SocketInputStream::read - %s", errorString );
    }
    
    if( debug ){
        printf("SocketInputStream:read(), numbytes:%d -", len);
        for( int ix=0; ix<len; ++ix ){
            if( buffer[ix] > 20 )
                printf("%c", buffer[ix] );
            else
                printf("[%d]", buffer[ix] );
        }
        printf("\n");
    }
    
    return len;
}
