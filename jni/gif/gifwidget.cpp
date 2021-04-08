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
//#include "stdafx.h"
#include "gifwidget.h"


GifWidget::GifWidget()
{
    duration = 50;
     gif = new QGifCreator();
}

GifWidget::~GifWidget()
{
     delete gif;
}

void GifWidget::addFrame( IplImage* img, ColorMapObject* map, bool dither)
{
   //  QImage i(f);
  //   i = i.mirrored().convertToFormat(QImage::Format_RGB888);
     gif->resize(img->width,img->height);
     if(map) 
		 gif->addPalette(map);
     gif->prepareFrame(img, map, dither);
  //   prevFrames.append(i.mirrored());
     currentFrame = 0;
}



bool GifWidget::save(char* filename)
{
  //   qDebug() << "saving gif...";


    

   
     return saveGif(filename);
}

bool GifWidget::saveGif(char* filename)
{
  //   pause();
  //   gif->setDuration((double)intervalBox->value()/1000);
	gif->setDuration(duration/1000);//Ĭ����50ms�������������һ�£����������Ҳ���Կ����ǲ��������ΪintervalBox->value���׶���Ҳ��ȷ����д����50ms
  //   if(reverseBox->isChecked())
	//  gif->appendReversedCopy();
	bool bb;
     if(!(bb = gif->save(filename,1)))//��һ������Ĭ����һ�����Ե���Ϊ�������֣�����ÿ������֡��һ��
		//  saveEveryBox->isChecked() ? seBox->value() : 1))
     {
	//  QMessageBox::critical(this,tr("Error"),tr("Unexpected error while saving GIF file!"));
	  //PrintGifError(); przeniesione do save
     }
	 return bb;
  //   gif->removeReversedCopy();
 //    emit gifSaved(filename);
}

