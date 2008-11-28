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
#include "client_thread.h"

#include <QtNetwork>

ClientThread::ClientThread(int socketDescriptor, QObject *parent)
	: QThread(parent), m_socketDescriptor(socketDescriptor)
{
}

void ClientThread::sendGreetingMessage()
{
    QByteArray greeting = "foobar";
    QByteArray data = "GREETING " + QByteArray::number(greeting.size()) + " " + greeting;
    m_socket.write(data);
}

void ClientThread::run()
{
	if (!m_socket.setSocketDescriptor(m_socketDescriptor)) {
		emit error(m_socket.error());
		return;
	}

	QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
	QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(sendGreetingMessage()));
/*
	QString text = "foobar";
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << (quint16)0;
	out << text;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));

	m_socket.write(block);
	m_socket.disconnectFromHost();
	m_socket.waitForDisconnected();
*/
}

bool ClientThread::sendMessage(const QString &message)
{
	if (message.isEmpty())
		return false;

	QByteArray msg = message.toUtf8();
	QByteArray data = "MESSAGE " + QByteArray::number(msg.size()) + " " + msg;
	return m_socket.write(data) == data.size();
}

void ClientThread::processReadyRead()
{
	do {
		processData();
	} while (m_socket.bytesAvailable() > 0);
}

void ClientThread::processData()
{
	m_buffer = m_socket.readLine();
	sendMessage(m_buffer);

	m_buffer.clear();
}

