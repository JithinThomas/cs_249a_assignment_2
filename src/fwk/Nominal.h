/**
 * Nominal and Ordinal template types for encapsulating simple values.
 */

#ifndef FWK_NOMINAL_H
#define FWK_NOMINAL_H

/**
 * Nominal only defines equality, inequality, assignment, and conversion
 * to and from the representation type.
 */
template <class UnitType, class RepType>
class Nominal {
public:

    Nominal(const RepType v) :
        value_(v)
    {
        // Nothing else to do.
    }

    Nominal(const Nominal& n) :
        value_(n.value_)
    {
        // Nothing else to do.
    }


    void operator =(const Nominal& v) {
        value_ = v.value_;
    }


    virtual bool operator ==(const Nominal& v) const {
        return value_ == v.value_;
    }
	
    virtual bool operator !=(const Nominal& v) const {
        return value_ != v.value_;
    }


    RepType value() const {
        return value_;
    }

protected:

    RepType value_;

};

#endif
