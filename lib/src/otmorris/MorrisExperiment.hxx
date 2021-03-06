//                                               -*- C++ -*-
/**
 *  @file  MorrisExperiment.hxx
 *  @brief MorrisExperiment
 *
 *  Copyright 2005-2015 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *  @author: schueller
 */
#ifndef OTMORRIS_MORRISEXPERIMENT_HXX
#define OTMORRIS_MORRISEXPERIMENT_HXX

#include <openturns/TypedInterfaceObject.hxx>
#include <otmorris/MorrisExperimentImplementation.hxx>
#include "otmorris/OTMORRISprivate.hxx"

namespace OTMORRIS
{
/**
 * @class MorrisExperiment
 *
 * MorrisExperiment enables to build experiments for the Morris method
 */
class OTMORRIS_API MorrisExperiment
  : public OT::TypedInterfaceObject<MorrisExperimentImplementation>
{
  CLASSNAME;

public:
  typedef OT::Pointer<MorrisExperimentImplementation>  Implementation;

  /** Default constructor */
  MorrisExperiment();

  /** Default constructor */
  MorrisExperiment(const MorrisExperimentImplementation & implementation);

  /** Constructor from implementation */
  MorrisExperiment(const Implementation & p_implementation);

#ifndef SWIG
  /** Constructor from implementation pointer */
  MorrisExperiment(MorrisExperimentImplementation * p_implementation);
#endif

  /** Constructor using a p-level grid - Uniform(0,1)^d */
  MorrisExperiment(const OT::Indices & levels, const OT::UnsignedInteger N);

  /** Constructor using a p-level grid and intervals*/
  MorrisExperiment(const OT::Indices & levels, const OT::Interval & interval, const OT::UnsignedInteger N);

  /** Constructor using NumericalSample, which is supposed to be an LHS design -  - Uniform(0,1)^d*/
  MorrisExperiment(const OT::NumericalSample & lhsDesign, const OT::UnsignedInteger N);

  /** Constructor using NumericalSample, which is supposed to be an LHS design */
  MorrisExperiment(const OT::NumericalSample & lhsDesign, const OT::Interval & interval, const OT::UnsignedInteger N);

  /** Virtual constructor method */
  MorrisExperiment * clone() const;

  /** Generate method */
  OT::NumericalSample generate() const;

  /** String converter */
  OT::String __repr__() const;


}; /* class MorrisExperiment */

} /* namespace OTMORRIS */

#endif /* OTMORRIS_MORRISEXPERIMENT_HXX */
