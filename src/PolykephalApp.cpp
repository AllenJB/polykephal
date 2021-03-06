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
#include "PolykephalApp.h"
#include "server.h"

#include <QtNetwork>
#include <string>

PolykephalApp::PolykephalApp ( int & argc, char ** argv )
	: QCoreApplication(argc, argv)
{
}

void PolykephalApp::startup()
{
	m_server = new Server(this);
	if (!m_server->listen(QHostAddress::Any, 42000)) {
		qDebug() << "Unable to start the server: " << m_server->errorString();
		close(1);
		return;
	}

	qDebug() << "Server running on port " << m_server->serverPort();
}

void PolykephalApp::shutdown()
{
	this->exit();
}
