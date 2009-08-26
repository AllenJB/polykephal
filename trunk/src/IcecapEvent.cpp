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
#include "IcecapEvent.h"

#include <QRegExp>
#include <QDateTime>
#include <QStringList>
#include <QDebug>

namespace PK
{
	
	IcecapEvent::IcecapEvent(QString tag, Status status)
	{
		m_tag = tag;
		m_command = QString::Null();
		m_status = status;
		setTimestamp();
	}
	
	IcecapEvent::IcecapEvent(QString tag, QString command)
	{
		m_tag = tag;
		m_command = command;
		m_status = SUnset;
		setTimestamp();
	}
	
	IcecapEvent::IcecapEvent(QString eventName)
	{
		m_tag = QString::Null();
		m_command = eventName;
		m_status = SUnset;
		setTimestamp();
	}
	
	void IcecapEvent::setTimestamp()
	{
		m_timestamp = QDateTime::currentDateTime();
	}
	
	void IcecapEvent::setTimestamp(QString timestamp)
	{
		QDateTime dt = QDateTime();
		bool ok;
		if (QRegExp("^[0-9]+$").exactMatch(timestamp)) {
			// All numbers - assume unix timestamp with seconds only
			dt.setTime_t(timestamp.toUInt(&ok, 10));
		} else
		if (QRegExp("^[0-9]+\\.[0-9]+$").exactMatch(timestamp)) {
			QStringList split = timestamp.split('.');
			// TODO Check this actually works as expected. Bug filed with Qt as behaviour is undefined. 2007-03-12
			dt.setTime_t(split.takeFirst().toUInt(&ok, 10));
			dt.addMSecs(split.takeFirst().toUInt(&ok, 10));
		}
		// TODO What to do about unexpected formats?
		m_timestamp = dt;
	}
	
	QString IcecapEvent::getTimestamp() const
	{
		return QString("%1.%2").arg(m_timestamp.toTime_t()).arg(m_timestamp.toString("zzz"));
	}
	
	void IcecapEvent::setParameter(QString key, QString value)
	{
		m_parameterList.insert(key, value);
	}
	
	QString IcecapEvent::getParameter(QString key) const
	{
		if (m_parameterList.contains(key)) {
			return m_parameterList.value(key);
		}
		return NULL;
	}
	
	char IcecapEvent::getStatusString() const
	{
		// TODO Test if we need a null check here
		switch (m_status) {
			case SSuccess: return '+';
			case SFail: return '-';
			case SMore: return '>';
			case SUnset: ;
		}
		qDebug() << "WARNING: IcecapEvent Status is not set";
		return '?';
	}
	
	QString IcecapEvent::getTag() const
	{
		if (m_tag == NULL) {
			return "*";
		}
		return m_tag;
	}
	
	QString IcecapEvent::getCommand() const
	{
		return m_command;
	}

	IcecapEvent::Status IcecapEvent::getStatus() const
	{
		return m_status;
	}
	
	QString IcecapEvent::toIcecapMessage() const
	{
		// TODO Ensure this returns a UTF8 string
	
		// 4 types of possible messages: command, response, error and event
		// Server doesn't send commands to client, so we can ignore those
	
		QString parameters = getParameterString();
		if (getStatus() == SUnset) {
			// Event
			return QString("%1;%2;time=%3").arg(getTag()).arg(parameters).arg(getTimestamp());
		} else
		if (parameters.length() > 0) {
			// Response or Error
			return QString("%1;%2;%4;time=%3").arg(getTag()).arg(getStatusString()).arg(getTimestamp()).arg(parameters);
		} else {
			return QString("%1;%2;time=%3").arg(getTag()).arg(getStatusString()).arg(getTimestamp());
		}
	}
	
	QString IcecapEvent::getParameterString() const
	{
		QString retVal = "";
		QMap<QString,QString>::const_iterator end = m_parameterList.end();
		for ( QMap<QString,QString>::const_iterator it = m_parameterList.begin(); it != end; ++it ) {
			retVal += it.key();
			if (it.value().length() > 1) {
				retVal += "="+ it.value();
			}
			retVal += ";";
		}
		return retVal;
	}

}
