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
#ifndef ICECAPEVENT_H
#define ICECAPEVENT_H

#include <QString>
#include <QMap>
#include <QDateTime>
#include <QObject>

namespace PK
{

	class IcecapEvent : public QObject
	{
		Q_OBJECT
	
		public:
			typedef enum
			{
				SUnset,
				SSuccess,
				SFail,
				SMore
			} Status;
	
			// Command to send
			IcecapEvent(QString tag, Status status);
	
			// Received command
			IcecapEvent(QString tag, QString command);
	
			// Event (received or to send)
			IcecapEvent(QString eventName);
	
			void setTimestamp();
			void setTimestamp(QString timestamp);
			const QString getTimestamp();
	
			void setParameter(QString key, QString value);
			const QString getParameter(QString key);
	
			const char getStatusString();
	
			const QString getTag();
			const QString getCommand();
	
			const QString toIcecapMessage();
	
		private:
			const QString getParameterString();
			const Status getStatus();
	
			QDateTime m_timestamp;
			QString m_tag;
			Status m_status;
			/// Holds event name if an event
			QString m_command;
			QMap<QString,QString> m_parameterList;
	
	};

}

#endif
