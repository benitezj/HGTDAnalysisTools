#include <TEllipse.h>

float ASU=198;//size of ASU
float ASU_boundary=2.25;
float LRDetSeparation=1.5;
long RMAX=600;

TF1 fMax("fMax",TString("sqrt(")+RMAX+"^2 - x^2)",0,RMAX);  

void checkYMax(float X,float &Y){
  if(sqrt(X*X+Y*Y) > RMAX){ //if x,y is outside RMAX change the Y value
    if(Y>0) Y=fMax.Eval(X);
    else Y=-fMax.Eval(X);
  }
}



////// 1mm geometry
void drawSensors1mmGeometry(int side, bool bottom=0){

  Float_t x_start[23] = {120, 114, 90, 1, 41, 81, 121, 161, 201, 161, 241, 241, 241, 281, 321, 361, 361, 361, 361, 401, 441, 481, 481};
  Float_t x_size[23] = {524, 524, 542, 40, 40, 40, 40, 40, 40, 470, 380, 362, 40, 40, 40, 236, 218, 200, 40, 40, 40, 56, 20};
  
  Float_t y_start[23] = {1, 41, 81, 120, 114, 121, 121, 161, 161, 121, 161, 201, 241, 241, 241, 241, 281, 321, 361, 361, 361, 361, 401};
  Float_t y_size[23] = {40, 40, 40, 524, 524, 506, 506, 452, 452, 40, 40, 40, 344, 326, 308, 40, 40, 40, 164, 146, 110, 40, 40};
  
  TBox sensor;
  sensor.SetFillColor(0);
  sensor.SetFillStyle(0);
  sensor.SetLineColor(4);
  sensor.SetLineWidth(1);
  sensor.SetLineStyle(1);

  for(int s=0;s<23;s++){
    sensor.DrawBox(x_start[s],y_start[s],x_start[s]+x_size[s],y_start[s]+y_size[s]);
    sensor.DrawBox(-x_start[s],y_start[s],-(x_start[s]+x_size[s]),y_start[s]+y_size[s]);
    sensor.DrawBox(-x_start[s],-y_start[s],-(x_start[s]+x_size[s]),-(y_start[s]+y_size[s]));
    sensor.DrawBox(x_start[s],-y_start[s],x_start[s]+x_size[s],-(y_start[s]+y_size[s]));
  }

}



//////////////////////////////Old 1mm + 3mm geometry
void drawASU(int side, bool halfX, bool halfY, float x, float y, bool sensors=0, int cellsize=0, int trigger=0){
  //draws active area in ASU
  //ignores dead region between sensors
  
  TLine line;
  line.SetLineWidth(1);
  // if(cellsize==3)line.SetLineColorAlpha(kBlack, 0.15);
  // else line.SetLineColorAlpha(kBlack, 0.05);
  //line.SetLineColorAlpha(kBlack, 0.3);
  line.SetLineColor(1);


  float X1;
  float Y1;
  float X2;
  float Y2;

  /////////////////////////////
  //show module boundaries (will be placed at the sensor boundary, not ASU)
  /////////////////////////////
								       
  //bottom
  X1=side*(x+ASU_boundary);
  X2=side*(x+(halfX?ASU/2:ASU)-ASU_boundary);
  Y1=y+ASU_boundary;
  Y2=Y1;
  checkYMax(Y1,X1);
  checkYMax(Y2,X2);
  if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX) ;//remove spurious dots on edge
  else line.DrawLine(X1,Y1,X2,Y2);

  //top
  X1=side*(x+ASU_boundary);
  X2=side*(x+(halfX?ASU/2:ASU)-ASU_boundary);
  Y1=y+((halfY?ASU/2.:ASU)-ASU_boundary);
  Y2=Y1;
  checkYMax(Y1,X1);
  checkYMax(Y2,X2);
  if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX);//remove spurious dots on edge
  else line.DrawLine(X1,Y1,X2,Y2);
  
  //left
  X1=side*(x+ASU_boundary);
  X2=X1;
  Y1=y+ASU_boundary;
  Y2=y+(halfY?ASU/2.:ASU)-ASU_boundary;
  checkYMax(X1,Y1);
  checkYMax(X2,Y2);
  if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX);//remove spurious dots on edge
  else line.DrawLine(X1,Y1,X2,Y2);

  //right
  X1=side*(x+(halfX?ASU/2.:ASU)-ASU_boundary);
  X2=X1;
  Y1=y+ASU_boundary;
  Y2=y+(halfY?ASU/2.:ASU)-ASU_boundary;
  checkYMax(X1,Y1);
  checkYMax(X2,Y2);
  if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX);//remove spurious dots on edge
  else line.DrawLine(X1,Y1,X2,Y2);


  if(sensors){
    ///horizontal middle line
    X1=side*(x+ASU_boundary);
    X2=side*(x+(halfX?ASU/2.:ASU)-ASU_boundary);
    Y1=y+ASU/2-(halfY?ASU_boundary:0);
    Y2=Y1;
    checkYMax(Y1,X1);
    checkYMax(Y2,X2);
    if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX) ;//remove spurious dots on edge
    else line.DrawLine(X1,Y1,X2,Y2);

    //vertical middle line
    X1=side*(x+ASU/2-(halfX?ASU_boundary:0));
    X2=X1;
    Y1=y+ASU_boundary;
    Y2=y+(halfY?ASU/2.:ASU)-ASU_boundary;
    checkYMax(X1,Y1);
    checkYMax(X2,Y2);
    if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX);//remove spurious dots on edge
    else line.DrawLine(X1,Y1,X2,Y2);

  }


  if(trigger>0){

    //divide the sensor into nxn squares
    float tilesize=((ASU-2.*ASU_boundary)/2.)/trigger;

    for(int i=0;i<trigger;i++){
      //draw horizontal lines
      X1=side*(x+ASU_boundary);
      X2=side*(x+(halfX?ASU/2.:ASU)-ASU_boundary);
      Y1=y+ASU_boundary+i*tilesize;
      Y2=Y1;
      checkYMax(Y1,X1);
      checkYMax(Y2,X2);
      if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX) ;//remove spurious dots on edge
      else line.DrawLine(X1,Y1,X2,Y2);

      //draw the vertical lines
      X1=side*(x+ASU_boundary+i*tilesize);
      X2=X1;
      Y1=y+ASU_boundary;
      Y2=y+(halfY?ASU/2.:ASU)-ASU_boundary;
      checkYMax(X1,Y1);
      checkYMax(X2,Y2);
      if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX);//remove spurious dots on edge
      else line.DrawLine(X1,Y1,X2,Y2);
      
    }

  }


  if(cellsize>0){
    ///Draw the horizontal lines
    for(int i=0;i<=(ASU-2*ASU_boundary);i+=cellsize){
      X1=side*(x+ASU_boundary);
      Y1=y+ASU_boundary+i;
      X2=side*(x+(halfX?ASU/2:ASU)-ASU_boundary);
      Y2=y+ASU_boundary+i;

      checkYMax(Y1,X1);
      checkYMax(Y2,X2);
      if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX)continue;//remove spurious dots on edge
      line.DrawLine(X1,Y1,X2,Y2);
    }

    ///Draw the vertical lines
    for(int i=0;i<=(halfX?ASU/2-2*ASU_boundary:ASU-2*ASU_boundary);i+=cellsize){
      X1=side*(x+ASU_boundary+i);
      Y1=y+ASU_boundary;
      X2=side*(x+ASU_boundary+i);
      Y2=y+ASU-ASU_boundary;

      checkYMax(X1,Y1);
      checkYMax(X2,Y2);
      if(fabs(X1)>RMAX||fabs(Y1)>RMAX||fabs(X2)>RMAX||fabs(Y2)>RMAX)continue;//remove spurious dots on edge
      line.DrawLine(X1,Y1,X2,Y2);
    }
  }




}


void drawHalfDector(int side=1){
  
  //draw top row
  drawASU(side,0,0,LRDetSeparation/2,2.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+ASU,2.5*ASU,1,3);
  
  //draw second row from top
  drawASU(side,0,0,LRDetSeparation/2,1.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+ASU,1.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+2*ASU,1.5*ASU,1,3);
 
  //draw third row from top
  drawASU(side,0,0,LRDetSeparation/2,0.5*ASU,1,1);
  drawASU(side,1,0,LRDetSeparation/2+ASU,0.5*ASU,1,1);
  drawASU(side,0,0,LRDetSeparation/2+1.5*ASU,0.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+2.5*ASU,0.5*ASU,1,3);

  //draw fourth row from top (only half in y direction)
  drawASU(side,0,0,LRDetSeparation/2+0.5*ASU,-0.5*ASU,1,1);
  drawASU(side,0,0,LRDetSeparation/2+1.5*ASU,-0.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+2.5*ASU,-0.5*ASU,1,3);
 
  //draw fifth row from top
  drawASU(side,0,0,LRDetSeparation/2,-1.5*ASU,1,1);
  drawASU(side,1,0,LRDetSeparation/2+ASU,-1.5*ASU,1,1);
  drawASU(side,0,0,LRDetSeparation/2+1.5*ASU,-1.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+2.5*ASU,-1.5*ASU,1,3);

  //draw sixth row from top
  drawASU(side,0,0,LRDetSeparation/2,-2.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+ASU,-2.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+2*ASU,-2.5*ASU,1,3);

  //draw bottom row
  drawASU(side,0,0,LRDetSeparation/2,-3.5*ASU,1,3);
  drawASU(side,0,0,LRDetSeparation/2+ASU,-3.5*ASU,1,3);
  

  ////Draw the outer boundary
  TEllipse * OutBoundary=new TEllipse(0,0,RMAX,RMAX);
  OutBoundary->SetFillStyle(0);
  OutBoundary->SetLineWidth(2);
  OutBoundary->Draw();
}


void drawCellGridDetGeo(){//draws positive x side of detector by default
  drawHalfDector(1);
  drawHalfDector(-1);
}


void drawSensors(int side, bool bottom=0){
  //
  drawASU(side,0,0,LRDetSeparation/2,2.5*ASU,1);
  drawASU(side,0,0,LRDetSeparation/2+ASU,2.5*ASU,1);

  //
  drawASU(side,0,0,LRDetSeparation/2,1.5*ASU,1);
  drawASU(side,0,0,LRDetSeparation/2+ASU,1.5*ASU,1);
  drawASU(side,0,0,LRDetSeparation/2+2*ASU,1.5*ASU,1);

  //
  drawASU(side,0,0,LRDetSeparation/2,0.5*ASU,1);
  drawASU(side,1,0,LRDetSeparation/2+ASU,0.5*ASU,1);
  drawASU(side,0,0,LRDetSeparation/2+1.5*ASU,0.5*ASU,1);
  drawASU(side,0,0,LRDetSeparation/2+2.5*ASU,0.5*ASU,1);

  //
  if(!bottom){
    drawASU(side,0,1,LRDetSeparation/2+0.5*ASU,0,1);
    drawASU(side,0,1,LRDetSeparation/2+1.5*ASU,0,1);
    drawASU(side,0,1,LRDetSeparation/2+2.5*ASU,0,1);
  }else{
    drawASU(side,0,0,LRDetSeparation/2+0.5*ASU,-0.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+1.5*ASU,-0.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+2.5*ASU,-0.5*ASU,1);
  }


  if(bottom){
    drawASU(side,0,0,LRDetSeparation/2,-1.5*ASU,1);
    drawASU(side,1,0,LRDetSeparation/2+ASU,-1.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+1.5*ASU,-1.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+2.5*ASU,-1.5*ASU,1);
    
    //
    drawASU(side,0,0,LRDetSeparation/2,-2.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+ASU,-2.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+2*ASU,-2.5*ASU,1);
    
    //
    drawASU(side,0,0,LRDetSeparation/2,-3.5*ASU,1);
    drawASU(side,0,0,LRDetSeparation/2+ASU,-3.5*ASU,1);

    TEllipse * OutBoundary=new TEllipse(0,0,RMAX,RMAX);
    OutBoundary->SetFillStyle(0);
    OutBoundary->SetLineWidth(2);
    OutBoundary->Draw();

  }

}


////////////TRIGGER/////////////////////////////////
void drawTriggerTiles(int side=1,int bottom=0){
 
  if(bottom){
    TEllipse * OutBoundary=new TEllipse(0,0,RMAX,RMAX);
    OutBoundary->SetFillStyle(0);
    OutBoundary->SetLineWidth(2);
    OutBoundary->Draw();
  }
  
  //drawSensorsQ1(side);

  //2x2 tiles
  drawASU(side,0,0,LRDetSeparation/2,2.*ASU,0);
  drawASU(side,0,0,LRDetSeparation/2+ASU,2.*ASU,0);
  drawASU(side,0,0,LRDetSeparation/2+2*ASU,1.*ASU,0);
  drawASU(side,0,0,LRDetSeparation/2+2*ASU,0.*ASU,0);

  ///1x1 tiles
  drawASU(side,1,1,LRDetSeparation/2+0.0*ASU,1.5*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,1.5*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,1.5*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,1.5*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,1.0*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,1.0*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,0.5*ASU,1);
  drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,0.0*ASU,1);

  /// 1/2 x 1/2 tiles
  drawASU(side,1,1,LRDetSeparation/2,1.0*ASU,1,0,2);
  drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,1.0*ASU,1,0,2);
  drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,0.5*ASU,1,0,2);
  drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,0.0*ASU,1,0,2);
  drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,0.5*ASU,1,0,2);

  /// 1/3 x 1/3 tiles
  drawASU(side,1,1,LRDetSeparation/2,0.5*ASU,1,0,3);
  drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,0.0*ASU,1,0,3);

  if(bottom){
    //2x2 tiles
    drawASU(side,0,0,LRDetSeparation/2,-3.*ASU,0);
    drawASU(side,0,0,LRDetSeparation/2+ASU,-3.*ASU,0);
    drawASU(side,0,0,LRDetSeparation/2+2*ASU,-2.*ASU,0);
    drawASU(side,0,0,LRDetSeparation/2+2*ASU,-1.*ASU,0);

    ///1x1 tiles
    drawASU(side,1,1,LRDetSeparation/2+0.0*ASU,-2.0*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,-2.0*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,-2.0*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,-2.0*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,-1.5*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,-1.5*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,-1.0*ASU,1);
    drawASU(side,1,1,LRDetSeparation/2+1.5*ASU,-0.5*ASU,1);

    /// 1/2 x 1/2 tiles
    drawASU(side,1,1,LRDetSeparation/2,-1.5*ASU,1,0,2);
    drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,-1.5*ASU,1,0,2);
    drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,-1.0*ASU,1,0,2);
    drawASU(side,1,1,LRDetSeparation/2+1.0*ASU,-0.5*ASU,1,0,2);
    drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,-1.0*ASU,1,0,2);

    /// 1/3 x 1/3 tiles
    drawASU(side,1,1,LRDetSeparation/2,-1.0*ASU,1,0,3);
    drawASU(side,1,1,LRDetSeparation/2+0.5*ASU,-0.5*ASU,1,0,3);

  }

}

