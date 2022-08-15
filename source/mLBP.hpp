#ifndef MLBP_H
#define MLBP_H

#include "LBPAbstract.hpp"

class mLBP final : public LBPAbstract{
public:
    mLBP( int points );

    /**
     * Compares every neighbour with a mean value
     */
    gint Run( guchar pixel, guchar * neighbours ) const override;
private:

    /**
     * Pixel plus every neighbour divided by number of neighbours + 1
     */ 
    gint CalculateMean( guchar pixel, guchar * neighbours ) const;
};

#endif