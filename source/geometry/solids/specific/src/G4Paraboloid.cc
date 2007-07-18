//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4Paraboloid.cc,v 1.1 2007-07-18 13:45:51 tnikitin Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4Paraboloid
//
// Implementation for G4Paraboloid class
//
// History:
//
// --------------------------------------------------------------------

#include "globals.hh"

#include "G4Paraboloid.hh"

#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"

#include "meshdefs.hh"

#include "Randomize.hh"

#include "G4VGraphicsScene.hh"
#include "G4Polyhedron.hh"
#include "G4NURBS.hh"
#include "G4NURBSbox.hh"
#include "G4VisExtent.hh"

using namespace CLHEP;

///////////////////////////////////////////////////////////////////////////////
//
// constructor - check parameters

G4Paraboloid::G4Paraboloid(const G4String& pName,
                               G4double pDz,
                               G4double pR1,
                               G4double pR2)
  : G4VSolid(pName), fCubicVolume(0.)
{
  if(pDz > 0. && pR2 > pR1 && pR1 >= 0.)
  {
    r1 = pR1;
    r2 = pR2;
    dz = pDz;
  }
  else
  {
    G4cerr << "Error - G4Paraboloid::G4Paraboloid(): " << GetName() << G4endl
           << "Z half-length must be larger than zero! " << G4endl;
    G4Exception("G4Paraboloid::G4Paraboloid()", "InvalidSetup", 
                FatalException, "Invalid dimensions. Negative Input Values or R1>=R2.");
  }

// r1^2 = k1 * (-dz) + k2
// r2^2 = k1 * ( dz) + k2
// => r1^2 + r2^2 = k2 + k2 => k2 = (r2^2 + r1^2) / 2
// and r2^2 - r1^2 = k1 * dz - k1 * (-dz) => k1 = (r2^2 - r1^2) / 2 / dz

  k1 = (r2 * r2 - r1 * r1) / 2 / dz;
  k2 = (r2 * r2 + r1 * r1) / 2;

//  GetSurfaceArea();
  fSurfaceArea = 0.;
}

///////////////////////////////////////////////////////////////////////////////
//
// Fake default constructor - sets only member data and allocates memory
//                            for usage restricted to object persistency.
//
G4Paraboloid::G4Paraboloid( __void__& a )
  : G4VSolid(a), fpPolyhedron(0), fCubicVolume(0.)
{
  fSurfaceArea = 0.;
}

///////////////////////////////////////////////////////////////////////////////
//
// Destructor

G4Paraboloid::~G4Paraboloid()
{
}

/////////////////////////////////////////////////////////////////////////
//
// Dispatch to parameterisation for replication mechanism dimension
// computation & modification.

//void ComputeDimensions(       G4VPVParamerisation p,
//                        const G4Int               n,
//                        const G4VPhysicalVolume*  pRep )
//{
//  p->ComputeDimensions(*this,n,pRep) ;
//}


///////////////////////////////////////////////////////////////////////////////
//
// Calculate extent under transform and specified limit

G4bool
G4Paraboloid::CalculateExtent(const EAxis pAxis,
                             const G4VoxelLimits& pVoxelLimit,
                             const G4AffineTransform& pTransform,
                                   G4double& pMin, G4double& pMax) const
{
  G4double xMin = -r2 + pTransform.NetTranslation().x(), xMax = r2 + pTransform.NetTranslation().x(),
           yMin = -r2 + pTransform.NetTranslation().y(), yMax = r2 + pTransform.NetTranslation().y(),
           zMin = -dz + pTransform.NetTranslation().z(), zMax = dz + pTransform.NetTranslation().z();
  if(!pTransform.IsRotated() || pTransform.NetRotation()(G4ThreeVector(0, 0, 1)) == G4ThreeVector(0, 0, 1))
  {
    if(pVoxelLimit.IsXLimited())
    {
      if(pVoxelLimit.GetMaxXExtent() < xMin - 0.5 * kCarTolerance || pVoxelLimit.GetMinXExtent() > xMax + 0.5 * kCarTolerance)
      {
        return false;
      }
      else
      {
        if(pVoxelLimit.GetMinXExtent() > xMin)
        {
          xMin = pVoxelLimit.GetMinXExtent();
        }
        if(pVoxelLimit.GetMaxXExtent() < xMax)
        {
          xMax = pVoxelLimit.GetMaxXExtent();
        }
      }
    }
    if(pVoxelLimit.IsYLimited())
    {
      if(pVoxelLimit.GetMaxYExtent() < yMin - 0.5 * kCarTolerance || pVoxelLimit.GetMinYExtent() > yMax + 0.5 * kCarTolerance)
      {
        return false;
      }
      else
      {
        if(pVoxelLimit.GetMinYExtent() > yMin)
        {
          yMin = pVoxelLimit.GetMinYExtent();
        }
        if(pVoxelLimit.GetMaxYExtent() < yMax)
        {
          yMax = pVoxelLimit.GetMaxYExtent();
        }
      }
    }
    if(pVoxelLimit.IsZLimited())
    {
      if(pVoxelLimit.GetMaxZExtent() < zMin - 0.5 * kCarTolerance || pVoxelLimit.GetMinZExtent() > zMax + 0.5 * kCarTolerance)
      {
        return false;
      }
      else
      {
        if(pVoxelLimit.GetMinZExtent() > zMin)
        {
          zMin = pVoxelLimit.GetMinZExtent();
        }
        if(pVoxelLimit.GetMaxZExtent() < zMax)
        {
          zMax = pVoxelLimit.GetMaxZExtent();
        }
      }
    }
    switch(pAxis)
    {
      case kXAxis:
        pMin = xMin;
        pMax = xMax;
        break;
      case kYAxis:
        pMin = yMin;
        pMax = yMax;
        break;
      case kZAxis:
        pMin = zMin;
        pMax = zMax;
        break;
      default:
        pMin = 0;
        pMax = 0;
        return false;
    }
  }
  else
  {
    G4bool existsAfterClip=true;

    // Calculate rotated vertex coordinates

    G4int noPolygonVertices=0;
    G4ThreeVectorList* vertices = CreateRotatedVertices(pTransform,noPolygonVertices);

    if(pAxis == kXAxis || pAxis == kYAxis || pAxis == kZAxis)
    {
      pMin =  kInfinity;
      pMax = -kInfinity;

      for(G4ThreeVectorList::iterator it = vertices->begin(); it < vertices->end(); it++)
      {
        if(pMin > (*it)[pAxis]) pMin = (*it)[pAxis];
        if((*it)[pAxis] < pVoxelLimit.GetMinExtent(pAxis)) pMin = pVoxelLimit.GetMinExtent(pAxis);
        if(pMax < (*it)[pAxis]) pMax = (*it)[pAxis];
        if((*it)[pAxis] > pVoxelLimit.GetMaxExtent(pAxis)) pMax = pVoxelLimit.GetMaxExtent(pAxis);
      }

      if(pMin > pVoxelLimit.GetMaxExtent(pAxis) || pMax < pVoxelLimit.GetMinExtent(pAxis)) existsAfterClip = false;
    }
    else
    {
      pMin = 0;
      pMax = 0;
      existsAfterClip = false;
    }
    delete vertices;
    return existsAfterClip;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Return whether point inside/outside/on surface

EInside G4Paraboloid::Inside(const G4ThreeVector& p) const
{
  // First check is  the point is above or below the solid.
  if(std::fabs(p.z()) > dz + 0.5 * kCarTolerance) return kOutside;

  G4double rho2 = p.perp2(),
           rhoSurfTimesTol2  = (k1 * p.z() + k2) * sqr(kCarTolerance),
           A = rho2 - ((k1 *p.z() + k2) + 0.25 * kCarTolerance * kCarTolerance);

  if(A < 0 && sqr(A) > rhoSurfTimesTol2) // Actually checking rho < radius of paraboloid at z = p.z().
  // We're either inside or in lower/upper cutoff area.
    if(std::fabs(p.z()) > dz - 0.5 * kCarTolerance)
      // We're in the upper/lower cutoff area, sides have a paraboloid shape
      // maybe further checks should be made to make these nicer
      return kSurface;
    else
      return kInside;
  else if(A <= 0 || sqr(A) < rhoSurfTimesTol2)
  // We're in the parabolic surface.
    return kSurface;
  else
    return kOutside;
}

///////////////////////////////////////////////////////////////////////////////
//

G4ThreeVector G4Paraboloid::SurfaceNormal( const G4ThreeVector& p) const
{
  G4ThreeVector n(0, 0, 0);
  if(std::fabs(p.z()) > dz + 0.5 * kCarTolerance)
  // If above or below just return normal vector for the cutoff plane.
    n = G4ThreeVector(0, 0, p.z()/std::fabs(p.z()));
  else if(std::fabs(p.z()) > dz - 0.5 * kCarTolerance)
  // This means we're somewhere in the plane z = dz or z = -dz. (As far as the program is concerned anyway.
  {
    if(p.z() < 0) // Are we in upper or lower plane?
    {
      if(p.perp2() > sqr(r1 + 0.5 * kCarTolerance))
      {
        n = G4ThreeVector(p.x(), p.y(), -k1 / 2).unit();
      }
      else if(r1 < 0.5 * kCarTolerance || p.perp2() > sqr(r1 - 0.5 * kCarTolerance))
      {
        n = G4ThreeVector(p.x(), p.y(), 0.).unit() + G4ThreeVector(0., 0., -1.).unit();
        n = n.unit();
      }
      else
      {
        n = G4ThreeVector(0., 0., -1.);
      }
    }
    else
    {
      if(p.perp2() > sqr(r2 + 0.5 * kCarTolerance))
      {
        n = G4ThreeVector(p.x(), p.y(), 0.).unit();
      }
      else if(r2 < 0.5 * kCarTolerance || p.perp2() > sqr(r2 - 0.5 * kCarTolerance))
      {
        n = G4ThreeVector(p.x(), p.y(), 0.).unit() + G4ThreeVector(0., 0., 1.).unit();
        n = n.unit();
      }
      else
      {
        n = G4ThreeVector(0., 0., 1.);
      }
    }
  }
  else
  {
    G4double rho2 = p.perp2(),
             rhoSurfTimesTol2  = (k1 * p.z() + k2) * sqr(kCarTolerance),
             A = rho2 - ((k1 *p.z() + k2) + 0.25 * kCarTolerance * kCarTolerance);

    if(A < 0 && sqr(A) > rhoSurfTimesTol2) // Actually checking rho < radius of paraboloid at z = p.z().
    // We're inside.
    {
      if(p.mag2() != 0) n = p.unit();
      
    }
    else if(A <= 0 || sqr(A) < rhoSurfTimesTol2)
    // We're in the parabolic surface.
    {
      n = G4ThreeVector(p.x(), p.y(), - k1 / 2).unit();
    }
    else
    {
      n = G4ThreeVector(p.x(), p.y(), - k1 / 2).unit();
    }
  }

  if(n.mag2() == 0)
  {
    G4Exception("G4Paraboloid::SurfaceNormal(p)", "Notification",
                JustWarning, "No normal defined for this point p.");
    G4cerr << "p = " << 1 / mm * p << " mm" << G4endl;
    G4cerr << *this;
  }
  return n;
}

///////////////////////////////////////////////////////////////////////////////
//
// Calculate distance to shape from outside, along normalised vector
// - return kInfinity if no intersection
//

G4double G4Paraboloid::DistanceToIn( const G4ThreeVector& p,
                                    const G4ThreeVector& v  ) const
{
  G4double rho2 = p.perp2(), paraRho2 = std::fabs(k1 * p.z() + k2);

  if(r2 && p.z() > - kCarTolerance * 0.5 + dz) 
  // If the points is above check for intersection with upper edge.
  {
    if(v.z() < 0)
    {
      G4double intersection = (dz - p.z()) / v.z(); // With plane z = dz.
      if(sqr(p.x() + v.x()*intersection)
        + sqr(p.y() + v.y()*intersection) < sqr(r2 + 0.5 * kCarTolerance))
      {
        if(p.z() < 0.5 * kCarTolerance + dz)
          return 0;
        else
          return intersection;
      }
    }
    else // Direction away, no posibility of intersection
      return kInfinity;
  }
  else if(r1 && p.z() < kCarTolerance * 0.5 - dz)
  // If the points is belove check for intersection with lower edge.
  {
    if(v.z() > 0)
    {
      G4double intersection = (-dz - p.z()) / v.z(); // With plane z = -dz.
      if(sqr(p.x() + v.x()*intersection)
        + sqr(p.y() + v.y()*intersection) < sqr(r1 + 0.5 * kCarTolerance))
      {
        if(p.z() > - 0.5 * kCarTolerance - dz)
          return 0;
        else
        {
          return intersection;
        }
      }
    }
    else// Direction away, no posibility of intersection
      return kInfinity;
  }

  G4double A = k1 / 2 * v.z() - p.x() * v.x() - p.y() * v.y(),
           vRho2 = v.perp2(), intersection,
           B = (k1 * p.z() + k2 - rho2) * vRho2;

  if(rho2 > paraRho2 && sqr(rho2 - paraRho2 - 0.25 * kCarTolerance * kCarTolerance) > kCarTolerance * kCarTolerance * paraRho2 || p.z() < - dz + kCarTolerance || p.z() > dz - kCarTolerance) // Make sure it's safely outside.
  // Is there a problem with squaring rho twice?
  {
    if(!vRho2) // Needs to be treated seperately.
    {
      intersection = ((rho2 - k2)/k1 - p.z())/v.z();
      if(intersection < 0) return kInfinity;
      else if(std::fabs(p.z() + v.z() * intersection) <= dz) return intersection;
      else
      {
        return kInfinity;
      }
    }
    else if(A*A + B < 0) // No real intersections.
    {
      return kInfinity;
    }
    else
    {
      intersection = (A - std::sqrt(B + sqr(A))) / vRho2;
      if(intersection < 0)
      {
        return kInfinity;
      }
      else if(std::fabs(p.z() + intersection * v.z()) < dz + 0.5 * kCarTolerance) return intersection;
      else
      {
        return kInfinity;
      }
    }
  }
  else if(sqr(rho2 - paraRho2 - .25 * kCarTolerance * kCarTolerance) <= kCarTolerance * kCarTolerance * paraRho2) // If this is true we're somewhere in the border.
  {
    G4ThreeVector normal(p.x(), p.y(), -k1/2);
    if(normal.dot(v) <= 0)
      return 0;
    else
      return kInfinity;
  }
  else
  {
    if(Inside(p) == kInside)
      G4Exception("G4Paraboloid::DistanceToIn(p,v)", "Notification",
                  JustWarning, "Point p is inside!");
    else
      G4Exception("G4Paraboloid::DistanceToIn(p,v)", "Notification",
                  JustWarning, "There's a bug in this function (apa)!");
      G4cerr << "p = " << p * (1/mm) << " mm" << G4endl;
      G4cerr << "v = " << v * (1/mm) << " mm" << G4endl;
      G4cerr << *this << G4endl;
    return 0;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// Calculate distance (<= actual) to closest surface of shape from outside
// - Return 0 if point inside

G4double G4Paraboloid::DistanceToIn(const G4ThreeVector& p) const
{
  G4double safe = 0;
  if(std::fabs(p.z()) > dz + 0.5 * kCarTolerance)
  // If we're below or above the paraboloid treat it as a cylinder with radius r2.
  {
    if(p.perp2() > sqr(r2 + 0.5 * kCarTolerance))
    {
      // This algorithm is exact for now but contains 2 sqrt calculations.
      // Might be better to replace with approximated version
      G4double dRho = p.perp() - r2;
      safe = std::sqrt(sqr(dRho) + sqr(std::fabs(p.z()) - dz));
    }
    else
    {
      safe = std::fabs(p.z()) - dz;
    }
  }
  else 
  {
    G4double paraRho = std::sqrt(k1 * p.z() + k2);
    G4double rho = p.perp();

    if(rho > paraRho + 0.5 * kCarTolerance)
    // Should check the value of paraRho here, for small values the following algorithm is bad.
    {
      // Lukas : safe = k1 / 2 * (paraRho - rho) / rho;
        safe = k1 / 2 * (-paraRho + rho) / rho;
	// G4cout<<"From G4Paraboloid::DistToIn safe="<<safe <<" rho="<<rho <<" paraPho="<<paraRho<<G4endl;
      if(safe < 0) safe = 0;
    }
  }
  return safe;
}

///////////////////////////////////////////////////////////////////////////////
//
// Calculate distance to surface of shape from `inside'

G4double G4Paraboloid::DistanceToOut(const G4ThreeVector& p,
                                    const G4ThreeVector& v,
                                    const G4bool calcNorm,
                                          G4bool *validNorm,
                                          G4ThreeVector *n  ) const
{
  G4double rho2 = p.perp2(), paraRho2 = std::fabs(k1 * p.z() + k2);
  G4double vRho2 = v.perp2(), intersection;
  if(calcNorm)
    *validNorm = false;

  // We have that the particle p follows the line x = p + s * v
  // meaning x = p.x() + s * v.x(), y = p.y() + s * v.y() and z = p.z() + s * v.z()
  // The equation for all points on the surface (surface expanded for to include all z)
  // x^2 + y^2 = k1 * z + k2 => .. =>
  // => s = (A +- std::sqrt(A^2 + B)) / vRho2
  // where 
  G4double A = k1 / 2 * v.z() - p.x() * v.x() - p.y() * v.y();
  // and
  G4double B = (-rho2 + paraRho2) * vRho2;

  if(rho2 < paraRho2 && sqr(rho2 - paraRho2 - 0.25 * kCarTolerance * kCarTolerance) > kCarTolerance * kCarTolerance * paraRho2 && std::fabs(p.z()) < dz - kCarTolerance) // Make sure it's safely inside.
  {
    if(v.z() > 0)
    {
      // It's heading upwards, check where it colides with the plane z = dz.
      // When it does, is that in the surface of the paraboloid.
      // z = p.z() + variable * v.z() for all points the particle can go.
      // => variable = (z - p.z()) / v.z() so intersection must be:
      intersection = (dz - p.z()) / v.z();
      G4ThreeVector ip = p + intersection * v; // Point of intersection.

      if(ip.perp2() < sqr(r2 + kCarTolerance))
      {
        if(calcNorm)
        {
          *n = G4ThreeVector(0, 0, 1);
          if(r2 < 0.5 * kCarTolerance || ip.perp2() > sqr(r2 - 0.5*kCarTolerance))
          {
            *n += G4ThreeVector(ip.x(), ip.y(), - k1 / 2).unit();
            *n = n->unit();
          }
          *validNorm = true;
        }
        return intersection;
      }
    }
    else if(v.z() < 0)
    {
      // It's heading downwards, check were it colides with the plane z = -dz.
      // When it does, is that in the surface of the paraboloid.
      // z = p.z() + variable * v.z() for all points the particle can go.
      // => variable = (z - p.z()) / v.z() so intersection must be:
      intersection = (-dz - p.z()) / v.z();
      G4ThreeVector ip = p + intersection * v; // Point of intersection.

      if(ip.perp2() < sqr(r1 + kCarTolerance * 0.5))
      {
        if(calcNorm)
        {
          *n = G4ThreeVector(0, 0, -1);
          if(r1 < 0.5 * kCarTolerance || ip.perp2() > sqr(r1 - 0.5*kCarTolerance))
          {
            *n += G4ThreeVector(ip.x(), ip.y(), - k1 / 2).unit();
            *n = n->unit();
          }
          *validNorm = true;
        }
        return intersection;
      }
    }

    // Now check for collisions with paraboloid surface.

    if(vRho2 == 0) // Needs to be treated seperately.
    {
      intersection = ((rho2 - k2)/k1 - p.z())/v.z();
      if(calcNorm)
      {
        G4ThreeVector intersectionP = p + v * intersection;
        *n = G4ThreeVector(intersectionP.x(), intersectionP.y(), -k1/2);
        *n = n->unit();

        *validNorm = true;
      }
      return intersection;
    }
    else if(A <= 0 && B >= sqr(A) * (sqr(vRho2) - 1) || A >= 0)
    {
      intersection = (A + std::sqrt(B + sqr(A))) / vRho2;
      if(calcNorm)
      {
        G4ThreeVector intersectionP = p + v * intersection;
        *n = G4ThreeVector(intersectionP.x(), intersectionP.y(), -k1/2);
        *n = n->unit();
        *validNorm = true;
      }
      return intersection;
    }
    G4Exception("G4Paraboloid::DistanceToOut(p,v,...)", "Notification",
                JustWarning, "There is no intersection between given line and solid!");
    G4cout << "p = " << p << G4endl;
    G4cout << "v = " << v << G4endl;
    G4cout << *this << G4endl;
    return kInfinity;
  }
  else if((rho2 < paraRho2 + kCarTolerance || sqr(rho2 - paraRho2 - 0.25 * kCarTolerance*kCarTolerance) < kCarTolerance * kCarTolerance * paraRho2 )
       && std::fabs(p.z()) < dz + kCarTolerance * 0.5) 
  // If this is true we're somewhere in the border.
  {
    G4ThreeVector normal = G4ThreeVector (p.x(), p.y(), -k1/2);

    if(std::fabs(p.z()) > dz - kCarTolerance * 0.5)
    {
      // We're in the lower or upper edge
      if(v.z() > 0 && p.z() > 0 || v.z() < 0 && p.z() < 0)
      // If we're headig out of the object that is treated here
      {
        if(calcNorm)
        {
          *validNorm = true;
          if(p.z() > 0)
            *n = G4ThreeVector(0, 0, 1);
          else
            *n = G4ThreeVector(0, 0, -1);
        }
        return 0;
      }

      if(v.z() == 0)
      {
        // Case where we're moving inside the surface needs to be treated separately.
        // Distance until it goes out throu a side is returned.
        G4double r = (p.z() > 0)? r2 : r1;
        G4double pDotV = p.dot(v);
        G4double A = vRho2 * ( sqr(r) - sqr(p.x()) - sqr(p.y()));
        intersection = (-pDotV + std::sqrt(A + sqr(pDotV))) / vRho2;

        if(calcNorm)
        {
          *validNorm = true;

          *n = (G4ThreeVector(0, 0, p.z()/std::fabs(p.z())) + G4ThreeVector(p.x() + v.x() * intersection, p.y() + v.y() * intersection, -k1/2).unit()).unit();
        }
        return intersection;
      }
    }
    else if(normal.dot(v) >= 0)
    {
      if(calcNorm)
      {
        *validNorm = true;
        *n = normal.unit();
      }
      return 0;
    }

    if(v.z() > 0)
    // Check for collision with upper edge.
    {
      intersection = (dz - p.z()) / v.z();
      G4ThreeVector ip = p + intersection * v;

      if(ip.perp2() < sqr(r2 - 0.5 * kCarTolerance))
      {
        if(calcNorm)
        {
          *validNorm = true;
          *n = G4ThreeVector(0, 0, 1);
        }
        return intersection;
      }
      else if(ip.perp2() < sqr(r2 + 0.5 * kCarTolerance))
      {
        if(calcNorm)
        {
          *validNorm = true;
          *n = G4ThreeVector(0, 0, 1) + G4ThreeVector(ip.x(), ip.y(), - k1 / 2).unit();
          *n = n->unit();
        }
        return intersection;
      }
    }
    if(r1 && v.z() < 0)
    // Check for collision with lower edge.
    {
      intersection = (-dz - p.z()) / v.z();
      G4ThreeVector ip = p + intersection * v;

      if(ip.perp2() < sqr(r1 - 0.5 * kCarTolerance))
      {
        if(calcNorm)
        {
          *validNorm = true;
          *n = G4ThreeVector(0, 0, -1);
        }
        return intersection;
      }
      else if(ip.perp2() < sqr(r1 + 0.5 * kCarTolerance))
      {
        if(calcNorm)
        {
          *validNorm = true;
          *n = G4ThreeVector(0, 0, -1) + G4ThreeVector(ip.x(), ip.y(), - k1 / 2).unit();
          *n = n->unit();
        }
        return intersection;
      }
    }

    if(vRho2 != 0)
      intersection = (A + std::sqrt(B + sqr(A))) / vRho2;
    else
      intersection = ((rho2 - k2) / k1 - p.z()) / v.z();

    if(calcNorm)
    {
      *validNorm = true;
      *n = G4ThreeVector(p.x() + intersection * v.x(), p.y() + intersection * v.y(), - k1 / 2);
    }
    return intersection;
  }
  else
  {
#ifdef G4SPECSDEBUG
    if(kOutside == Inside(p))
    {
      G4Exception("G4Paraboloid::DistanceToOut(p,v,...)", "Notification",
                  JustWarning, "Point p is outside!");
      G4cout << p << G4endl;
      G4cout << v << G4endl;
      G4cout << *this << G4endl;
    }
    else
      G4Exception("G4Paraboloid::DistanceToOut(p,v,...)", "Notification",
                  JustWarning, "There's an error in this functions code.");
#endif
    return kInfinity;
  }
  return 0;
} 

///////////////////////////////////////////////////////////////////////////////
//
// Calculate distance (<=actual) to closest surface of shape from inside

G4double G4Paraboloid::DistanceToOut(const G4ThreeVector& p) const
{
  G4double safe=0.0,rho,safeR,safeZ ;
  G4double tanRMax,secRMax,pRMax ;

#ifdef G4SPECSDEBUG
  if( Inside(p) == kOutside )
  {
     G4cout.precision(16) ;
     G4cout << G4endl ;
     DumpInfo();
     G4cout << "Position:"  << G4endl << G4endl ;
     G4cout << "p.x() = "   << p.x()/mm << " mm" << G4endl ;
     G4cout << "p.y() = "   << p.y()/mm << " mm" << G4endl ;
     G4cout << "p.z() = "   << p.z()/mm << " mm" << G4endl << G4endl ;
     G4Exception("G4Paraboloid::DistanceToOut(p)", "Notification", JustWarning, 
                 "Point p is outside !?" );
  }
#endif

  rho = p.perp();
  safeZ = dz - std::fabs(p.z()) ;

  tanRMax = (r2 - r1)*0.5/dz ;
  secRMax = std::sqrt(1.0 + tanRMax*tanRMax) ;
  pRMax   = tanRMax*p.z() + (r1+r2)*0.5 ;
  safeR  = (pRMax - rho)/secRMax ;

  if (safeZ < safeR)    safe = safeZ;
  else safe = safeR;
  if ( safe < 0.5 * kCarTolerance ) safe = 0;
  return safe ;
}

//////////////////////////////////////////////////////////////////////////
//
// G4EntityType

G4GeometryType G4Paraboloid::GetEntityType() const
{
  return G4String("G4Paraboloid");
}

//////////////////////////////////////////////////////////////////////////
//
// Stream object contents to an output stream

std::ostream& G4Paraboloid::StreamInfo( std::ostream& os ) const
{
  os << "-----------------------------------------------------------\n"
     << "    *** Dump for solid - " << GetName() << " ***\n"
     << "    ===================================================\n"
     << " Solid type: G4Paraboloid\n"
     << " Parameters: \n"
     << "    z half-axis:   " << dz/mm << " mm \n"
     << "    radius at -dz: " << r1/mm << " mm \n"
     << "    radius at dz:  " << r2/mm << " mm \n"
     << "-----------------------------------------------------------\n";

  return os;
}

////////////////////////////////////////////////////////////////////
//
// GetPointOnSurface

G4ThreeVector G4Paraboloid::GetPointOnSurface() const
{
  G4double A = (fSurfaceArea == 0)? CalculateSurfaceArea(): fSurfaceArea;
  G4double z = RandFlat::shoot(0.,1.);
  G4double phi = RandFlat::shoot(0., twopi);
  if(pi*(sqr(r1) + sqr(r2))/A >= z)
  {
    G4double rho;
    if(pi * sqr(r1) / A > z)
    {
      rho = RandFlat::shoot(0., 1.);
      rho = std::sqrt(rho);
      rho *= r1;
      return G4ThreeVector(rho * std::cos(phi), rho * sin(phi), -dz);
    }
    else
    {
      rho = RandFlat::shoot(0., 1);
      rho = std::sqrt(rho);
      rho *= r2;
      return G4ThreeVector(rho * std::cos(phi), rho * sin(phi), dz);
    }
  }
  else
  {
    z = RandFlat::shoot(0., 1.)*2*dz - dz;
    return G4ThreeVector(std::sqrt(z*k1 + k2)*std::cos(phi), std::sqrt(z*k1 + k2)*std::sin(phi), z);
  }
}

G4ThreeVectorList*
G4Paraboloid::CreateRotatedVertices(const G4AffineTransform& pTransform,
                                         G4int& noPolygonVertices) const
{
  G4ThreeVectorList *vertices;
  G4ThreeVector vertex;
  G4double meshAnglePhi, cosMeshAnglePhiPer2,
           crossAnglePhi, coscrossAnglePhi, sincrossAnglePhi, sAnglePhi, sRho, dRho, rho, lastRho = 0., swapRho;
  G4double rx, ry, rz, k3, k4, zm;
  G4int crossSectionPhi, noPhiCrossSections, noRhoSections;

  // Phi cross sections
  //
  noPhiCrossSections=G4int (twopi/kMeshAngleDefault)+1;

  if (noPhiCrossSections<kMinMeshSections)
  {
    noPhiCrossSections=kMinMeshSections;
  }
  else if (noPhiCrossSections>kMaxMeshSections)
  {
    noPhiCrossSections=kMaxMeshSections;
  }
  meshAnglePhi=twopi/(noPhiCrossSections-1);

  sAnglePhi = -meshAnglePhi*0.5*0;
  cosMeshAnglePhiPer2 = std::cos(meshAnglePhi / 2.);

  noRhoSections = G4int(pi/2/kMeshAngleDefault) + 1;
  // There is no obvious value for noRhoSections, at the moment the parabola is
  // viewed as a quarter circle mean this formula for it.

  // An alternetive would be to calculate max deviation from parabola and
  // keep adding new vertices there until it was under a decided constant.

  // maxDeviation on a line between points (rho1, z1) and (rho2, z2) is given
  // by rhoMax = sqrt(k1 * z + k2) - z * (rho2 - rho1) / (z2 - z1) - (rho1 * z2 - rho2 * z1) / (z2 - z1)
  // where z is k1 / 2 * (rho1 + rho2) - k2 / k1

  sRho = r1;
  dRho = (r2 - r1) / double(noRhoSections - 1);

  vertices=new G4ThreeVectorList();

  if (vertices)
  {
    for (crossSectionPhi=0; crossSectionPhi<noPhiCrossSections;
         crossSectionPhi++)
    {
      crossAnglePhi=sAnglePhi+crossSectionPhi*meshAnglePhi;
      coscrossAnglePhi=std::cos(crossAnglePhi);
      sincrossAnglePhi=std::sin(crossAnglePhi);
      lastRho = 0;
      for (int iRho=0; iRho < noRhoSections;
           iRho++)
      {
        // Compute coordinates of cross section at section crossSectionPhi
        //
        if(iRho == noRhoSections - 1)
          rho = r2;
        else
        {
          rho = iRho * dRho + sRho;

          // This part is to ensure that the vertices will form a volume larger than the paraboloid
          k3 = k1 / (2*rho + dRho);
          k4 = rho - k3 * (sqr(rho) - k2) / k1;
          zm = (sqr(k1 / (2 * k3)) - k2) / k1;
          rho += std::sqrt(k1 * zm + k2) - zm * k3 - k4;
        }

        rho += (1 / cosMeshAnglePhiPer2 - 1) * (iRho * dRho + sRho);


        if(rho < lastRho)
        {
          swapRho = lastRho;
          lastRho = rho + dRho;
          rho = swapRho;
        }
        else
        {
          lastRho = rho + dRho;
        }

        rx = coscrossAnglePhi*rho;
        ry = sincrossAnglePhi*rho;
        rz = (sqr(iRho * dRho + sRho) - k2) / k1;
        vertex = G4ThreeVector(rx,ry,rz);
        vertices->push_back(pTransform.TransformPoint(vertex));
      }
    }    // Phi
    noPolygonVertices = noRhoSections ;
  }
  else
  {
    DumpInfo();
    G4Exception("G4Paraboloid::CreateRotatedVertices()",
                "FatalError", FatalException,
                "Error in allocation of vertices. Out of memory !");
  }

  return vertices;
}

/////////////////////////////////////////////////////////////////////////////
//
// Methods for visualisation

void G4Paraboloid::DescribeYourselfTo (G4VGraphicsScene& scene) const
{
  scene.AddSolid(*this);
}

G4NURBS* G4Paraboloid::CreateNURBS () const
{
  // Box for now!!!
  //
  return new G4NURBSbox(r1, r1, dz);
}

G4Polyhedron* G4Paraboloid::CreatePolyhedron () const
{
  return new G4PolyhedronParaboloid(r1, r2, dz, 0., twopi); // FIXME
}


G4Polyhedron* G4Paraboloid::GetPolyhedron () const
{
  if (!fpPolyhedron ||
      fpPolyhedron->GetNumberOfRotationStepsAtTimeOfCreation() !=
      fpPolyhedron->GetNumberOfRotationSteps())
  {
    delete fpPolyhedron;
    fpPolyhedron = CreatePolyhedron();
  }
  return fpPolyhedron;
}

