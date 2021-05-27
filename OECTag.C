struct Decision {
  enum OECTag tag;

  // Strategies to deal with the waveforms:
  short code = 0;
  // 1000 0000: save full waveform
  // 0100 0000: save waveforms according to the preScaleFactor
  // 0010 0000: save partial waveforms with zero suppression, 
  //            single channel threshold is set by zeroSuppressionThreshold
  // 0001 0000: save waveforms with reduced frequency, e.g.,
  //            only save the ADC value every N sampling points.
  //            N is set by samplingFrequencyReduction

  // This flag decides if the zero suppression is applied 
  // to the current event
  double zeroSuppressionThreshold = 0.1; //p.e

  // This pre-scale factor is the probability to save
  // the waveforms of the current event
  double preScaleFactor = 0.01;

  // Reduce the sampling frequency of the waveforms
  // keep the sampled ADC every N points, N = 2, 3, 4...
  int samplingFrequencyReduction = 1;
};

Decision tables[] {

  {IBD, 0x10, 0, 0, 1},
  {...},
};

enum OECTag : int {
// ---------------------------------------------------------------------
// the tag value has 32 bits: 32 or 16 bits
//
// 0000 0000 | 0000 0000 | 0000 0000 0000 0000
//    ID_A        ID_B            ID_C 
// 
// ID_A (0-7 bits)  : Detector ID
//             0x10 : CD
//             0x11 : CD water buffer
//             0x20 : WP
//             0x40 : TT
//
// ID_B (8-15 bits) : Top level categories of physics events 
//             0x10 : muon
//             0x20 : muon spallations, neutron, michel elec, etc
//             0x40 : not correlated with a muon
muCD,
muCW, //CD water buffer
muWP, 
muTT,
//
// ID_C (16-31 bits): Sub-level categories of physics events 
//
// ------------ ID_A=0x10, ID_B=0x10 (CD muons) ------------------------
// ID_C = 0x1000: normal CD muon. Criteria: Evis>0.1 GeV
muCD,
// ID_C = 0x2000: low energy CD muon. Criteria:
//           Muons go through CD water or veto water, 5%
//           Evis<=0.1 GeV, but correlated with IWP/OWP/TT tagged muons
muCDLE, 
// ID_C = 0x3000: Shower Muon. Criteria:
//           Evis>10 GeV, 14% of the muons, 16 MB/s
//           Evis>10 GeV, 6% of the muons, 7 MB/s
muShower, 
// ID_C = 0x4000: Stop Muon. Criteria:
//           deposit energy of stop muons (4%) is still large, 0-7 GeV
muStop, 
//
// Tracks (8-15 bits)
// ID_C = 0x100: Single track: rec. sucessfully single online  
muSglTrack,
//
// ID_C = 0x200: Double track: rec. sucessfully double online
muDblTrack,
//
// ID_C = 0x400: Shower track: rec. sucessfully shower online 
//               Evis>10 GeV
muShrTrack,
//
// ID_C = 0x800: Stop         
muStpTrack,
//
// ID_C = 0xF00: No track: can not be reconstructed online
muNoTrack,

// ---------------------------------------------------------------------
// ------------ ID_A=0x10, ID_B=0x20 (CD Muon spallations) -------------
// ID_C = 0x1000: Spallation Neutrons. Criteria:
//             1.8 MeV < E < 2.6 MeV, all t2Mu[]<1.5 ms
spn,
// ID_C = 0x2000: Michel Electrons. Criteria:
//             E < 100 MeV, all t2Mu[]<20 microSec, correlated with IWP/OWP/TT tagged muons
michelElec,
//
// ID_C = 0x4000: Low Energy CD events, but no WP and TT trigger
//
externalSpall,
//
// Tracks (8-15 bits) 
// ID_C = 0x100:   External track: going through CD water buffer 
externalTrack,

// ---------------------------------------------------------------------
// ---- ID_A=0x10, ID_B=0x40 (CD events not correlated with muon) ------
// ID_C = 0x100x:  Isolated event 
singles:
// ID_C = 0x1001:  Isolated event, energy < 0.7 MeV
// Low energy isolated event
// Internal and external radioactivity, C14+dark noise, etc. 
// No influence to primary analysis
// Store Q-T pair
singles_LE,
// ID_C = 0x1002:  Isolated event, 0.7 MeV < energy < 2 MeV 
// Medium energy isolated events
// Internal and external radioactivity, C11, etc
// Contribute to accidental background of IBD, need spectra 
// Save waveform of fired PMT for 1% events
// Save Q-T pair for the other 99%
singles_ME,
// ID_C = 0x1003:  Isolated event, 2 MeV < energy < 10 MeV, 
// High energy isolated events
// Long life cosmogenic isotopes, neutron capture from unknown sources, Tl208 decays, etc.
// Crucial for several analyses
// Waveform ?
singles_HE,
// ID_C = 0x1004: Isolated event, energy > 10 MeV
// Extremely high energy isolated events, very interesting
// Atmospheric neutrino, hep solar neutrino, supernova neutrino, etc.
// Be11, C9, etc.
// Waveform of fired PMTs
singles_XE,
// ------------ Coincidence Types --------
// A set of coincident events is tagged if they are close 
// in time and space.
// deltaT[]: time interval between every two nearby events
// deltaR[]: distance between every two nearby events
// The events that satisfy (deltaT<2 ms and deltaR<3 m) are 
// chained together, and defined as coincidences.
// If(deltaT>2 ms or deltaR>3 m), the event is categorized 
// as an isolated event, namely the 'Singles'.

// t2Mu[]: time to the nearest muon (CD, IWP, OWP, TT) for
// each event in the chain. Only if all t2Mu[]<1.5 ms, 
// the chained events are considered to be vetoed by a muon.

// ID_C = 0x2001:  proton decay candidates
// Prompt: Kaon+muon, 100 MeV < E < 1 GeV.
// Delay: Michel electron, 2-60 MeV
// 100 MeV < E < 1 GeV, all t2Mu[]>1.5 ms
// No WP/TT correlated trigger
// Store all waveforms
correlated_protondecay,
// ID_C = 0x2002: Bi-Po candidates from radioactivity decays
// prompt 0.x to 4 MeV
// delay: 0.x to 1.5 MeV
correlated_bipo,
// ID_C = 0x2003: IBD candidates
// prompt 0.7 to 12 MeV
// delay 1.5 to 6 MeV? nH and nC
correlated_ibd,
// ID_C = 0x2004: other coincidences
// prompt and delayed energy fall into the other ranges
// solar/atmospheric neutrino CC channel, etc.
correlated_others,

// ------------ Calibration events --------------
// Tag calibration source events by vertex cut around the source position
// ID_C = 0x3000:
calibration
};
