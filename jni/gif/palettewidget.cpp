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
////#include "stdafx.h"
#include "palettewidget.h"

/*#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFile>
#include <QTextStream>*/
#include "qgifcreator.h"

PaletteWidget::PaletteWidget(): 
    cols(10), palette(NULL), size(256), sqSize(0),
     hlIndex(-1)
{
   //  setMouseTracking(true);
}

PaletteWidget::~PaletteWidget()
{
     GifFreeMapObject(palette);
}



bool PaletteWidget::fromImage(const IplImage* img, int palette_size, float mindiff)
{

     if(palette && mindiff > 1)
     {
	//  qDebug() << "deleting old palette, size: " << size << ", colors: " << palette->ColorCount;
	  GifFreeMapObject(palette);
	//  qDebug() << "done";
	  palette = NULL;
     }

     size = palette_size;
   //  QImage fimg = img.mirrored().convertToFormat(QImage::Format_RGB888);
     
	// IplImage* timg = img;

	 const int npix = img->width *img->height;//widthStep的应用有可能有问题，需要尝试
     const int bytesPerLine = img->widthStep;//img->width*3; //wynik INNY niz fimg.bytesPerLine()
     Frame r(npix),g(npix),b(npix);
     Frame output(npix);

     //Byte* data = fimg.bits();
     // for (int i=0, j=0; i<npix; i++)
     // {
     // 	  r[i]=data[j++];
     // 	  g[i]=data[j++];
     // 	  b[i]=data[j++];
     // }

     int cpix = 0;
     
     for(int rw=0;rw<img->height;rw++)
     {
	 // uchar* line = fimg.scanLine(rw);
	  uchar* line = (uchar*)(img->imageData + rw*img->widthStep);
	  //for(int i=0;i+2<fimg.bytesPerLine();i+=3)
	  for(int i=0;i+2<bytesPerLine;i+=3)
	  {
	       //qDebug() << "cpix: " << cpix;
	       r[cpix]=line[i];
	       g[cpix]=line[i+1];
	       b[cpix]=line[i+2];
	       cpix++;
	  }
     }

     ColorMapObject* previous = palette;
     palette = GifMakeMapObject(size, NULL);
     if (!palette) 
     {
//	  qDebug() << "NULL palette!";
	  return false;
     }
     

     if (GifQuantizeBuffer(img->width, img->height, &size, 
     			&(r[0]),&(g[0]),&(b[0]), &(output[0]), 
     			palette->Colors) == GIF_ERROR) 
     {
     	//  qDebug() << "quentize failed!";
     	  palette = NULL;
     	  return false; 
     }

     float df = difference(palette,previous);
     //qDebug() << "difference: " << df;
     if(previous && df < mindiff)
     {
	  GifFreeMapObject(palette);
	  palette = previous;
     }
     else if(df >= mindiff)
	  GifFreeMapObject(previous);

     // qDebug() << "palette (" << palette->ColorCount << ") :";
     // for(int i=0;i<size;i++)
     // 	  qDebug() << i << ": (" << palette->Colors[i].Red << "," << 
     // 	       palette->Colors[i].Green << "," << 
     // 	       palette->Colors[i].Blue << ")"; 
	       
     //setFixedSize(width(), (size/cols)*((width()-2)/cols));
    // setFixedSize(img->width(),(size/cols)*sqSize+sqSize+2);
     //update();
     return true;
}


float PaletteWidget::difference(ColorMapObject* a, ColorMapObject* b)
{
     if(!a || !b)
	  return 0;
     int same = 0;
     for(int c=0; c < a->ColorCount; c++)
     {
	  GifColorType& cc = a->Colors[c];
	  for(int p=0; p < b->ColorCount; p++)
	  {
	       GifColorType& pc = b->Colors[p];
	       if(pc.Red == cc.Red && pc.Green == cc.Green && pc.Blue == cc.Blue)
		    same++;
	  }
     }
     return (float)same/(float)size;
}

/*
QString PaletteWidget::toString()
{
     QString str;
     for(int i=0;i<palette->ColorCount;i++)
	  str += QString::number(palette->Colors[i].Red) + ","
	       + QString::number(palette->Colors[i].Green) + ","
	       + QString::number(palette->Colors[i].Blue) + ";";
     return str;
}

bool PaletteWidget::fromString(const QString& str)
{
     QStringList rgb = QString(str).split(";", QString::SkipEmptyParts);
     if(palette)
	  FreeMapObject(palette);
     palette = GifMakeMapObject(rgb.size(), NULL);
     if(!palette)
	  return false;
     size = rgb.size();
     qDebug() << "loaded palette size: " << size;
     for(int i=0;i<size;i++)
     {
	  QStringList c = rgb.at(i).split(",");
	  if(c.size() != 3)
	       return false;
	  palette->Colors[i].Red = c.at(0).toInt();
	  palette->Colors[i].Green = c.at(1).toInt();
	  palette->Colors[i].Blue = c.at(2).toInt();
     }
     setFixedSize(width(),(size/cols)*sqSize+sqSize+2);
     update();
     return true;
}

bool PaletteWidget::toFile(const QString& path)
{
     if(!palette)
	  return false;
     QFile file(path);
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return false;

     QTextStream out(&file);
     out << toString();
     file.close();

     return true;
}

bool PaletteWidget::fromFile(const QString& path)
{
     QFile file(path);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return false;
     
     return fromString(file.readAll());
}

*/
