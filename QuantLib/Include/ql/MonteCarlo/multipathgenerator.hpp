
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
/*! \file multipathgenerator.hpp
    \brief Generates a multi path from a random-point generator

    $Id$
*/

// $Source$
// $Log$
// Revision 1.7  2001/06/22 16:38:15  lballabio
// Improved documentation
//
// Revision 1.6  2001/05/25 09:29:40  nando
// smoothing #include xx.hpp and cutting old Log messages
//
// Revision 1.5  2001/05/24 15:38:08  nando
// smoothing #include xx.hpp and cutting old Log messages
//

#ifndef quantlib_montecarlo_multi_path_generator_h
#define quantlib_montecarlo_multi_path_generator_h

#include "ql/MonteCarlo/multipath.hpp"

namespace QuantLib {

    namespace MonteCarlo {

        //! Generates a multipath from a random number generator
        /*! MultiPathGenerator<RAG> is a class that returns a random multi path.
            RAG is a sample generator which returns an array, must have the
            minimal interface,
            \code
            RAG{
                RAG();
                RAG(Array &average, Matrix &covariance, long seed);
                Array next();
                double weight();
            };
            \endcode
        */
        template <class RAG>
        class MultiPathGenerator {
          public:
            // typedef MultiPath SampleType;
            // this typedef would make MultiPathGenerator into a sample 
            // generator
            MultiPathGenerator();
            MultiPathGenerator(int timeDimension,
                               const Math::Matrix &covariance,
                               const Array &average = Array(),
                               long seed=0);
            MultiPathGenerator(const std::vector<Time> &dates,
                               const Math::Matrix &covariance,
                               const Array &average = Array(),
                               long seed=0);
            MultiPath next() const;
            double weight() const{return weight_;}
        private:
            int numAssets_, timeDimension_;
            std::vector<Time> timeDelays_;
            mutable double weight_;
            Array average_;
            RAG rndArray_;
        };

        template <class RAG>
        inline MultiPathGenerator<RAG >::MultiPathGenerator():
                numAssets_(0), timeDimension_(0), weight_(0) {}

        template <class RAG>
        inline MultiPathGenerator<RAG >::MultiPathGenerator(
            int timeDimension,
            const Math::Matrix &covariance,
            const Array &average,
            long seed):
            timeDimension_(timeDimension),
            timeDelays_(timeDimension, 1.0),
            numAssets_(covariance.rows()),
            average_(covariance.rows(),0.0),
            rndArray_(covariance, seed){

            QL_REQUIRE(timeDimension_ > 0,
                "Time dimension("+
                DoubleFormatter::toString(timeDimension_)+
                ") too small");

            if(average.size() != 0){
                QL_REQUIRE(average.size() == average_.size(),
                           "MultiPathGenerator covariance and average "
                           "do not have the same size");
                std::copy(average.begin(),average.end(),average_.begin());
            }

        }

        template <class RAG>
        inline MultiPathGenerator<RAG >::MultiPathGenerator(
            const std::vector<Time> &dates,
            const Math::Matrix &covariance,
            const Array &average,
            long seed):
            timeDimension_(dates.size()),
            timeDelays_(dates.size()),
            numAssets_(covariance.rows()),
            average_(covariance.rows(), 0.0),
            rndArray_(covariance, seed){

            if(average.size() != 0){
                QL_REQUIRE(average.size() == average_.size(),
                           "MultiPathGenerator covariance and average "
                           "do not have the same size");
                std::copy(average.begin(),average.end(),average_.begin());
            }

            QL_REQUIRE(timeDimension_ > 0,
                "Time dimension("+
                IntegerFormatter::toString(timeDimension_)+
                ") too small");

             QL_REQUIRE(dates[0] >= 0,
                 "MultiPathGenerator: first date(" +
                 DoubleFormatter::toString(dates[0])+
                 ") must be positive");
            timeDelays_[0] = dates[0];

            if(timeDimension_ > 1){
                for(int i = 1; i < timeDimension_; i++){
                    QL_REQUIRE(dates[i] >= dates[i-1],
                        "MultiPathGenerator: date(" +
                        IntegerFormatter::toString(i-1)+")="+
                        DoubleFormatter::toString(dates[i-1])+
                        " is later than date("+
                        IntegerFormatter::toString(i)+")="+
                        DoubleFormatter::toString(dates[i]));
                    timeDelays_[i] = dates[i] - dates[i-1];
                }
            }
        }

        template <class RAG>
        inline MultiPath MultiPathGenerator<RAG >::next() const{

            QL_REQUIRE(numAssets_ > 0,
                "MultiPathGenerator: object declared but not initialized");
            MultiPath multiPath(numAssets_, timeDimension_);
            Array nextArray(numAssets_);
            weight_ = 1.0;
            for(int i = 0; i < timeDimension_; i++){
                nextArray = average_ * timeDelays_[i]
                            + rndArray_.next()* QL_SQRT(timeDelays_[i]);
                weight_ *= rndArray_.weight();
                std::copy(nextArray.begin(), nextArray.end(),
                          multiPath.column_begin(i));
            }
            return multiPath;
        }

    }

}

#endif
