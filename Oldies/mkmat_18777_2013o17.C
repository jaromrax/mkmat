#include <unistd.h>  //sleep 
//#include "cuts_manip.h"  // NEW - Instead of defined functions here....
/*
 *    a   THIS WILL CREATE  MATRIX FROM A TELESCOPE
 *    
 *  .L mkmat.C 
 *  loadcuts()  
 *  batchmat( 47, 2 )
 *
 *  mkmat1( 
 *  mkmat1(-1,"V","e<10") 
 *  mkmat1(-1,"V","") 
 */
/*
 *
 * PROBLEM BINNING:
 *  
 *
mkmat1(1, "V",  "V001<10")              
is an example (bad btw  as  v017*0.4950 is drq) of spikes 
 *
 *
 */


/**********************************************************************************
 *
 *   XML
 *
 *
 **********************************************************************************/


/*
 *  no dependence on libxml2
 */

#include "TXMLEngine.h"
#include "TString.h" // good for xml

#include "TCutG.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TFile.h"
#include "TROOT.h"
// string
#include <stdio.h>
// FileExists:
#include <sys/stat.h> 
//
#include "TTree.h"
#include "TH1.h"
#include "TPad.h"


void DisplayTele(TXMLEngine* xml, XMLNodePointer_t node, Int_t level, const char* search );
/*
 * DRIVING ALL THE XML 
 */
std::string addr[4]={"telescopes", "t1", "V" , "a_e" };
int leveloffset=-1;
int getattrlevel=3;
char output[100];
TXMLEngine* xml;
XMLNodePointer_t mainnode ;
XMLDocPointer_t xmldoc;


void xmlreadfile_init(const char* filename = "example.xml")
{
   xml = new TXMLEngine;
   xmldoc = xml->ParseFile(filename);
   if (xmldoc==0) {
      delete xml;      return;  
   }
   // take access to main node   
    mainnode = xml->DocGetRootElement(xmldoc);
}//xmlreadfile_init

double xmlread_a_e(int tele, const char* VOL){
  char tmpdet[20];// t1....
    //  V, addwith
    addr[2]= VOL;  addr[3]= "a_e" ; 
    leveloffset=-1; sprintf( tmpdet, "t%d", tele );
    addr[1]=tmpdet;
   DisplayTele(xml, mainnode, 0, "telescopes" );
   //   printf("GO ON with %f\n",  atof(output) ); 
   return  atof(output);
}



double xmlread_a_de(int tele, const char* VOL){
  char tmpdet[20];// t1....
    //  V, addwith
    addr[2]= VOL;  addr[3]= "a_de" ; 
    leveloffset=-1; sprintf( tmpdet, "t%d", tele );
    addr[1]=tmpdet;
   DisplayTele(xml, mainnode, 0, "telescopes" );
   //   printf("GO ON with %f\n",  atof(output) ); 
   return  atof(output);
}


int xmlread_blklow(int tele, const char* VOL){
  char tmpdet[20];// t1....
    //  V, addwith
    addr[2]= VOL;  addr[3]= "blklow" ; 
    leveloffset=-1; sprintf( tmpdet, "t%d", tele );
    addr[1]=tmpdet;
   DisplayTele(xml, mainnode, 0, "telescopes" );
   //   printf("GO ON with %f\n",  atof(output) ); 
   return  atof(output);
}

int xmlread_dechan(int tele, const char* VOL){
  char tmpdet[20];// t1....
    //  V, addwith
    addr[2]= VOL;  addr[3]= "dechan" ; 
    leveloffset=-1; sprintf( tmpdet, "t%d", tele );
    addr[1]=tmpdet;
   DisplayTele(xml, mainnode, 0, "telescopes" );
   //   printf("GO ON with %f\n",  atof(output) ); 
   return  atoi(output);
}

int xmlread_echan(int tele, const char* VOL){
  char tmpdet[20];// t1....
    //  V, addwith
    addr[2]= VOL;  addr[3]= "echan" ; 
    leveloffset=-1; sprintf( tmpdet, "t%d", tele );
    addr[1]=tmpdet;
   DisplayTele(xml, mainnode, 0, "telescopes" );
   //   printf("GO ON with %f\n",  atof(output) ); 
   return  atoi(output);
}

/*
   //
   //  DEFINE  (PATH and)    ATTRIBUTE
   //   
   leveloffset=-1;addr[3]= "addwith" ; 
   DisplayTele(xml, mainnode, 0, "telescopes" );
   printf("GO ON with %f\n",  atof(output) ); 

   leveloffset=-1;  addr[3]= "dechan" ;
   DisplayTele(xml, mainnode, 0, "telescopes" );
   printf("GO ON with %d\n",  atoi(output) ); 
*/
void xmlreadfile_fini()
{
   // Release memory before exit
   xml->FreeDoc(xmldoc);
   delete xml;
}//xmlreadfile_init



void DisplayTele(TXMLEngine* xml, XMLNodePointer_t node, Int_t level, const char* search ) 
{
 
  TString tel;
  TString sat;

  tel.Append( xml->GetNodeName(node)  );
  //    printf("<%s>\n", tel.Data() );


  if ( (leveloffset<0) ){
    if (tel.CompareTo( search )==0){
      leveloffset=level;  sprintf( output, "%s", "" );
      //   printf("Found level offset.... ==%d\n", leveloffset );
    }else{//neni to on, hledej
       // display all child nodes   
      XMLNodePointer_t child = xml->GetChild(node);
      while (child!=0) {
	DisplayTele(xml, child, level+1, search ); 
         child = xml->GetNext(child);
      }//while
    }
  }// HLEDEJ dokud neinicializujes



  if (leveloffset>=0){ // 
    if ( tel.CompareTo( search  )==0){
      //      printf("%*c<%s>\n",level+1,' ', addr[ level- leveloffset ].c_str()   );
      // display all child nodes   
      if (getattrlevel<=level){
	//	  printf("reached attr  level .... ==%d\n", level );

	// display attributes
	XMLAttrPointer_t attr = xml->GetFirstAttr(node);
	while (attr!=0) {
	  sat=xml->GetAttrName(attr);
	  //	  printf(" Checking   attr =%s/%s\n", sat.Data(), addr[ getattrlevel ].c_str()  );
	  if ( sat.CompareTo( addr[ getattrlevel ].c_str() )==0){
	    sprintf( output, "%s",   xml->GetAttrValue(attr) );
	    printf("%*c  attr <%s = %s>\n",level+1,' ',
		   sat.Data() ,  output  );
	  }// attr OK
	  // printf("%*c attr: %s value: %s\n",level+1,' ', xml->GetAttrName(attr), xml->GetAttrValue(attr));
	  attr = xml->GetNextAttr(attr);  
	}//display attributes
     }else{ 
      XMLNodePointer_t child = xml->GetChild(node);
      while (child!=0) {
	//	printf("searching %s\n", addr[level- leveloffset +1].c_str()  );
	DisplayTele(xml, child, level+1, addr[level- leveloffset +1].c_str() ); 
         child = xml->GetNext(child);
      }//reached attrlevel
      }//else attrlevel  
    }//compare OK
    
  }
  return;
}//DisplayTele------------------------------------------







/**********************************************************************************
 *
 *   cuts
 *
 *
 **********************************************************************************/





/*
 *      save the defined cut into    cuts.root
 */
/*
void savecut(TCutG *cut, const char* name){
  TDirectory *dir=gDirectory;
  TCutG *newcut=(TCutG*)cut->Clone( name ); // better to clone before (other dir...)
  TFile *nf=new TFile("cuts.root", "UPDATE");
  newcut->Write();                         
  nf->ls();
  nf->Close();
  dir->cd();
  //  dir->ls();
}//savecut--------------

void loadcuts(){
  gROOT->GetListOfSpecials()->ls();// ORIGINAL
  TDirectory *dir=gDirectory;
  TFile *nf=new TFile("cuts.root", "UPDATE");
  int n=gDirectory->GetNkeys();
  if (gDirectory->GetListOfKeys()){
      TObject *o;
      int max=gDirectory->GetList()->GetEntries();
      max=gDirectory->GetListOfKeys()->GetEntries();
      for (int iii=0 ; iii<max ; iii++ ){
	TString sa1=gDirectory->GetListOfKeys()->At(iii)->GetName();
	  gDirectory->GetObject( sa1.Data() , o );
	  TString sa2=o->ClassName();
	  // important check - else it makes double entries...
	  if ((sa2.Index("TCutG")==0)&&(gROOT->GetListOfSpecials()->FindObject(o)==NULL)) {
	    //	  if ((sa2.Index("TCutG")==0)&&(gDirectory->FindObject(o)==NULL)) {
	    //	    gDirectory->Add( (TH1F*)o );
	    gROOT->GetListOfSpecials()->Add( (TCutG*)o );
	  }// TCutG
      }
  }//gDirectory->GetListOfKeys()

  nf->ls();
  nf->Close();
  dir->cd();
  gROOT->GetListOfSpecials()->ls();
}//savecut--------------
//  remove   rename.............
 // gDirectory->rmdir("cutt7d") !!!
 // cutt7p->Clone("cutt7pV");cutt7pV->Write()
 //



void rmcut(const char* name, int version=0){
  if (version==0){
    printf("make a backup and use the version number  ;1 ;2\n","");
    return;
  }
    TFile *nf=new TFile("cuts.root", "UPDATE");
    char name2[100];
    sprintf( name2 , "%s;%d", name, version );
    printf("deleting  %s\n", name2  );
    gDirectory->rmdir( name2 );
    nf->ls();
    nf->Close();
}


void cpcut(const char* name, const char* newname){

  TFile *nf=new TFile("cuts.root", "UPDATE");
  TObject *o;
  gDirectory->GetObject( name , o );
  TCutG* cut=(TCutG*)o;
  TCutG* newcut=(TCutG*)cut->Clone( newname );
  newcut->Write();
    //  newcut->Print();  newcut->Draw("pawl");
  nf->ls();
  nf->Close();

}



********************************CUTS */


/**********************************************************************************
 *
 *   1kB +   file exists
 *
 *
 **********************************************************************************/




bool FileExists(string strFilename, int minsize) { 
  struct stat stFileInfo; 
  bool blnReturn; 
  int intStat; 

  // Attempt to get the file attributes 
  intStat = stat(strFilename.c_str(),&stFileInfo); 
  if(intStat == 0) { 
    // We were able to get the file attributes 
    // so the file obviously exists. 
    printf( "SIZE==%d\n", stFileInfo.st_size ); 
    if (stFileInfo.st_size<minsize){ blnReturn = false;
    }else{
    blnReturn = true; 
    }
  } else { 
    // We were not able to get the file attributes. 
    // This may mean that we don't have permission to 
    // access the folder which contains this file. If you 
    // need to do that level of checking, lookup the 
    // return values of stat which will give you 
    // more details on why stat failed. 
    printf( "Not exist ....size==%d\n", stFileInfo.st_size ); 
    blnReturn = false; 
  } 
   
  return(blnReturn); 
}






/**********************************************************************************
 *
 *   mkmat
 *
 *
 **********************************************************************************/


/*
 *
 *  MAIN  LINE
 *
 *   1/coef
 *   2/ de[0]  e[0]   jsou kanaly od detektoru
 *   tele <0   ==>   dE + 1/f*E
 *
 */
void mkmat(int teleIN, int count=-1, int bins=2, TCutG *cut=NULL, int dimen=2 , 
	   const char* VOL="V", const char* CONDITION="" ,double coefover=0.0 ){

  int reverse=0;   //  0:   dE* coef + E
      reverse=1;   //  1:   dE  + coef*E
  int tele=teleIN;

  double coef[16]; // pro jistotu (8 needed)

  if (tele<0){ printf("1D PLOTTING IN REVERSE (dE + 1/f*E)\n",""); reverse=1; tele=-1*tele;}

   printf(" making matrix for telescope T%d%s, #%d, 5000/bins :%d cut@%d %dDIM\n", 
	 tele,VOL, count, bins, (int)cut, dimen );


  /*
   * ONCE THIS COEF is set,  you cannot change again because of !cuts!
   *  coenficients for de:de + 0.49*e. By default I put 0.5...
   *  then I need best resolution == finetune...
   *
   *  POZOR NA re-bin artefakty
   */

  int de[8], e[8];
  char cmd[500];
  char cmdcond[500];
  char sde[10];
  char se[10];
  char telename[10];  // T1
  char spename[10];  // T1V  or  T1V1D
  char randomization[50];// rand()


  

  if (  FileExists("mkmat.xml",33) ){  //minimum size 
    
    xmlreadfile_init("mkmat.xml");

     coef[tele-1]=xmlread_a_e( tele, VOL );
     de[tele-1]=  xmlread_dechan(  tele, VOL );
     e[tele-1]=   xmlread_echan(   tele, VOL );

    xmlreadfile_fini();
  }//  XML FILE EXISTS------------------------------
  else{
    printf("NO XML FILE \n\n \n%s", "");
    return;
  }
  //-----------------------------------------------XML OK

  
  if (coefover!=0){
    coef[tele-1]=coefover;
  }
  printf("coeficient :         %f\n\n", coef[tele-1] );



  sprintf( sde,"%s%03d",    VOL, de[tele-1] );
  sprintf(  se,"%s%03d",    VOL, e[tele-1] );
  sprintf(  telename,"T%d%s",  tele,  VOL );// T1V
  sprintf(  spename,"T%d%s",  tele,  VOL );//  (T1V or T1V1D)SPECTRUM NAME (redefine later if 1D)

  // if I do V017+0.5-rand(0,1) => spe is shifted!
  //         V017+rand(0,1)     =>  is correct...
  //  sprintf(randomization,"+0.5-1e-3*(rand()%%1e3)","" );//  0.456*(  V001 R )
  sprintf(randomization,"+1e-3*(rand()%%1e3)","" );//  0.456*(  V001 R)  R="" or "+x"



  //COMMAND  - 1D   or  2D ..................
  char option[20]=""; //"col" for bidim

  if (dimen==2){
  sprintf( cmd,    "%s:%s+%s*%.5f>>%s( %d,0,5000,%d,0,5000 )",
	   sde,sde,se,   coef[tele-1],  spename , 5000/bins , 5000/bins );
  sprintf( option,"%s","col" );
  }//dim==2


  if (dimen==1){
  sprintf(  spename,"T%d%s1D",  tele,  VOL );//  SPECTRUM NAME REDEFINE
  //
  // with rand: 25sec
  // wo   rand: 24sec
  //  sprintf( cmd,    "%s+(%s)*%.4f>>%s( 5000,0,5000 )",   // SIMPLE BUT WITH SPIKES
  //  sprintf( cmd,    "%s+(%s+1e-3*(rand()%%1e3))*%.4f>>%s( 5000,0,5000 )",
  //	    se,sde,   coef[tele-1],  spename  );// better automaticaly:
  /*
   *   dE * factor + E   ===>>>  This is how normaly we plot the matrix.
   */
  if (reverse==0){
  sprintf( cmd,    "%s+(%s %s)*%.4f>>%s( 5000,0,5000 )",
	   se,sde, randomization,  coef[tele-1],  spename  );
  }
  /*   reverse------------------------------------------------------
   *   dE  +  factor *E   ???  10000 bins! ==>> but consistent with dE...
   */
  if (reverse==1){
  sprintf( cmd,    "%.6f*(%s %s)+%s>>%s( 10000,0,10000 )",
	   coef[tele-1], se, randomization, sde,  spename  );
  }
  //	   se,sde, randomization,  coef[tele-1],  spename  );
  sprintf( option,"%s","" );
 }//dim==1





  // CUT - IF ANY - AND dE>0      -------------------------------
  if (cut!=NULL){
    sprintf( cmdcond,"%s>0 && %s",   sde , cut->GetName()   );// USE CUT
  }else{
    sprintf( cmdcond,"%s>0",   sde );     //  at least dE >0 !!!!!1
  }
 



  //problem....         V001 V002.........
  // I solve it by replacing     de->  V001
  //                             e->   V017
  // simply - another fine interpreter.

  if (strlen(CONDITION)>1){
    TString cono=CONDITION;
    cono.ReplaceAll("de", sde );
    cono.ReplaceAll("e", se );

    cono.ReplaceAll("dE", sde );
    cono.ReplaceAll("E", se );
    sprintf( cmdcond,"%s && %s",cmdcond,cono.Data() );//at least dE >0 !!1
  } // if condition:  reinterpret it

  // GRAPHICS OFF
  //  sprintf( option,"%s%s",option, "goff" );
  printf("\"%s\"  , \"%s\" ,  \"%s\"   \n", cmd, cmdcond,  option );
  
  printf("drawing ttree ............\n%s", "");
  if (count>0){   nanot->Draw(cmd, cmdcond,  option  , count );}
  if (count<=0){  nanot->Draw(cmd, cmdcond,  option          );}


  //  gDirectory->ls();
  printf("searching <%s>\n", spename );
  TH1* h2;
  //  TH2F* h2=(TH2F*)gDirectory->Get( telename );
  //  sleep(1);
  h2=(TH1*)gDirectory->FindObject( spename ); 
  //  if ( dimen==2){  h2=(TH1*)gDirectory->FindObject( telename );  }
  //  if ( dimen==1){  h2=(TH1*)gDirectory->FindObject( telename );  }

  h2->Print();
  TString hname=h2->GetTitle(  );
  hname.Append(" made in ");
  hname.Append( gFile->GetName() );
  h2->SetTitle( hname.Data()  );

  //"nanot->Draw(\"V022:V006+V022*0.535>>h6a(200,2500,2700,200,750,950)\",\"V022>0\",\"col\" )
  //goff  gPad->Modified();  gPad->Update();
}//mkmat




/*
 *      1D  spectrum  based on cut
 */
void mkmat1(int tele=0 ,TCutG *cut=NULL, int count=-1, int bins=2, double coefover=0.0 ){
  int dimension=1;
  char VOL[10]="V";
  mkmat(tele, count, bins, cut, dimension , VOL ,"", coefover );  // DIM ==1
}// WITH CUT - 1DIM

/*
 *      1D  spectrum  based on TEXT COND
 */
void mkmat1(int tele=0, const char* COND="", int count=-1, int bins=2, double coefover=0.0 ){
  int dimension=1;
  char VOL[10]="V";
  mkmat(tele, count, bins, NULL, dimension , VOL, COND, coefover   );  // DIM ==1
}// WITH TEXT COND - 1DIM



/*
 *      2D  spectrum,  no cut,     6,"V"
 */
void mkmat(int tele=-1 ){
  int dimension=2;
  int bins=2;
  const char* VOL
  mkmat(tele, -1, bins, NULL, dimension , VOL );  // DIM ==1
}// WITH CUT - 1DIM

















/************************************************************************************
 *    batch use of mkmat:   open file, save to another file.... T6spectra.root
 *
 ************************************************************************************
 */
void batchmat(int run,  int  det=6, const char* VOL="V" ){

  //  TChain *c=new TChain("nanot");
  TString s;
  char fname[180];
  char fnamespe[180];
  char namespe[180];

  char telename[30];// T6
  char telename1D[30];// T61D - original name
  char telenamed[33];// T6d
  char telenamep[30];// T6p
  char telenamede[30];// T6de

  char cutdname[30];// cutt6d
  char cutpname[30];// cutt6p

  // accept  both 003 or 0003......
  sprintf( fname, "RUN%03d_%s.root", run,  VOL );
  if (  FileExists( fname,1000 )  ){
    printf("File %s exists\n", fname);   
  }else{
    sprintf( fname, "RUN%04d_%s.root", run,  VOL );
  }


  sprintf( namespe, "T%d_run%03d_%s", det, run, VOL  );//spectrum name
  sprintf( fnamespe, "T%d%sspectra.root", det, VOL );       //filename

  // these names must correspond to mkmat    T2V  T2V1D
  sprintf( telename, "T%d%s", det, VOL );       //telename
  sprintf( telename1D, "T%d%s1D", det, VOL );      //telename 1D
  sprintf( telenamed, "T%d_d_run%03d_%s", det, run, VOL);       //telename d
  sprintf( telenamep, "T%d_p_run%03d_%s", det, run, VOL );       //telename p
  sprintf( telenamede, "T%d_de_run%03d_%s", det, run, VOL );       //telename de

  sprintf( cutdname, "cutt%dd%s", det, VOL );       //cutname d 
  sprintf( cutpname, "cutt%dp%s", det, VOL );       //cutname p



  /*
   *        READ THE FOLLOWING  ROOT  FILE-----------------------------------------
   */
  if ( ! FileExists( fname,1000 )  ){//  FILE root DOES EXIST..................
       printf("file %s doesnot exist or is zombie\n", fname  );
 }else{
       printf("file %s exists OK\n", fname  );


  TFile *f=new TFile( fname ,"READ");  //   read from
  TTree *t=(TTree*)f->Get("nanot");
  /*
   *    CREATE  MATRIX   (and  d  and p spectra as well?)
   */
  mkmat( det, VOL  );
  TH2F* t6bi=(TH2F*)gDirectory->Get(  telename  );

   /*
    *    CREATE  1DIM   (and  d  and p spectra as well?)
    */
  printf( "searching for TCutG : %s, %s\n", cutdname, cutpname );
  TCutG *mycutd=(TCutG*)gROOT->GetListOfSpecials()->FindObject( cutdname );
  TCutG *mycutp=(TCutG*)gROOT->GetListOfSpecials()->FindObject( cutpname );

  TH1F* t61d_d;
  TH1F* t61d_p;
  TH1F* t61d_de;
  /*
   *  if there are cuts  ==>>  create 1D
   */
  if (mycutd!=NULL){
    mkmat1( det,  mycutd ); // create 1D
    t61d_d=(TH1F*)gDirectory->Get(  telename1D  );
    t61d_d->SetName( telenamed );
  }//cutd
  if (mycutp!=NULL){
    mkmat1( det,   mycutp ); // create 1D
    t61d_p=(TH1F*)gDirectory->Get(  telename1D  );
    t61d_p->SetName( telenamep );
  }//cutd
  // Run with a condition:
  // MAKE 1D   dE  ONLY SPECTRUM

  mkmat1( -det ,  "e<10" );  // and now see....  only dE plotted; reverse
  t61d_de=(TH1F*)gDirectory->Get(  telename1D  );
  t61d_de->SetName( telenamede );




  // SAVE .................................................
  TDirectory *dir=gDirectory;  
  TFile *f2=new TFile( fnamespe ,"UPDATE");   //  save to ...fnamespe...Tispectra.root
    t6bi->SetName( namespe );
    t6bi->Write();

    if (t61d_d!=NULL){t61d_d->Write();}
    if (t61d_p!=NULL){t61d_p->Write();}
    t61d_de->Write();

    f2->ls();
    printf("closing %s\n",  fnamespe );
  f2->Close();
  dir->cd();
  
  //  t->Show(2);
    printf("closing %s\n",  fname );
    f->Close();
  }//FILE root DOES EXIST
  
    
}//  batchmat
