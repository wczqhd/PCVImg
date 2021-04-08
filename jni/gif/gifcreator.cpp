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
#include "gifcreator.h"
#include <cstring>

#include <iostream>
using namespace std;

#define PrintGifError()  ;  
GifCreator::GifCreator():duration(0.1), revStart(-1)
{
}

GifCreator::~GifCreator()
{
     //usuwanie powt贸rze艅
     for(int i=0;i<cmaps.size()-1;i++)
	  for(int j=i+1;j<cmaps.size();j++)
	       if(cmaps.at(i) == cmaps.at(j))
	       {
		    cmaps.erase(cmaps.begin()+j);
		    j--;
	       }
     for(int i=0;i<cmaps.size();i++)
	  GifFreeMapObject(cmaps[i]);
}


int GifCreator::save(const char* filename, int every)
{
     if (!frames.size() || !cmaps.size()) 
     {
	  cerr << "ERROR: no frames or color map found!";
	  cerr << "frames: " << frames.size() << "\nmaps: " << cmaps.size() << endl;
	  return 0;
     }
  
  int nErr = 0;
     GifFileType *GifFile = EGifOpenFileName(filename, false,&nErr);
  
     if (!GifFile){
	  PrintGifError();
	  return 1;
     }

     if (EGifPutScreenDesc(
	      GifFile,
	      w, h, colorRes, 0, cmaps.size() > 1 ? NULL : cmaps.at(0)
	      ) == GIF_ERROR){
	  PrintGifError();
	  return 2;
     }

  
     // ------ loop ------
     int loop_count;
     loop_count=0;
     
	  char nsle[12] = "NETSCAPE2.0";
	  char subblock[3];
	//  if (EGifPutExtensionFirst(GifFile, APPLICATION_EXT_FUNC_CODE, 11, nsle) == GIF_ERROR) {
	  if (EGifPutExtensionLeader(GifFile, 255) == GIF_ERROR) { //杩欎釜鏇挎崲鍙兘鏈夐棶棰�
	       PrintGifError();
	       return 3;
	  }
	  subblock[0] = 1;
	  subblock[2] = loop_count % 256;
	  subblock[1] = loop_count / 256;
/*
	  EGifPutExtensionLeader(_gif, 255);
        EGifPutExtensionBlock(_gif, 11, data + 3);
        EGifPutExtensionBlock(_gif, 3, data + 15);
        EGifPutExtensionTrailer(_gif);*/
	   EGifPutExtensionBlock(GifFile, 11, nsle);
        EGifPutExtensionBlock(GifFile, 3, subblock);
	  
      if(  EGifPutExtensionTrailer(GifFile) == GIF_ERROR)
	  {//杩欎釜鏇挎崲鍙兘鏈夐棶棰�
	       PrintGifError();
	       return 4;
	  }
 
     
     //------------------


     for (int ni=0; ni<frames.size(); ni+=every) 
	 {      
     //  if(!savingProgress((float)ni/(frames.size()/every)*100))
	   //           return true; //przerwanie przez usera
    
	  unsigned char ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
 
    
	  ExtStr[0] = (false) ? 0x06 : 0x04;
	  // ExtStr[1] = delay[ni] % 256;
	  // ExtStr[2] = delay[ni] / 256;
    
	  int dt = duration*100.0f;
	  ExtStr[1] = dt % 256;
	  ExtStr[2] = dt / 256;

	  /* Dump graphics control block. */
	  EGifPutExtension(GifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr);
            
	  if (EGifPutImageDesc(
		   GifFile,
		   0, 0, w, h, false, cmaps.size() > ni ? cmaps.at(ni) : cmaps.at(cmaps.size()-1)
		   ) == GIF_ERROR) 
	  {
	       PrintGifError();
	       endProgress();
	       return 5;
	  }
       
       
	  for (int y = 0, j=(h-1)*w; y < h; y++, j-=w) 
	  {
	       if (EGifPutLine(GifFile, &(frames[ni][j]), w) == GIF_ERROR)
		   {
		    PrintGifError();
		    endProgress();
		    return 6;
	       }
	  }
     }

     //comment
     if(EGifPutComment(GifFile,"GIF  cool") == GIF_ERROR){
	  PrintGifError();
	  endProgress();
	  return 7;
     }
   
     if (EGifCloseFile(GifFile) == GIF_ERROR) {
	  PrintGifError();
	  endProgress();
	  return 8;
     }

     endProgress();
	// AfxMessageBox("wwwwwwwww");
     return 9;
}

void GifCreator::appendReversedCopy()
{
     if(frames.size() < 2)
	  return;
     if(cmaps.size() > 1 && cmaps.size() != frames.size())
	  cerr << "WARNING: going to append reversed copy of frames but palettes probably won't match!" << endl;
     revStart = frames.size();
     for(int i=frames.size()-2;i>0;i--)
	  frames.push_back(frames.at(i));

     if(cmaps.size() > 1)
	  for(int i=cmaps.size()-2;i>0;i--)
	       cmaps.push_back(cmaps.at(i));
}

void GifCreator::removeReversedCopy()
{
     if(revStart < 0)
	  return;

     while(frames.size() > revStart)
	  frames.erase(frames.begin()+frames.size()-1);
     while(cmaps.size() > revStart)
	  cmaps.erase(cmaps.begin()+cmaps.size()-1);
}
