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
#include "icecapmisc.h"

#include <QtNetwork>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QRegExp>

ClientThread::ClientThread(QObject *parent)
	: QTcpSocket(parent)
{
	connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void ClientThread::sendGreetingMessage()
{
	QDateTime dt = QDateTime::currentDateTime();
	QString greeting = QString("*;preauth;time=%1.%2;remote_ip=%3").arg(dt.toTime_t()).arg(dt.toString("zzz")).arg(this->peerAddress().toString());
	sendMessage(greeting);
}

void ClientThread::run()
{

}

bool ClientThread::sendMessage(const QString &message)
{
	if (message.isEmpty())
		return false;

	QByteArray data = message.toUtf8() +"\n";
	qDebug() << "SEND >> " << message.toUtf8();
	return write(data) == data.size();
}

void ClientThread::processReadyRead()
{
	do {
		processData();
	} while (bytesAvailable() > 0);
}

void ClientThread::processData()
{
	m_buffer = readLine();

	// Get rid of line endings (deals with all 3 possibilities: \r, \n and \r\n)
	if (m_buffer.endsWith('\n')) {
		m_buffer.remove(m_buffer.size() - 1, 1);
	}
	if (m_buffer.endsWith('\r')) {
		m_buffer.remove(m_buffer.size() - 1, 1);
	}

	QString strBuffer = m_buffer;
	// QString strBuffer = m_buffer.remove(m_buffer.size() -1, 1);
	// qDebug() << "RECV << " << m_buffer;
	qDebug() << "RECV << " << toHexString(strBuffer);
	// sendMessage(m_buffer);

	// Decode the communication
	QStringList parts = strBuffer.split(';');
	m_buffer.clear();

	if (parts.size() < 2) {
		// No visible tag, send error: command not understood (no tag)
		// Currently this acts exactly the same as icecapd
		QDateTime dt = QDateTime::currentDateTime();
		sendMessage(QString("%1;-;bad;time=%2.%3").arg(strBuffer).arg(dt.toTime_t()).arg(dt.toString("zzz")));
		return;
	}

	Icecap::Cmd cmd;
	cmd.tag = parts.takeFirst();
	cmd.command = parts.takeFirst();

	// Decode parameters
	// network is the minimal required information to determine the protocol
	cmd.parameterList = decodeParams(parts);
	if (cmd.parameterList.contains("network")) {
		cmd.network = cmd.parameterList["network"];
	}
	if (cmd.parameterList.contains("mypresence")) {
		cmd.mypresence = cmd.parameterList["mypresence"];
	}
	if (cmd.parameterList.contains("channel")) {
		cmd.channel = cmd.parameterList["channel"];
	}
}

/// Split into key, value pairs around the first occurence of =
QMap<QString,QString> ClientThread::decodeParams(QStringList parameterList)
{
	QMap<QString, QString> parameterMap;
	QStringList::const_iterator end = parameterList.end();
	for ( QStringList::const_iterator it = parameterList.begin(); it != end; ++it ) {
		QStringList thisParam = (*it).split("=");
		QString key = thisParam.takeFirst ();
		QString value = thisParam.join ("=");
		parameterMap.insert (key, value);
	}
	return parameterMap;
}

/// Translate the given string to hex with space between values
QString ClientThread::toHexString(QString strBuffer)
{
	return QString(strBuffer.toAscii().toHex().toUpper()).replace(QRegExp("([0-9A-Fa-f]{2})"), QString("\\1 "));
}
