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

#include "SSLSocketFactory.h"

#include <decaf/internal/net/Network.h>

#include <decaf/internal/net/ssl/DefaultSSLContext.h>
#include <decaf/internal/net/ssl/DefaultSSLSocketFactory.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;

////////////////////////////////////////////////////////////////////////////////
SocketFactory* SSLSocketFactory::defaultSocketFactory = NULL;

////////////////////////////////////////////////////////////////////////////////
SSLSocketFactory::SSLSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocketFactory::~SSLSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory* SSLSocketFactory::getDefault() {

    if( SSLSocketFactory::defaultSocketFactory != NULL ) {
        return SSLSocketFactory::defaultSocketFactory;
    }

    // Check the DefaultSSLContext to see if any SSL Providers are enabled
    SSLContext* context = DefaultSSLContext::getContext();
    if( context != NULL ) {

        // The SSLContext owns the Factory returned here, no need to manage it.
        SSLSocketFactory::defaultSocketFactory = context->getSocketFactory();
    }

    // Non found, use the non-functional default one.
    if( SSLSocketFactory::defaultSocketFactory == NULL ) {
        SSLSocketFactory::defaultSocketFactory =
            new DefaultSSLSocketFactory( "SSL Support is not enabled in this build." );

        // Since we created this one we need to make sure it is destroyed when the Network
        // Runtime is shutdown.
        Network::getNetworkRuntime()->addAsResource( SSLSocketFactory::defaultSocketFactory );
    }

    return SSLSocketFactory::defaultSocketFactory;
}