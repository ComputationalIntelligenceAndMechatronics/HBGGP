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
 *  \file   beagle/GP/MutationSwapDepthSelectiveConstrainedOp.hpp
 *  \brief  Definition of the class GP::MutationSwapDepthSelectiveConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2010/06/26 20:54:45 $
 */

#ifndef Beagle_GP_MutationSwapDepthSelectiveConstrainedOp_hpp
#define Beagle_GP_MutationSwapDepthSelectiveConstrainedOp_hpp

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
#include "beagle/GP/MutationSwapConstrainedOp.hpp"
#include "DepthDependentSelectionOp.h"
namespace Beagle {
namespace GP {

/*!
 *  \class MutationSwapDepthSelectiveConstrainedOp
 *    beagle/GP/MutationSwapDepthSelectiveConstrainedOp.hpp "beagle/GP/MutationSwapDepthSelectiveConstrainedOp.hpp"
 *  \brief Constrained GP tree swap mutation operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationSwapDepthSelectiveConstrainedOp : public MutationSwapConstrainedOp, public DepthDependentSelectionOp {

public:

  //! GP::MutationSwapDepthSelectiveConstrainedOp allocator type.
  typedef AllocatorT<MutationSwapDepthSelectiveConstrainedOp,MutationSwapConstrainedOp::Alloc>
          Alloc;
  //! GP::MutationSwapDepthSelectiveConstrainedOp handle type.
  typedef PointerT<MutationSwapDepthSelectiveConstrainedOp,MutationSwapConstrainedOp::Handle>
          Handle;
  //! GP::MutationSwapDepthSelectiveConstrainedOp bag type.
  typedef ContainerT<MutationSwapDepthSelectiveConstrainedOp,MutationSwapConstrainedOp::Bag>
          Bag;

  explicit MutationSwapDepthSelectiveConstrainedOp(std::string inMutationPbName="gp.mutswap.indpb",
                                     std::string inDistribPbName="gp.mutswap.distrpb",
                                     std::string inName="GP-MutationSwapDepthSelectiveConstrainedOp");
  virtual ~MutationSwapDepthSelectiveConstrainedOp() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

};

}
}

#endif // Beagle_GP_MutationSwapConstrainedOp_hpp
