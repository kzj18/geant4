// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4OpenGLStoredWin32.hh,v 1.2 1999-01-09 16:22:45 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// OpenGLStoredWin32 graphics system factory.

#if defined (G4VIS_BUILD_OPENGLWIN32_DRIVER) || defined (G4VIS_USE_OPENGLWIN32)

#ifndef G4OPENGLSTOREDWIN32_HH
#define G4OPENGLSTOREDWIN32_HH

#include "G4VGraphicsSystem.hh"

class G4OpenGLStoredWin32: public G4VGraphicsSystem {
public:
  G4OpenGLStoredWin32 ();
  G4VSceneHandler* CreateScene ();
  G4VViewer*  CreateView  (G4VSceneHandler&);
};

#endif

#endif
