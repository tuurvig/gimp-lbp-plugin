#ifndef LBP_H
#define LBP_H

#include "LBPAbstract.hpp"

class LBP final : public LBPAbstract{
public:
    LBP( int points );

    /**
     * Applies the basic LBP algorithm for the pixel and its neighbours
     */ 
    gint Run( guchar pixel, guchar * neighbours ) const override;
private:
};

#endif