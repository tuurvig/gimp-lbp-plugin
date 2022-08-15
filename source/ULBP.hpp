#ifndef ULBP_H
#define ULBP_H

#include "LBPAbstract.hpp"

class ULBP final : public LBPAbstract{
public:
    ULBP( int points );

    /**
     * Applies LBP algorithm and returns the result if the number is uniform
     * There must be at max two 01 10 transitions in number's binary notation
     * If its ununiform number the method returns blue colour
     */
    gint Run( guchar pixel, guchar * neighbours ) const override;
private:
};

#endif