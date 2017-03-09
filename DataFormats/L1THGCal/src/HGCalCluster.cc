#include "DataFormats/L1THGCal/interface/HGCalCluster.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"

using namespace l1t;

HGCalCluster::HGCalCluster( const LorentzVector p4, 
                            int pt,
                            int eta,
                            int phi )
   : L1Candidate(p4, pt, eta, phi),
     centre_(0, 0, 0),
     mipPt_(0),
     seedMipPt_(0)
{

}


HGCalCluster::HGCalCluster(  const l1t::HGCalTriggerCell &tcSeed )
    : seedDetId_( tcSeed.detId() ),
      centre_(0, 0, 0),
      mipPt_(0),
      seedMipPt_(0)
{
    addTriggerCell( tcSeed );
}


HGCalCluster::~HGCalCluster()
{

}


void HGCalCluster::addTriggerCell(const l1t::HGCalTriggerCell &tc)
{

    if( triggercells_.size() == 0 ){ 
        seedDetId_ = tc.detId();
        seedMipPt_ = tc.mipPt();
    }

    /* update cluster positions */
    Basic3DVector<float> tcVector( tc.position().x(),
                                   tc.position().y(),
                                   tc.position().z() );

    Basic3DVector<float> centreVector( tc.position().x(), 
                                       tc.position().y(), 
                                       tc.position().z() );

    centreVector = centreVector*mipPt_ + tcVector*tc.mipPt();
    centreVector = centreVector / ( mipPt_ + tc.mipPt() ) ;

    GlobalPoint centreTmp((float)centreVector.x(), 
                          (float)centreVector.y(), 
                          (float)centreVector.z() );

    GlobalPoint centreProjTmp((float)centreVector.x() / centreVector.z(),  
                              (float)centreVector.y() / centreVector.z(), 
                              (float)centreVector.z() / centreVector.z() );

    centre_ = centreTmp;
    centreProj_ = centreProjTmp;

    /* update cluster energies */
    mipPt_ += tc.mipPt();

    int hwPt = 0;
    hwPt  += tc.hwPt();
    this->setHwPt(hwPt);

    math::PtEtaPhiMLorentzVector p4( ( this->p4() )  );
    p4 += tc.p4(); 
    this->setP4( p4 );

}
void HGCalCluster::addTriggerCellList( edm::Ptr<l1t::HGCalTriggerCell> &p)
{

    triggercells_.push_back( p );

}


double HGCalCluster::distance(const l1t::HGCalTriggerCell &tc) const
{

    GlobalPoint tcPointXYZ( tc.position().x(), 
                             tc.position().y(), 
                             tc.position().z() );
    
    return ( tcPointXYZ - centre_ ).mag();
   
}


uint32_t HGCalCluster::subdetId()  const
{

    HGCalDetId seedDetId( seedDetId_ );
    
    return seedDetId.subdetId();

}


uint32_t HGCalCluster::layer() const
{
    
    HGCalDetId seedDetId( seedDetId_ );    
    int layer = 52;
    if( seedDetId.subdetId()==3 ){
        layer =  seedDetId.layer();
    }
    else if( seedDetId.subdetId()==4 ){
        layer = seedDetId.layer() + 28;
    }
    return layer;
}


int32_t HGCalCluster::zside() const
{
    
    HGCalDetId seedDetId( seedDetId_ );    
    
    return seedDetId.zside();

}


bool HGCalCluster::operator<(const HGCalCluster& cl) const
{

    bool res = false;

    /* Prioratize high pT */
    if( mipPt() < cl.mipPt()) {
        res = true;
    }

    return res;

}

