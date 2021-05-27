#ifndef Hit_h
#define Hit_h


 struct Hit
     {
       int   pmtid; 
       float hittime;
    Hit()
      : pmtid(0), hittime(0.0) {
    }
      bool operator < (const Hit rhs) const {
      return this->hittime<rhs.hittime;
        }
     };

#endif
