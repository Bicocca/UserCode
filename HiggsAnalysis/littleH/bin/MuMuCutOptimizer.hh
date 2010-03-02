#ifndef MuMuCutOptimizer_h
#define MuMuCutOptimizer_h

#include <TVector3.h>
#include <TH1F.h>

#include "smallHBaseClass.h"
#include <vector>

class MuMuCutOptimizer : public smallHBaseClass{
public:
  
  MuMuCutOptimizer(TChain *tree=0);
  virtual ~MuMuCutOptimizer() {};
  void Loop();
  int theBestQQ() const;
  bool accept_glb_mu(const int mu_index) const;
  bool accept_trk_mu(const int mu_index) const;

private:

  int MIN_nhits_trk;
  float MAX_normchi2_trk;
  float MAX_normchi2_glb;
  int MIN_nhits_pixel;
  float MAX_d0_trk;
  float MAX_dz_trk;
  float weight;
  bool onlyTheBest;            
};
#endif
