#include <TTree.h>
#include <TFile.h>
#include <iomanip>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string>

#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/mman>
#include <fcntl.h>
#include <unistd.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <sys/mman.h>
//#include <fcntl.h>
//#include <unistd.h>



#define MAX_SEQ 1000

static FILE* fd_txt = NULL;
static int nch;
static int nevt_seq[MAX_SEQ];
static int nevt_tot;
static int nseq;
static float yscale[4];
static int npts_evt;
static size_t size;
static int16_t* adc_data;

int scope_data_init(char* asciiFile){
    int i;
    char line[1024];
    char dt1[1024];
    char dt0[1024];
    int  nrec;
    int  id_bin;
    struct stat buf;
    
    if (fd_txt != NULL)
        fclose(fd_txt);
    
    if (size > 0){
        assert(adc_data != NULL);
        munmap(adc_data, size);
        size = 0;
    }
    
    nch = 0;
    for(i=0; i<MAX_SEQ; i++)
        nevt_seq[i] = 0;
    nevt_tot = 0;
    dt0[0] = '\0';
    npts_evt = 0;
    
    fd_txt = fopen(asciiFile, "r");
    if (fd_txt == NULL){
        perror(asciiFile);
        return 1;
    }
        
    nrec = 0;
    while(fgets(line, sizeof(line), fd_txt)){
        int   nel, npts, nev;
        float yinc;
        nel = sscanf(line, "%*d,%*d,%d,%*d,%*f,%*lf,%*f,%f,%*f,%*f,%*d,%*f,%*f,%*f,%*f,%*[^,],%[^,],%*[^,],%*d,%*d,%*d,%*d,%*f,%*f,%d",
                     &npts, &yinc, dt1, &nev);
        if (npts_evt == 0) npts_evt = npts;
        assert(nel == 4 && npts > 0 && nev > 0 && npts == npts_evt);
        /* Figure out number of channels if not known */
        if (nch == 0){
            if (dt0[0] == '\0'){
                memcpy(dt0, dt1, 1024);
                nevt_seq[0] = nev;
            }
            if (strncmp(dt0, dt1, 1024) == 0){
                assert(nrec < 4);
                yscale[nrec] = yinc;
            }
            else
                nch = nrec;
        }
        else{
            assert(nrec < nch * MAX_SEQ);
            nevt_seq[nrec/nch] = nev;
        }
#ifdef DEBUG
        printf("%d %s %s %d\n", nel, dt0, dt1, nev);
#endif
        nrec++;
    }
    if (nch == 0)
        nch = nrec;
    nseq = nrec/nch;
    assert(nrec == nch * nseq);
    for(i=0; i<nseq; i++)
        nevt_tot += nevt_seq[i];
#ifdef DEBUG
    printf("nrec=%d nch=%d nseq=%d nevt=%d\n", nrec, nch, nseq, nevt_tot);
#endif
    
    /* prep to access real wfm data */
    {
        char *s;
        strncpy(line, asciiFile, sizeof(line));
        s = strstr(line, ".txt");
        assert(s != NULL);
        strncpy(s, ".dat", 4);
    }
    id_bin = open(line, O_RDONLY);
    if (id_bin < 0){
        perror(line);
        return 1;
    }
    if (fstat(id_bin, &buf) < 0){
        perror(line);
        return 1;
    }
    size = buf.st_size;
    assert(size == npts_evt * nch * nevt_tot * 2 + nevt_tot * 8);
    adc_data = (int16_t *)mmap(NULL, size, PROT_READ, MAP_SHARED, id_bin, 0);
    if (adc_data == NULL){
        perror("mmap()");
        return 1;
    }
    close(id_bin);
    return 0;
}

int getAdcData(int evtnum, int chnum, float* buf, int maxpts)
{
    int iseq;
    int offst;
    int i;
    int npts_filled;
    if (evtnum < 0 || evtnum >= nevt_tot || chnum < 0 || chnum >= nch)
        return -1;
    offst = 0;
    for(iseq=0; evtnum >= nevt_seq[iseq]; iseq++){
        offst += (npts_evt * nch + 4) * nevt_seq[iseq];
        evtnum -= nevt_seq[iseq];
    }
    offst += npts_evt * (chnum * nevt_seq[iseq] + evtnum);
    for(i=0; i<npts_evt && i<maxpts; i++)
        buf[i] = adc_data[offst+i] * yscale[chnum];
    npts_filled = i;
    return npts_filled;
}



//int scope_data_decode(int argc, char** argv){
    
int scope_data_decode_macro(char* fname){
    int i;
    int ier;
    int ns;
    float buf[4096];
    
    
//    if (argc < 2) return 1;
    
    ier = scope_data_init(fname);
    if (ier != 0) return 1;
    
    printf("ATTENTION - All 4 channels are always filled, if in reality nch < 4, value 1000. is filled");
    
    
    printf("#nch=%d nevt=%d\n", nch, nevt_tot);
    
    // create the ntuple

    TString fn = (TString)fname;

    // beautify the output name
    int dot = fn.First(".");
    TString filename;
    for (int ia = 0 ; ia < dot; ia++)
        filename+= fn[ia];
    
    filename+=".root";
    
    TFile ofile(filename.Data(),"RECREATE");
    //
    TTree *tree = new TTree("HGTDTime","Time part of the HGTD time measurement");
    
    int split = 0;
    int bsize = 64000;
    //    Event *event = 0;
    
    // loop over channels

   int nsamples  = getAdcData(1, 2, buf, sizeof(buf)/sizeof(float));
  //  int nsamples  = 2000;

    // with current oscilloscope, 4 is the maximal number of channels we can read out
    float chan1[nsamples] ;
    float chan2[nsamples] ;
    float chan3[nsamples] ;
    float chan4[nsamples] ;
    
    for (int ij = 0; ij < nsamples; ij++){
        chan1[ij] = 1000.;
        chan2[ij] = 1000.;
        chan3[ij] = 1000.;
        chan4[ij] = 1000.;
    }
    
    int ntrig = 0;
    int nsamp = 0;
    int evnr = 0;
 
    ostringstream ch1stream;
    ch1stream << "Channel1["<<nsamples<<"]/F";
    TString ch1string = ch1stream.str();
    
    ostringstream ch2stream;
    ch2stream << "Channel2["<<nsamples<<"]/F";
    TString ch2string = ch2stream.str();
  
    ostringstream ch3stream;
    ch3stream << "Channel3["<<nsamples<<"]/F";
    TString ch3string = ch3stream.str();
    
    ostringstream ch4stream;
    ch4stream << "Channel4["<<nsamples<<"]/F";
    TString ch4string = ch4stream.str();
    
    tree->Branch("Channel1",&chan1,ch1string.Data());
    tree->Branch("Channel2",&chan2,ch2string.Data());
    tree->Branch("Channel3",&chan3,ch3string.Data());
    tree->Branch("Channel4",&chan4,ch4string.Data());
    
    tree->Branch("ntrig",&ntrig,"ntrig/I");
    tree->Branch("nsamp",&nsamp,"nsamp/I");
    tree->Branch("evnr",&evnr,"evnr/I");
    
    
    
    ////
    //    std::vector<float> channels;
    //
    //    for (int ik = 0; ik < nch; ik++)
    //        channels.append()
    
    printf("Starting loop %d over events, number of samples per channel and event: %d \n",nevt_tot,nsamples);
    
  
    ntrig = nevt_tot;

    
    for (int i = 0; i < ntrig;i++){
    //for (int i = 0; i < 100;i++){
        
        if (i%1000 == 0)
            printf("Processing event number: %d \n",i);
        
        
        // initialize branch each event
        for (int ij = 0; ij < nsamples; ij++){
            chan1[ij] = 1000.;
            chan2[ij] = 1000.;
            chan3[ij] = 1000.;
            chan4[ij] = 1000.;
        }
      
        nsamp = nsamples;
        evnr = i;
        
        for (int j = 0; j < nch; j++){
            
            ns = getAdcData(i, j, buf, sizeof(buf)/sizeof(float)); // returns number of points per channel per event (default is 2000)
            
            // very lumberjack's method, I am sorry
            for(int ii=0; ii<ns; ii++){
                if (j == 0)
                    chan1[ii] = buf[ii];
                if (j == 1)
                    chan2[ii] = buf[ii];
                if (j == 2)
                    chan3[ii] = buf[ii];
                if (j == 3)
                    chan4[ii] = buf[ii];
            }
         }
        
        tree->Fill(); // fill every event
    }
    
    tree->Write();
    ofile.Close();
    
    return 0;
}
