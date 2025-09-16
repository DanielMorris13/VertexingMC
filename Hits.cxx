#include <Riostream.h>
#include "Hits.h"

ClassImp(Hits)

Hits::Hits(): TObject(),
    fZ(0.),
    fPhi(0.),
    fIndex(0)
    {
    }


Hits::Hits(double z, double phi, int index): TObject(),
    fZ(z),
    fPhi(phi),
    fIndex(index)
    {
    }


Hits::Hits(double z, double phi): TObject(),
    fZ(z),
    fPhi(phi),
    fIndex(0)
    {
    }


Hits::~Hits() {
    //cout<<"DESTRUCTOR\n";
}


