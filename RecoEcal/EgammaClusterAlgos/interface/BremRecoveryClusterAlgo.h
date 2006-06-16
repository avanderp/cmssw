#ifndef RecoEcal_EgammaClusterAlgos_BremRecoveryClusterAlgo_h_
#define RecoEcal_EgammaClusterAlgos_BremRecoveryClusterAlgo_h_

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include <vector>


#ifndef VerbosityLevelEnum
#define VerbosityLevelEnum
enum VerbosityLevel { DEBUG = 0, WARNING = 1, INFO = 2, ERROR = 3 }; 
#endif

/*
  The BremRecoveryClusterAlgo class encapsulates the functionality needed
  to perform the SuperClustering.
  
  WARNING: This code assumes that the BasicClusters 
  from the event are sorted by energy
*/

class BremRecoveryClusterAlgo
{
 public:
  
  BremRecoveryClusterAlgo(double eb_sc_road_etasize = 0.06, // Search window in eta - Barrel
			  double eb_sc_road_phisize = 0.80, // Search window in phi - Barrel
			  double ec_sc_road_etasize = 0.14, // Search window in eta - Endcap
			  double ec_sc_road_phisize = 0.40, // Search window in eta - Endcap
			  double theSeedEnergyThreshold = 0.40,
			  VerbosityLevel the_verbosity = ERROR
			  )
    {
      eb_rdeta_ = eb_sc_road_etasize;
      eb_rdphi_ = eb_sc_road_phisize;
      ec_rdeta_ = ec_sc_road_etasize;
      ec_rdphi_ = ec_sc_road_phisize;
      seedEnergyThreshold = theSeedEnergyThreshold;
      verbosity = the_verbosity;
    }

  void setVerbosity(VerbosityLevel the_verbosity)
    {
      verbosity = the_verbosity;
    }
  
  // the method called from outside to do the SuperClustering - returns a vector of SCs:
  reco::SuperClusterCollection makeSuperClusters(reco::BasicClusterRefVector & clusters);
  
 private:
  
  // make superclusters out of clusters produced by the Island algorithm:
  void makeIslandSuperClusters(reco::BasicClusterRefVector &clusters_v, 
			       double etaRoad, double phiRoad);
  
  // make superclusters out of clusters produced by the Hybrid algorithm:
  void makeHybridSuperClusters(reco::BasicClusterRefVector &clusters_v);
  
  // return true if the cluster is within the search phi-eta window of the seed
  bool match(reco::BasicClusterRef seed_p, 
	     reco::BasicClusterRef cluster_p,
	     double etaRoad, double phiRoad);
  
  //

  VerbosityLevel verbosity;

  double eb_rdeta_;
  double eb_rdphi_;
  double ec_rdeta_;
  double ec_rdphi_;
  
  double seedEnergyThreshold;
  
  reco::SuperClusterCollection superclusters_v;
  
};

#endif
