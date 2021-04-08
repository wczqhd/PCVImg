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

#ifndef GIFWIDGET_H
#define GIFWIDGET_H


#include "qgifcreator.h"


class GifWidget 
{
    
public:
     GifWidget();
     virtual ~GifWidget();
     void addFrame( IplImage* img, ColorMapObject* map, bool dither);
     void setColorRes(int res){gif->setColorRes(res);}
  //   void suggestName(const QString& name){suggestedName = name;}
     bool saveGif(char* filename);
     
	 bool save(char* filename);

	 void setDuration(float d){duration = d;}
    

private:
     QGifCreator* gif;
     bool reversePlay;
     ColorMapObject* palette;
    
     void createActions();
    
  //   QList<QImage> prevFrames;
     int timerId;
     int currentFrame;
     int skipped;
  
	 float duration;
	  
	 
	  
};

#endif
