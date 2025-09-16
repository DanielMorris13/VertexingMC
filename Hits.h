#ifndef HITS_H
#define HITS_H

#include "TObject.h"

class Hits : public TObject {
   
public:
    
    Hits();
    Hits(double z, double phi, int index);
    Hits(double z, double phi);
    
    virtual ~Hits();
    
    double getZ() const {return fZ;}
    double getPhi() const {return fPhi;}
    double getIndex() const {return fIndex;}
    
private:
  
    double fZ;
    double fPhi;
    int fIndex;
    
    ClassDef(Hits,1)
};

#endif
