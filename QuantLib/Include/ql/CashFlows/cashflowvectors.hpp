
/*
 * Copyright (C) 2000-2001 QuantLib Group
 *
 * This file is part of QuantLib.
 * QuantLib is a C++ open source library for financial quantitative
 * analysts and developers --- http://quantlib.sourceforge.net/
 *
 * QuantLib is free software and you are allowed to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of it under the conditions stated
 * in the QuantLib License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You should have received a copy of the license along with this file;
 * if not, contact ferdinando@ametrano.net
 * The license is also available at http://quantlib.sourceforge.net/LICENSE.TXT
 *
 * The members of the QuantLib Group are listed in the Authors.txt file, also
 * available at http://quantlib.sourceforge.net/Authors.txt
*/

/*! \file cashflowvectors.hpp
    \brief Cash flow vector builders

    $Id$
*/

//  $Source$
//  $Log$
//  Revision 1.6  2001/06/22 16:38:15  lballabio
//  Improved documentation
//
//  Revision 1.5  2001/06/19 10:25:18  lballabio
//  Disabled short/long floating coupons
//
//  Revision 1.4  2001/06/18 08:05:59  lballabio
//  Reworked indexes and floating rate coupon
//
//  Revision 1.3  2001/06/15 13:52:07  lballabio
//  Reworked indexes
//
//  Revision 1.2  2001/06/01 16:50:16  lballabio
//  Term structure on deposits and swaps
//
//  Revision 1.1  2001/05/31 08:56:40  lballabio
//  Cash flows, scheduler, and generic swap added - the latter should be specialized and tested
//

#ifndef quantlib_cash_flow_vectors_hpp
#define quantlib_cash_flow_vectors_hpp

#include "ql/cashflow.hpp"
#include "ql/rate.hpp"
#include "ql/calendar.hpp"
#include "ql/daycounter.hpp"
#include "ql/index.hpp"
#include "ql/termstructure.hpp"
#include "ql/handle.hpp"
#include <vector>

namespace QuantLib {

    namespace CashFlows {

        //! helper class building a sequence of fixed rate coupons
        class FixedRateCouponVector 
        : public std::vector<Handle<CashFlow> > {
          public:
            FixedRateCouponVector(
                const std::vector<double>& nominals, 
                const std::vector<Rate>& couponRates, 
                const Date& startDate, const Date& endDate, 
                int frequency, const Handle<Calendar>& calendar, 
                RollingConvention rollingConvention, bool isAdjusted, 
                const Handle<DayCounter>& dayCount, 
                const Date& stubDate = Date(), 
                const Handle<DayCounter>& firstPeriodDayCount = 
                    Handle<DayCounter>());
        };

        //! helper class building a sequence of floating rate coupons
        /*! \warning The passing of a non-null stub date - i.e., the creation 
            of a short/long first coupon - is currently disabled. 
            \todo A suitable algorithm should be implemented for the 
            calculation of the interpolated index fixing for a 
            short/long first coupon. 
        */
        class FloatingRateCouponVector 
        : public std::vector<Handle<CashFlow> > {
          public:
            FloatingRateCouponVector(
                const std::vector<double>& nominals, 
                const Date& startDate, const Date& endDate, 
                int frequency, const Handle<Calendar>& calendar, 
                RollingConvention rollingConvention, 
                const RelinkableHandle<TermStructure>& termStructure,
                const Handle<Index>& index = Handle<Index>(), 
                const std::vector<Spread>& spreads = std::vector<Spread>(), 
                const Date& stubDate = Date());
        };

    }

}


#endif
