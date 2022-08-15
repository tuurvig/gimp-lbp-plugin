#ifndef LBP_ABSTRACT
#define LBP_ABSTRACT

#include <libgimp/gimp.h>

class LBPAbstract{
public:
    LBPAbstract( gint points );
    
    /**
     * Method that takes in pixel and an array of its neighbours
     */
    virtual gint Run( guchar pixel, guchar * neighbours ) const = 0;
    virtual ~LBPAbstract() = default;
protected:
    gint m_Points;
};

#endif