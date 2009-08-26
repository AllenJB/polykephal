/*
  Copyright (C) 2008 by Allen Brooker
  polykephal@allenjb.me.uk

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#include <stdlib.h>

#include "server.h"
#include "client_thread.h"
#include "PolykephalApp.h"

#include <QListIterator>
#include <QMutableListIterator>
#include <QWaitCondition>

Server::Server(QObject *parent)
	: QTcpServer(parent)
{
}

void Server::incomingConnection(int socketDescriptor)
{
	ClientThread *connection = new ClientThread(this);
	connection->setSocketDescriptor(socketDescriptor);
	connection->sendGreetingMessage();
	m_connectionList.append(connection);
	emit newConnection(connection);
}

void Server::shutdown()
{
	// TODO Disconnect from all networks
		// Iterate over list of connected networks; Call disconnect

	// Disconnect all clients
	PK::IcecapEvent event("shutdown", PK::IcecapEvent::SSuccess);

	QMutableListIterator<ClientThread*> it( m_connectionList );
	while (it.hasNext()) {
		qDebug() << "Sending shutdown event";
		ClientThread* current = it.next();
		current->sendMessage(event);
		current->shutdown();
 		current->waitForDisconnected(10000);
		it.remove();
	}

	// Shutdown application
	PolykephalApp* app = (PolykephalApp*) this->parent();
	app->shutdown();

}

void Server::broadcast(PK::IcecapEvent &event)
{
	QListIterator<ClientThread*> it( m_connectionList );
	while (it.hasNext()) {
		ClientThread* current = it.next();
		current->sendMessage(event);
	}
}

