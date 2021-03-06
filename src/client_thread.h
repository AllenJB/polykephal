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
#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QString>
#include <QMap>
#include <QByteArray>
#include <QSemaphore>


#include "IcecapEvent.h"

static const int MaxBufferSize = 1024000;

class ClientThread : public QTcpSocket
{
	Q_OBJECT

	public:
		ClientThread(QObject *parent);

		void run();
		bool sendMessage(const PK::IcecapEvent &message);
		void sendGreetingMessage();
		void shutdown();

	private slots:
		void processReadyRead();
		QString toHexString(const QString strBuffer);

	private:
		void processData();
		void handleEvent(PK::IcecapEvent &event);

		QByteArray m_buffer;
};

#endif
