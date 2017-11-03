//HRS-R replay script
const char** PATHS;
const char* RAW_DATA_FORMAT;
const char* STD_REPLAY_OUTPUT_DIR;
const char* CUSTOM_REPLAY_OUTPUT_DIR;
const char* SUMMARY_PHYSICS_PATH;
const char* SUMMARY_PHYSICS_FORMAT;

using namespace std;

void replay_R(Int_t runnumber=0,Int_t defAll=0,Int_t defEvt=-1,Int_t fstEvt=0,Bool_t QuietRun = kFALSE){

  //==================================
  //  Input & Output Definition
  //==================================
  char* RNAME="%s/g2p_%d.root";
  char* ODEF="RHRS.odef";
  char* CUTS="RHRS.cuts";

  const char *pPATHS[]={
    ".",
    "raw",
    "/cache/mss/halla/g2p/raw",
    0
  };

  const char *pRAW_DATA_FORMAT="%s/g2p_%d.dat.%d";
  const char *pSTD_REPLAY_OUTPUT_DIR=gSystem->Getenv("REPLAY_OUT_PATH");
  const char *pCUSTOM_REPLAY_OUTPUT_DIR=gSystem->Getenv("REPLAY_OUT_PATH");
  TString pSUMMARY_PHYSICS_PATH=gSystem->Getenv("REPLAY_SUMMARY_PATH");
  pSUMMARY_PHYSICS_PATH.Append("/summaryphy_%d.log");
  const char *pSUMMARY_PHYSICS_FORMAT=pSUMMARY_PHYSICS_PATH.Data();

  PATHS=pPATHS;
  RAW_DATA_FORMAT=pRAW_DATA_FORMAT;
  STD_REPLAY_OUTPUT_DIR=pSTD_REPLAY_OUTPUT_DIR;
  CUSTOM_REPLAY_OUTPUT_DIR=pSTD_REPLAY_OUTPUT_DIR;
  SUMMARY_PHYSICS_FORMAT=pSTD_REPLAY_OUTPUT_DIR;

  //==================================
  //  Module switches
  //==================================
  Bool_t bHelicity=kFALSE; // This part goes to Chao's code
  Bool_t bBeam=kTRUE;
  Bool_t bScaler=kTRUE;
  Bool_t bPhysics=kTRUE; // Always calculate gold tracks

  //==================================
  //  R-HRS Detectors
  //==================================
  THaApparatus *HRSR = new THaHRS("R", "Right HRS");

  //add detectors that are not in the default config 
  HRSR->AddDetector( new THaVDC("vdc", "Vertical Drift Chamber") );
  HRSR->AddDetector( new THaCherenkov("cer", "Gas Cherenkov counter" ));
  HRSR->AddDetector( new THaShower("ps", "Pre-shower pion rej."));
  HRSR->AddDetector( new THaShower("sh", "Shower pion rej."));
  gHaApps->Add(HRSR);

  //==================================
  //  Scaler
  //==================================
  if(bScaler){
    THaScalerGroup* RightScalers = new THaScalerGroup("evright");
    gHaScalers->Add(RightScalers);
  }
  
  //==================================
  //  Decoder Data
  //==================================
  THaApparatus* dec = new THaDecData("DR","Misc. Decoder Data");
  gHaApps->Add( dec );
  
  //==================================
  //  Beam
  //==================================
  if(bBeam){
    THaIdealBeam *ib = new THaIdealBeam("ib", "Ideal beam");
    gHaApps->Add(ib);
    THaG2PUnRasteredBeam *Rurb = new THaG2PUnRasteredBeam("Rurb","Unrastered beam from R-HRS");
    gHaApps->Add(Rurb);
    THaG2PRasteredBeam *Rrb = new THaG2PRasteredBeam("Rrb","Unrastered beam from R-HRS");
    gHaApps->Add(Rrb);
    THaG2PHappex *happex=new THaG2PHappex("happex.R","happex");
    Rurb->AddDetector(happex);
    THaG2PClockDecode *clock=new THaG2PClockDecode("Rclk","clock info");
    Rurb->AddDetector(clock);
  }
  TString beamchoice = "ib";
    
  //==================================
  //  Physics
  //==================================
  if(bPhysics){
    gHaPhysics->Add( new THaGoldenTrack( "R.gold", "HRS-R Golden Track", "R" ));
  }
  
  //=====================================
  //  Set up Analyzer and replay data
  //=====================================
  ReplayCore(
	     runnumber,     //run #
	     defAll,        //-1=replay all;0=ask for a number
	     defEvt,        //defaut replay event num
	     RNAME,         //output file format
	     ODEF,          //output define
	     CUTS,          //empty cut define
	     bScaler,       //replay scalar
	     bHelicity,     //replay helicity
	     fstEvt,        //First Event To Replay
	     QuietRun       //whether ask user for inputs
	     );
}
 

