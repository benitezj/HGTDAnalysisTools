


void drawGridQuadrant(int xs,int ys){//xs,ys=+-1


    Double_t X; 
    Double_t Y; 
    Double_t ymin = 0; 
    Double_t ymax = 0; 
    Double_t xmin = 0; 
    Double_t xmax = 0;
    TLine line;
    line.SetLineWidth(1);

    long rmax=600;
    long rmid=280;
    long rmin=50;

    TF1 f1("f1",TString("sqrt(")+(rmax)+"^2 - x^2)", 0, rmax);  //eta=2.4 at r=648mm 
    TF1 f2("f2",TString("sqrt(")+(rmid)+"^2  - x^2)", 0, rmid); //eta=3.2 at r=289mm
    TF1 f3("f2",TString("sqrt(")+(rmin)+"^2  - x^2)", 0, rmin); //eta=5 at r=48mm

    ///3x3mm cells
    line.SetLineColorAlpha(kBlack, 0.30);
    for(X = 3; X < rmax; X+=3){
      ymax = f1.Eval(X); 
      if(X<rmid) ymin = f2.Eval(X);  
      else ymin = 0; 
      line.DrawLine(xs*X, ys*ymin, xs*X, ys*ymax); 
    }
    for(Y = 3; Y <rmax; Y+=3){
      xmax = f1.Eval(Y); 
      if(Y<rmid) xmin = f2.Eval(Y);
      else xmin = 0; 
      line.DrawLine(xs*xmin, ys*Y, xs*xmax, ys*Y); 
    }

    //1x1mm cells
    line.SetLineColorAlpha(kBlack, 0.10);
    for(X=1; X<rmid; X++){
      if(X<=rmin) ymin = f3.Eval(X);  
      else ymin = 0; 
      ymax = f2.Eval(X); 
      line.DrawLine(xs*X, ys*ymin, xs*X, ys*ymax); 
    }
    for(Y=1; Y<rmid; Y++){
      if(Y<=rmin) xmin = f3.Eval(Y);  
      else xmin = 0; 
      xmax = f2.Eval(Y); 
      line.DrawLine(xs*xmin, ys*Y, xs*xmax, ys*Y); 
    }

}

void drawCellGrid(){

  drawGridQuadrant(1,1);
  drawGridQuadrant(-1,1);
  drawGridQuadrant(-1,-1);
  drawGridQuadrant(1,-1);

}

