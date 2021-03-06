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
#include "otmorris/Morris.hxx"
#include <openturns/PersistentObjectFactory.hxx>
#include "otmorris/MorrisExperiment.hxx"

using namespace OT;

namespace OTMORRIS
{

CLASSNAMEINIT(Morris);

static const Factory<Morris> Factory_Morris;

/** Standard constructor */
Morris::Morris(const NumericalSample & inputSample, const NumericalSample & outputSample)
  : PersistentObject()
  , inputSample_(inputSample)
  , outputSample_(outputSample)
  , elementaryEffectsMean_()
  , elementaryEffectsStandardDeviation_()
  , absoluteElementaryEffectsMean_()
{
  const UnsignedInteger size(inputSample.getSize());
  if (outputSample.getSize() != size)
    throw InvalidArgumentException(HERE) << "In Morris::Morris, input & output samples should be of same size. Here, input sample's size=" << size
                                         << ", output sample's size=" << outputSample.getSize();
  if (size == 0)
    throw InvalidArgumentException(HERE) << "In Morris::Morris, samples should not be empty";
  // Check that number of trajectories is correct
  const UnsignedInteger inputDimension(inputSample.getDimension());
  const UnsignedInteger N(static_cast<UnsignedInteger>(size / (inputDimension + 1)));
  if (size != N * (inputDimension + 1))
    throw InvalidArgumentException(HERE) << "In Morris::Morris, sample size should be a multiple of " << inputDimension + 1;
  // Perform evaluation of elementary effects
  computeEffects(N);
}

/** Standard constructor with levels definition, number of trajectories, model */
Morris::Morris(const Indices & levels, const UnsignedInteger N, const NumericalMathFunction & model)
  : PersistentObject()
  , inputSample_()
  , outputSample_()
  , elementaryEffectsMean_()
  , elementaryEffectsStandardDeviation_()
  , absoluteElementaryEffectsMean_()
{
  // Run generation of experiments
  const MorrisExperiment experiment(levels, N);
  // Generate input design
  inputSample_ = experiment.generate();
  // Evaluation of output design
  outputSample_ = model(inputSample_);

  // Compute number of trajectories
  // We could remove one or several trajectories due to replicate
  const UnsignedInteger n(static_cast<UnsignedInteger>(inputSample_.getSize() / (inputSample_.getDimension() + 1)));

  // Perform evaluation of elementary effects
  computeEffects(n);
}

/** Standard constructor with levels definition, number of trajectories, model and interval */
Morris::Morris(const Indices & levels, const UnsignedInteger N, const NumericalMathFunction & model, const Interval & interval)
  : PersistentObject()
  , inputSample_()
  , outputSample_()
  , elementaryEffectsMean_()
  , elementaryEffectsStandardDeviation_()
  , absoluteElementaryEffectsMean_()
{
  // Run generation of experiments
  const MorrisExperiment experiment(levels, interval, N);
  // Generate input design
  inputSample_ = experiment.generate();
  // Evaluation of output design
  outputSample_ = model(inputSample_);

  // Compute number of trajectories
  // We could remove one or several trajectories due to replicate
  const UnsignedInteger n(static_cast<UnsignedInteger>(inputSample_.getSize() / (inputSample_.getDimension() + 1)));

  // Perform evaluation of elementary effects
  computeEffects(n);
}


/** Standard constructor */
Morris::Morris(const NumericalSample & lhsDesign, UnsignedInteger N, const NumericalMathFunction & model)
  : PersistentObject()
  , inputSample_()
  , outputSample_()
  , elementaryEffectsMean_()
  , elementaryEffectsStandardDeviation_()
  , absoluteElementaryEffectsMean_()
{
  const UnsignedInteger size(lhsDesign.getSize());
  if (size == 0)
    throw InvalidArgumentException(HERE) << "In Morris::Morris, samples should not be empty";

  // Check coherancy between model and input sample
  const UnsignedInteger inputDimension(lhsDesign.getDimension());
  if (model.getInputDimension() != inputDimension)
    throw InvalidArgumentException(HERE) << "In Morris::Morris, model should have the same input dimension as sample. Here, input sample's dimension=" << inputDimension
                                         << ", model's input dimension=" << model.getInputDimension();

  // Run generation of experiments
  const MorrisExperiment experiment(lhsDesign, N);
  // Generate input design
  inputSample_ = experiment.generate();
  // Evaluation of output design
  outputSample_ = model(inputSample_);

  // Compute number of trajectories
  // We could remove one or several trajectories due to replicate
  const UnsignedInteger n(static_cast<UnsignedInteger>(inputSample_.getSize() / (inputDimension + 1)));

  // Perform evaluation of elementary effects
  computeEffects(n);
}

Morris::Morris(const NumericalSample & lhsDesign, const UnsignedInteger N, const NumericalMathFunction & model, const Interval & interval)
  : PersistentObject()
  , inputSample_()
  , outputSample_()
  , elementaryEffectsMean_()
  , elementaryEffectsStandardDeviation_()
  , absoluteElementaryEffectsMean_()
{
  const UnsignedInteger size(lhsDesign.getSize());
  if (size == 0)
    throw InvalidArgumentException(HERE) << "In Morris::Morris, samples should not be empty";

  // Check coherancy between model and input sample
  const UnsignedInteger inputDimension(lhsDesign.getDimension());
  if (model.getInputDimension() != inputDimension)
    throw InvalidArgumentException(HERE) << "In Morris::Morris, model should have the same input dimension as sample. Here, input sample's dimension=" << inputDimension
                                         << ", model's input dimension=" << model.getInputDimension();

  // Run generation of experiments
  const MorrisExperiment experiment(lhsDesign, interval, N);
  // Generate input design
  inputSample_ = experiment.generate();
  // Evaluation of output design
  outputSample_ = model(inputSample_);

  // Compute number of trajectories
  // We could remove one or several trajectories due to replicate
  const UnsignedInteger n(static_cast<UnsignedInteger>(inputSample_.getSize() / (inputDimension + 1)));

  // Perform evaluation of elementary effects
  computeEffects(n);
}

// Method that allocate and compute effects
void Morris::computeEffects(const UnsignedInteger N)
{
  // Allocate samples
  const UnsignedInteger inputDimension(inputSample_.getDimension());
  const UnsignedInteger outputDimension(outputSample_.getDimension());
  NumericalSample elementaryEffects(N, inputDimension * outputDimension);
  NumericalSample absoluteElementaryEffects(N, inputDimension * outputDimension);
  SquareMatrix dx(inputDimension, inputDimension);
  Matrix dy(inputDimension, outputDimension);
  Matrix ee;
  // Perform evaluation of elementary effects
  // Requires k system solves
  UnsignedInteger blockIndex(0);
  for (UnsignedInteger k = 0; k < N; ++k)
  {
    // Indices of current trajectory are k * (inputDimension+1) to (k+1)* (inputDimension+1)
    // The objective is to evaluate some finite differencies
    for (UnsignedInteger i = 0; i < inputDimension; ++i)
    {
      // Evaluate dx
      for (UnsignedInteger j = 0; j < inputDimension; ++j)
        dx(i, j) = inputSample_[blockIndex + i + 1][j] - inputSample_[blockIndex + i][j];
      // Evaluate dy
      for (UnsignedInteger j = 0; j < outputDimension; ++j)
        dy(i, j) = outputSample_[blockIndex + i + 1][j] - outputSample_[blockIndex + i][j];
    }
    // Solve linear system
    ee = dx.solveLinearSystem(dy);
    // Stores the elementary effects
    elementaryEffects[k] = NumericalPoint(*ee.getImplementation());
    absoluteElementaryEffects[k] = NumericalPoint(*ee.getImplementation());
    for (UnsignedInteger j = 0; j < inputDimension * outputDimension; ++j)
      absoluteElementaryEffects[k][j] = std::abs(absoluteElementaryEffects[k][j]);
    blockIndex += inputDimension + 1;
  } // end for k
  // Allocate ee mean/std support
  elementaryEffectsMean_ = NumericalSample(outputDimension, inputDimension);
  absoluteElementaryEffectsMean_ = NumericalSample(outputDimension, inputDimension);
  elementaryEffectsStandardDeviation_ = NumericalSample(outputDimension, inputDimension);
  // Evaluate mean/std
  // elementary effects compute mean
  elementaryEffectsMean_.getImplementation()->setData(elementaryEffects.computeMean());
  absoluteElementaryEffectsMean_.getImplementation()->setData(absoluteElementaryEffects.computeMean());
  elementaryEffectsStandardDeviation_.getImplementation()->setData(elementaryEffects.computeStandardDeviationPerComponent());
}

/* Virtual constructor method */
Morris * Morris::clone() const
{
  return new Morris(*this);
}

/* Mean effects */
NumericalPoint Morris::getMeanAbsoluteElementaryEffects(const UnsignedInteger marginal) const
{
  return absoluteElementaryEffectsMean_[marginal];
}

/* Mean effects */
NumericalPoint Morris::getMeanElementaryEffects(const UnsignedInteger marginal) const
{
  return elementaryEffectsMean_[marginal];
}

/* Standard deviation effects */
NumericalPoint Morris::getStandardDeviationElementaryEffects(const UnsignedInteger marginal) const
{
  return elementaryEffectsStandardDeviation_[marginal];
}

/* String converter */
String Morris::__repr__() const
{
  OSS oss;
  oss << "class=" << Morris::GetClassName()
      << ", input sample=" << inputSample_
      << ", output sample=" << outputSample_
      << ", ee mean= " << elementaryEffectsMean_
      << ", absolute ee mean= " << absoluteElementaryEffectsMean_
      << ", ee std= " << elementaryEffectsStandardDeviation_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void Morris::save(Advocate & adv) const
{
  PersistentObject::save( adv );
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
  adv.saveAttribute( "elementaryEffectsMean_", elementaryEffectsMean_ );
  adv.saveAttribute( "elementaryEffectsStandardDeviation_", elementaryEffectsStandardDeviation_ );
  adv.saveAttribute( "absoluteElementaryEffectsMean_", absoluteElementaryEffectsMean_ );
}

/* Method load() reloads the object from the StorageManager */
void Morris::load(Advocate & adv)
{
  PersistentObject::load( adv );
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "outputSample_", outputSample_ );
  adv.loadAttribute( "elementaryEffectsMean_", elementaryEffectsMean_ );
  adv.loadAttribute( "elementaryEffectsStandardDeviation_", elementaryEffectsStandardDeviation_ );
  adv.loadAttribute( "absoluteElementaryEffectsMean_", absoluteElementaryEffectsMean_ );
}


} /* namespace OTMORRIS */
