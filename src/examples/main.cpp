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

// START SNIPPET: demo

#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/Runnable.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Interger.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>

using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::concurrent;
using namespace cms;
using namespace std;

class HelloWorldProducer : public Runnable {
private:
	
	Connection* connection;
	Session* session;
	Destination* destination;
	MessageProducer* producer;
	int numMessages;

public:
	
	HelloWorldProducer( int numMessages ){
		connection = NULL;
    	session = NULL;
    	destination = NULL;
    	producer = NULL;
    	this->numMessages = numMessages;
	}
	
	virtual ~HelloWorldProducer(){
		cleanup();
	}
	
    virtual void run() {
        try {
            // Create a ConnectionFactory
            ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory("tcp://127.0.0.1:61613");

            // Create a Connection
            connection = connectionFactory->createConnection();
            connection->start();

            // Create a Session
            session = connection->createSession( Session::AUTO_ACKNOWLEDGE );

            // Create the destination (Topic or Queue)
            destination = session->createQueue( "TEST.FOO" );

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::NON_PERSISTANT );
            
            // Create the Thread Id String
            string threadIdStr = Interger::toString( Thread::getId() );
            
            // Create a messages
            string text = (string)"Hello world! from thread " + threadIdStr;
            
            for( int ix=0; ix<numMessages; ++ix ){
	            TextMessage* message = session->createTextMessage( text );

    	        // Tell the producer to send the message
        	    printf( "Sent message from thread %s\n", threadIdStr.c_str() );
            	producer->send( message );
            	
            	delete message;
            }
			
        }catch ( CMSException& e ) {
            e.printStackTrace();
        }
    }
    
private:

    void cleanup(){
    				
			// Destroy resources.
			try{                        
            	if( destination != NULL ) delete destination;
			}catch ( CMSException& e ) {}
			destination = NULL;
			
			try{
	            if( producer != NULL ) delete producer;
			}catch ( CMSException& e ) {}
			producer = NULL;
			
    		// Close open resources.
    		try{
    			if( session != NULL ) session->close();
    			if( connection != NULL ) connection->close();
			}catch ( CMSException& e ) {}

			try{
            	if( session != NULL ) delete session;
			}catch ( CMSException& e ) {}
			session = NULL;
			
            try{
            	if( connection != NULL ) delete connection;
			}catch ( CMSException& e ) {}
    		connection = NULL;
    }
};

class HelloWorldConsumer : public ExceptionListener, 
                           public MessageListener,
                           public Runnable {
	
private:
	
	Connection* connection;
	Session* session;
	Destination* destination;
	MessageConsumer* consumer;
	long waitMillis;
		
public: 

	HelloWorldConsumer( long waitMillis ){
		connection = NULL;
    	session = NULL;
    	destination = NULL;
    	consumer = NULL;
    	this->waitMillis = waitMillis;
	}
    virtual ~HelloWorldConsumer(){    	
    	cleanup();
    }
    
    virtual void run() {
    	    	
        try {

            // Create a ConnectionFactory
            ActiveMQConnectionFactory* connectionFactory = 
                new ActiveMQConnectionFactory( "tcp://127.0.0.1:61613" );

            // Create a Connection
            connection = connectionFactory->createConnection();
            delete connectionFactory;
            connection->start();
            
            connection->setExceptionListener(this);

            // Create a Session
            session = connection->createSession( Session::AUTO_ACKNOWLEDGE );

            // Create the destination (Topic or Queue)
            destination = session->createQueue( "TEST.FOO" );

            // Create a MessageConsumer from the Session to the Topic or Queue
            consumer = session->createConsumer( destination );
            
            consumer->setMessageListener( this );
            
            // Sleep while asynchronous messages come in.
            Thread::sleep( waitMillis );		
            
        } catch (CMSException& e) {
            e.printStackTrace();
        }
    }
    
    virtual void onMessage( const Message* message ){
    	
        try
        {
    	    const TextMessage* textMessage = 
                dynamic_cast< const TextMessage* >( message );
            string text = textMessage->getText();
            printf( "Received: %s\n", text.c_str() );
        } catch (CMSException& e) {
            e.printStackTrace();
        }
    }

    virtual void onException( const CMSException& ex ) {
        printf("JMS Exception occured.  Shutting down client.\n");
    }
    
private:

    void cleanup(){
    	
		// Destroy resources.
		try{                        
        	if( destination != NULL ) delete destination;
		}catch (CMSException& e) {}
		destination = NULL;
		
		try{
            if( consumer != NULL ) delete consumer;
		}catch (CMSException& e) {}
		consumer = NULL;
		
		// Close open resources.
		try{
			if( session != NULL ) session->close();
			if( connection != NULL ) connection->close();
		}catch (CMSException& e) {}
		
        try{
        	if( session != NULL ) delete session;
		}catch (CMSException& e) {}
		session = NULL;
		
		try{
        	if( connection != NULL ) delete connection;
		}catch (CMSException& e) {}
		connection = NULL;
    }
};
    
int main(int argc, char* argv[]) {
    
    HelloWorldProducer producer( 1000 );
	HelloWorldConsumer consumer( 5000 );
	
	// Start the consumer thread.
	Thread consumerThread( &consumer );
	consumerThread.start();
	
	// Start the producer thread.
	Thread producerThread( &producer );
	producerThread.start();

	// Wait for the threads to complete.
	producerThread.join();
	consumerThread.join();
}
    
// END SNIPPET: demo
