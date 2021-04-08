/************************************************************************
** QGifer
** Copyright (C) 2013 Lukasz Chodyla <chodak166@op.pl>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
************************************************************************/

#ifndef QGIFCREATOR_H
#define QGIFCREATOR_H

#include <opencv2\opencv.hpp>
#include "gifcreator.h"

class QGifCreator : public GifCreator
{
    
public:
     QGifCreator();
     virtual ~QGifCreator();
     
     Byte* imageData(const IplImage& img);
     void prepareFrame(IplImage* img, ColorMapObject* map, bool dither = true);
private:
     bool savingProgress(int p);
     void endProgress()
    // {if(progressDialog){progressDialog->close();delete progressDialog;progressDialog = NULL;}}
	 {}
  //   QProgressDialog* progressDialog;
    // QList<QImage*> cache;
    // private slots:

};

#endif
