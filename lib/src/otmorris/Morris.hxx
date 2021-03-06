//                                               -*- C++ -*-
/**
 *  @brief Morris
 *
 *  Copyright 2005-2016 Airbus-EDF-IMACS-Phimeca
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
 */
#ifndef OTMORRIS_MORRIS_HXX
#define OTMORRIS_MORRIS_HXX

#include <openturns/TypedInterfaceObject.hxx>
#include <openturns/StorageManager.hxx>
#include <openturns/NumericalPoint.hxx>
#include <openturns/NumericalSample.hxx>
#include <openturns/NumericalMathFunction.hxx>
#include "otmorris/OTMORRISprivate.hxx"

namespace OTMORRIS
{

class OTMORRIS_API Morris
  : public OT::PersistentObject
{
  CLASSNAME;

public:

  /** Standard constructor with in/out designs */
  Morris(const OT::NumericalSample & inputSample, const OT::NumericalSample & outputSample);

  /** Standard constructor with levels definition, number of trajectories, model */
  Morris(const OT::Indices & levels, const OT::UnsignedInteger N, const OT::NumericalMathFunction & model);

  /** Standard constructor with levels definition, number of trajectories, model and interval */
  Morris(const OT::Indices & levels, const OT::UnsignedInteger N, const OT::NumericalMathFunction & model, const OT::Interval & interval);

  /** Standard constructor with input lhs, number of trajectories and model */
  Morris(const OT::NumericalSample & lhsDesign, const OT::UnsignedInteger N, const OT::NumericalMathFunction & model);

  /** Standard constructor with input lhs, number of trajectories, interval and model */
  Morris(const OT::NumericalSample & lhsDesign, const OT::UnsignedInteger N, const OT::NumericalMathFunction & model, const OT::Interval & interval);


  /** Virtual constructor method */
  Morris * clone() const;

  // Get Mean/Standard deviation
  OT::NumericalPoint getMeanAbsoluteElementaryEffects(const OT::UnsignedInteger outputMarginal = 0) const;
  OT::NumericalPoint getMeanElementaryEffects(const OT::UnsignedInteger outputMarginal = 0) const;
  OT::NumericalPoint getStandardDeviationElementaryEffects(const OT::UnsignedInteger outputMarginal = 0) const;

  /** String converter */
  OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(OT::Advocate & adv);

protected:
  /** Default constructor for save/load mechanism */
  Morris() {};
  friend class OT::Factory<Morris>;

  // Method that allocate and compute effects
  void computeEffects(const OT::UnsignedInteger N);

private:
  OT::NumericalSample inputSample_;
  OT::NumericalSample outputSample_;
  // Elementary effects ==> N x (p*q) sample
  OT::NumericalSample elementaryEffectsMean_;
  OT::NumericalSample elementaryEffectsStandardDeviation_;
  OT::NumericalSample absoluteElementaryEffectsMean_;

}; /* class Morris */

} /* namespace OTMORRIS */

#endif /* OTMORRIS_MORRIS_HXX */
