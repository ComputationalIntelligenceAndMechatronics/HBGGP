/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/GP/MutationShrinkDepthSelectiveConstrainedOp.hpp
 *  \brief  Definition of the class GP::MutationShrinkDepthSelectiveConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2010/06/26 20:54:45 $
 */

#ifndef Beagle_GP_MutationShrinkDepthSelectiveConstrainedOp_hpp
#define Beagle_GP_MutationShrinkDepthSelectiveConstrainedOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/System.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/MutationShrinkConstrainedOp.hpp"
#include "DepthDependentSelectionOp.h"
namespace Beagle {
namespace GP {

/*!
 *  \class MutationShrinkDepthSelectiveConstrainedOp
 *    beagle/GP/MutationShrinkDepthSelectiveConstrainedOp.hpp "beagle/GP/MutationShrinkDepthSelectiveConstrainedOp.hpp"
 *  \brief Constrained GP tree shrink mutation operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationShrinkDepthSelectiveConstrainedOp : public MutationShrinkConstrainedOp, public DepthDependentSelectionOp {

public:

  //! GP::MutationShrinkDepthSelectiveConstrainedOp allocator type.
  typedef AllocatorT<MutationShrinkDepthSelectiveConstrainedOp,MutationShrinkConstrainedOp::Alloc>
          Alloc;
  //! GP::MutationShrinkDepthSelectiveConstrainedOp handle type.
  typedef PointerT<MutationShrinkDepthSelectiveConstrainedOp,MutationShrinkConstrainedOp::Handle>
          Handle;
  //! GP::MutationShrinkDepthSelectiveConstrainedOp bag type.
  typedef ContainerT<MutationShrinkDepthSelectiveConstrainedOp,MutationShrinkConstrainedOp::Bag>
          Bag;

  explicit MutationShrinkDepthSelectiveConstrainedOp(std::string inMutationPbName="gp.mutshrink.indpb",
                                       std::string inName="GP-MutationShrinkDepthSelectiveConstrainedOp");
  virtual ~MutationShrinkDepthSelectiveConstrainedOp() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

};

}
}

#endif // Beagle_GP_MutationShrinkConstrainedOp_hpp
