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
#ifndef ICECAPMISC_H
#define ICECAPMISC_H

#include <qmap.h>
#include <qstring.h>

namespace Icecap
{

    /**
     * Icecap command / response / event representation
     */
    typedef struct
    {
        QString tag;
        QString status;  // either +, > or - (success / finished, more-to-come or failure)
        QString command; // Holds event name if an event
        QMap<QString,QString> parameterList;

        QString network;
        QString mypresence;
        QString channel;

        QString sentCommand;
        QMap<QString, QString> sentParameterList;
    } Cmd;
}

#endif