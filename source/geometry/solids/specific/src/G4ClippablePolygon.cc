//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4ClippablePolygon.cc,v 1.8 2002-10-30 13:52:22 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class source file
//
//
// G4ClippablePolygon.cc
//
// Includes code from G4VSolid (P.Kent, V.Grichine, J.Allison)
//
// --------------------------------------------------------------------

#include "G4ClippablePolygon.hh"

#include "G4VoxelLimits.hh"

//
// Constructor
//
G4ClippablePolygon::G4ClippablePolygon()
  : normal(0.,0.,0.)
{
}

//
// Destructor
//
G4ClippablePolygon::~G4ClippablePolygon()
{
}

//
// AddVertexInOrder
//
void G4ClippablePolygon::AddVertexInOrder( const G4ThreeVector vertex )
{
  vertices.push_back( vertex );
}


//
// ClearAllVertices
//
void G4ClippablePolygon::ClearAllVertices()
{
  vertices.clear();
}



//
// Clip
//
G4bool G4ClippablePolygon::Clip( const G4VoxelLimits &voxelLimit )
{
  if (voxelLimit.IsLimited()) {
    ClipAlongOneAxis( voxelLimit, kXAxis );
    ClipAlongOneAxis( voxelLimit, kYAxis );
    ClipAlongOneAxis( voxelLimit, kZAxis );
  }
  
  return (vertices.size() > 0);
}


//
// PartialClip
//
// Clip, while ignoring the indicated axis
//
G4bool G4ClippablePolygon::PartialClip( const G4VoxelLimits &voxelLimit,
                                        const EAxis IgnoreMe )
{
  if (voxelLimit.IsLimited()) {
    if (IgnoreMe != kXAxis) ClipAlongOneAxis( voxelLimit, kXAxis );
    if (IgnoreMe != kYAxis) ClipAlongOneAxis( voxelLimit, kYAxis );
    if (IgnoreMe != kZAxis) ClipAlongOneAxis( voxelLimit, kZAxis );
  }
  
  return (vertices.size() > 0);
}


//
// GetExtent
//
G4bool G4ClippablePolygon::GetExtent( const EAxis axis, 
                                            G4double &min,
                                            G4double &max ) const
{
  //
  // Okay, how many entries do we have?
  //
  G4int noLeft = vertices.size();
  
  //
  // Return false if nothing is left
  //
  if (noLeft == 0) return false;
  
  //
  // Initialize min and max to our first vertex
  //
  min = max = vertices[0].operator()( axis );
  
  //
  // Compare to the rest
  //
  G4int i;
  for( i=1; i<noLeft; i++ )
  {
    G4double component = vertices[i].operator()( axis );
    if (component < min )
      min = component;
    else if (component > max )
      max = component;
  }
  
  return true;
}


//
// GetMinPoint
//
// Returns pointer to minimum point along the specified axis.
// Take care! Do not use pointer after destroying parent polygon.
//
const G4ThreeVector *G4ClippablePolygon::GetMinPoint( const EAxis axis ) const
{
  G4int noLeft = vertices.size();
  if (noLeft==0)
    G4Exception("G4ClippablePolygon::GetMinPoint() - empty polygon");
  
  const G4ThreeVector *answer = &(vertices[0]);
  G4double min = answer->operator()(axis);

  G4int i;
  for( i=1; i<noLeft; i++ )
  {
    G4double component = vertices[i].operator()( axis );
    if (component < min)
    {
      answer = &(vertices[i]);
      min = component;
    }
  }
  
  return answer;
}


//
// GetMaxPoint
//
// Returns pointer to maximum point along the specified axis.
// Take care! Do not use pointer after destroying parent polygon.
//
const G4ThreeVector *G4ClippablePolygon::GetMaxPoint( const EAxis axis ) const
{
  G4int noLeft = vertices.size();
  if (noLeft==0)
    G4Exception("G4ClippablePolygon::GetMaxPoint() - empty polygon");
  
  const G4ThreeVector *answer = &(vertices[0]);
  G4double max = answer->operator()(axis);

  G4int i;
  for( i=1; i<noLeft; i++ )
  {
    G4double component = vertices[i].operator()( axis );
    if (component > max)
    {
      answer = &(vertices[i]);
      max = component;
    }
  }
  
  return answer;
}
    

//
// InFrontOf
//
// Decide if this polygon is in "front" of another when
// viewed along the specified axis. For our purposes here,
// it is sufficient to use the minimum extent of the
// polygon along the axis to determine this.
//
// In case the minima of the two polygons are equal,
// we use a more sophisticated test.
//
// Note that it is possible for the two following
// statements to both return true or both return false:
//         polygon1.InFrontOf(polygon2)
//         polygon2.BehindOf(polygon1)
//
G4bool G4ClippablePolygon::InFrontOf( const G4ClippablePolygon &other,
                                            EAxis axis ) const
{
  //
  // If things are empty, do something semi-sensible
  //
  G4int noLeft = vertices.size();
  if (noLeft==0) return false;
  
  if (other.Empty()) return true;

  //
  // Get minimum of other polygon
  //
  const G4ThreeVector *minPointOther = other.GetMinPoint( axis );
  const G4double minOther = minPointOther->operator()(axis);
  
  //
  // Get minimum of this polygon
  //
  const G4ThreeVector *minPoint = GetMinPoint( axis );
  const G4double min = minPoint->operator()(axis);
  
  //
  // Easy decision
  //
  if (min < minOther-kCarTolerance) return true;    // Clear winner
  
  if (minOther < min-kCarTolerance) return false;    // Clear loser
  
  //
  // We have a tie (this will not be all that rare since our
  // polygons are connected)
  //
  // Check to see if there is a vertex in the other polygon
  // that is behind this one (or vice versa)
  //
  G4bool answer;
  G4ThreeVector normalOther = other.GetNormal();
  
  if (fabs(normalOther(axis)) > fabs(normal(axis)))
  {
    G4double minP, maxP;
    GetPlanerExtent( *minPointOther, normalOther, minP, maxP );
    
    answer = (normalOther(axis) > 0) ? (minP < -kCarTolerance)
                                     : (maxP > +kCarTolerance);
  }
  else
  {
    G4double minP, maxP;
    other.GetPlanerExtent( *minPoint, normal, minP, maxP );
    
    answer = (normal(axis) > 0) ? (maxP > +kCarTolerance)
                                : (minP < -kCarTolerance);
  }
  return answer;
}

//
// BehindOf
//
// Decide if this polygon is behind another.
// See notes in method "InFrontOf"
//
G4bool G4ClippablePolygon::BehindOf( const G4ClippablePolygon &other,
                                           EAxis axis ) const
{
  //
  // If things are empty, do something semi-sensible
  //
  G4int noLeft = vertices.size();
  if (noLeft==0) return false;
  
  if (other.Empty()) return true;

  //
  // Get minimum of other polygon
  //
  const G4ThreeVector *maxPointOther = other.GetMaxPoint( axis );
  const G4double maxOther = maxPointOther->operator()(axis);
  
  //
  // Get minimum of this polygon
  //
  const G4ThreeVector *maxPoint = GetMaxPoint( axis );
  const G4double max = maxPoint->operator()(axis);
  
  //
  // Easy decision
  //
  if (max > maxOther+kCarTolerance) return true;    // Clear winner
  
  if (maxOther > max+kCarTolerance) return false;    // Clear loser
  
  //
  // We have a tie (this will not be all that rare since our
  // polygons are connected)
  //
  // Check to see if there is a vertex in the other polygon
  // that is in front of this one (or vice versa)
  //
  G4bool answer;
  G4ThreeVector normalOther = other.GetNormal();
  
  if (fabs(normalOther(axis)) > fabs(normal(axis)))
  {
    G4double minP, maxP;
    GetPlanerExtent( *maxPointOther, normalOther, minP, maxP );
    
    answer = (normalOther(axis) > 0) ? (maxP > +kCarTolerance)
                                     : (minP < -kCarTolerance);
  }
  else
  {
    G4double minP, maxP;
    other.GetPlanerExtent( *maxPoint, normal, minP, maxP );
    
    answer = (normal(axis) > 0) ? (minP < -kCarTolerance)
                                : (maxP > +kCarTolerance);
  }
  return answer;
}


//
// GetPlanerExtent
//
// Get min/max distance in or out of a plane
//
G4bool G4ClippablePolygon::GetPlanerExtent( const G4ThreeVector &pointOnPlane, 
                                            const G4ThreeVector &planeNormal,
                                                  G4double &min,
                                                  G4double &max ) const
{
  //
  // Okay, how many entries do we have?
  //
  G4int noLeft = vertices.size();
  
  //
  // Return false if nothing is left
  //
  if (noLeft == 0) return false;
  
  //
  // Initialize min and max to our first vertex
  //
  min = max = planeNormal.dot(vertices[0]-pointOnPlane);
  
  //
  // Compare to the rest
  //
  G4int i;
  for( i=1; i<noLeft; i++ )
  {
    G4double component = planeNormal.dot(vertices[i] - pointOnPlane);
    if (component < min )
      min = component;
    else if (component > max )
      max = component;
  }
  
  return true;
}


//
// Clip along just one axis, as specified in voxelLimit
//
void G4ClippablePolygon::ClipAlongOneAxis( const G4VoxelLimits &voxelLimit,
                                           const EAxis axis )
{    
  if (!voxelLimit.IsLimited(axis)) return;
  
  G4ThreeVectorList tempPolygon;

  //
  // Build a "simple" voxelLimit that includes only the min extent
  // and apply this to our vertices, producing result in tempPolygon
  //
  G4VoxelLimits simpleLimit1;
  simpleLimit1.AddLimit( axis, voxelLimit.GetMinExtent(axis), kInfinity );
  ClipToSimpleLimits( vertices, tempPolygon, simpleLimit1 );

  //
  // If nothing is left from the above clip, we might as well return now
  // (but with an empty vertices)
  //
  if (tempPolygon.size() == 0)
  {
    vertices.clear();
    return;
  }

  //
  // Now do the same, but using a "simple" limit that includes only the max
  // extent. Apply this to out tempPolygon, producing result in vertices.
  //
  G4VoxelLimits simpleLimit2;
  simpleLimit2.AddLimit( axis, -kInfinity, voxelLimit.GetMaxExtent(axis) );
  ClipToSimpleLimits( tempPolygon, vertices, simpleLimit2 );

  //
  // If nothing is left, return now
  //
  if (vertices.size() == 0) return;
}


//
// pVoxelLimits must be only limited along one axis, and either the maximum
// along the axis must be +kInfinity, or the minimum -kInfinity
//
void G4ClippablePolygon::ClipToSimpleLimits( G4ThreeVectorList& pPolygon,
                                             G4ThreeVectorList& outputPolygon,
                                       const G4VoxelLimits& pVoxelLimit   )
{
  G4int i;
  G4int noVertices=pPolygon.size();
  G4ThreeVector vEnd,vStart;

  outputPolygon.clear();
    
  for (i=0;i<noVertices;i++)
  {
    vStart=pPolygon[i];
    if (i==noVertices-1)
    {
      vEnd=pPolygon[0];
    }
    else
    {
      vEnd=pPolygon[i+1];
    }

    if (pVoxelLimit.Inside(vStart))
    {
      if (pVoxelLimit.Inside(vEnd))
      {
        // vStart and vEnd inside -> output end point
        //
        outputPolygon.push_back(vEnd);
      }
      else
      {
        // vStart inside, vEnd outside -> output crossing point
        //
        pVoxelLimit.ClipToLimits(vStart,vEnd);
        outputPolygon.push_back(vEnd);
      }
    }
    else
    {
      if (pVoxelLimit.Inside(vEnd))
      {
        // vStart outside, vEnd inside -> output inside section
        //
        pVoxelLimit.ClipToLimits(vStart,vEnd);
        outputPolygon.push_back(vStart);
        outputPolygon.push_back(vEnd);
      }
      else    // Both point outside -> no output
      {
      }
    }
  }
}
