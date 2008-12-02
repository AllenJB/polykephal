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
	: QObject(parent), m_socketDescriptor(socketDescriptor)
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

	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
	connect(&m_socket, SIGNAL(connected()), this, SLOT(sendGreetingMessage()));
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
	qDebug() << "SEND >> " << data;
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
	qDebug() << "RECV << " << m_buffer;
	sendMessage(m_buffer);

	// Decode the communication
	QStringList parts = m_buffer.split(";");
	m_buffer.clear();

	if (parts.size() < 2) {
		// No visible tag, send error: command not understood (no tag)
		return;
	}

	Icecap::Cmd cmd;
	cmd.tag = parts.takeFirst();
	if (cmd.tag == "*") {
		cmd.eventName = parts.takeFirst();
	}

	// Decode parameters
	// FIXME The network / mypresence / channel values should either be removed or moved to a protocol specific location.
	result.parameterList = decodeParams(parts);
	if (result.parameterList.contains("network")) {
		result.network = result.parameterList["network"];
	}
	if (result.parameterList.contains("mypresence")) {
		result.mypresence = result.parameterList["mypresence"];
	}
	if (result.parameterList.contains("channel")) {
		result.channel = result.parameterList["channel"];
	}
}

// Split into key, value pairs around the first occurence of =
QMap<QString,QString> ClientThread::decodeParams(QStringList parameterList)
{
	QMap<QString, QString> parameterMap;
	QStringList::const_iterator end = parameterList.end();
	for ( QStringList::const_iterator it = parameterList.begin(); it != end; ++it ) {
		QStringList thisParam = QStringList::split("=", *it);
		QString key = thisParam.first ();
		thisParam.pop_front ();
		QString value = thisParam.join ("=");
		parameterMap.insert (key, value, TRUE);
	}
	return parameterMap;
}
