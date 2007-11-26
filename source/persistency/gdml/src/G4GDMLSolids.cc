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
//
// $Id: G4GDMLSolids.cc,v 1.18 2007-11-26 14:31:32 ztorzsok Exp $
// GEANT4 tag $ Name:$
//
// class G4GDMLSolids Implementation
//
// Original author: Zoltan Torzsok, November 2007
//
// --------------------------------------------------------------------

#include "G4GDMLSolids.hh"

std::string G4GDMLSolids::nameProcess(const std::string& in) {

   std::string out(file);
   
   std::string::size_type open = in.find("[",0);

   out.append(in,0,open);
   
   while (open != std::string::npos) {
   
      std::string::size_type close = in.find("]",open);

      if (close == std::string::npos) G4Exception("Bracket mismatch in loop!");
   
      std::string expr = in.substr(open+1,close-open-1);

      std::stringstream ss;
   
      ss << "[" << evaluator->Evaluate(expr) << "]";
   
      out.append(ss.str());

      open = in.find("[",close);
   }

   return out;
}

void G4GDMLSolids::booleanRead(const xercesc::DOMElement* const element,const BooleanOp op) {

   G4String name;
   G4String first;
   G4String second;

   G4ThreeVector position;
   G4ThreeVector rotation;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name") name = attribute_value;
   }

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="first"   ) first = refRead(child);         else
      if (tag=="second"  ) second = refRead(child);        else
      if (tag=="position") position = positionRead(child); else
      if (tag=="rotation") rotation = rotationRead(child); else
      G4Exception("GDML: Unknown tag in boolean solid: "+tag);
   }

   G4RotationMatrix rot;

   rot.rotateX(rotation.x());
   rot.rotateY(rotation.y());
   rot.rotateZ(rotation.z());

   G4Transform3D transform(rot,position);

   G4VSolid* firstSolid = getSolid(nameProcess(first));
   G4VSolid* secondSolid = getSolid(nameProcess(second));

   if (op==UNION       ) new G4UnionSolid(nameProcess(name),firstSolid,secondSolid,transform);        else
   if (op==SUBTRACTION ) new G4SubtractionSolid(nameProcess(name),firstSolid,secondSolid,transform);  else
   if (op==INTERSECTION) new G4IntersectionSolid(nameProcess(name),firstSolid,secondSolid,transform);
}

void G4GDMLSolids::boxRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String x;
   G4String y;
   G4String z;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="x"    ) x     = attribute_value; else
      if (attribute_name=="y"    ) y     = attribute_value; else
      if (attribute_name=="z"    ) z     = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);

   G4double _x = evaluator->Evaluate(x)*_lunit;
   G4double _y = evaluator->Evaluate(y)*_lunit;
   G4double _z = evaluator->Evaluate(z)*_lunit;

   _x *= 0.5;
   _y *= 0.5;
   _z *= 0.5;

   new G4Box(nameProcess(name),_x,_y,_z);
}

void G4GDMLSolids::coneRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String rmin1;
   G4String rmax1;
   G4String rmin2;
   G4String rmax2;
   G4String z;
   G4String startphi;
   G4String deltaphi;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"    ) name     = attribute_value; else
      if (attribute_name=="lunit"   ) lunit    = attribute_value; else
      if (attribute_name=="aunit"   ) aunit    = attribute_value; else
      if (attribute_name=="rmin1"   ) rmin1    = attribute_value; else
      if (attribute_name=="rmax1"   ) rmax1    = attribute_value; else
      if (attribute_name=="rmin2"   ) rmin2    = attribute_value; else
      if (attribute_name=="rmax2"   ) rmax2    = attribute_value; else
      if (attribute_name=="z"       ) z        = attribute_value; else
      if (attribute_name=="startphi") startphi = attribute_value; else
      if (attribute_name=="deltaphi") deltaphi = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _rmin1    = evaluator->Evaluate(rmin1   )*_lunit;
   G4double _rmax1    = evaluator->Evaluate(rmax1   )*_lunit;
   G4double _rmin2    = evaluator->Evaluate(rmin2   )*_lunit;
   G4double _rmax2    = evaluator->Evaluate(rmax2   )*_lunit;
   G4double _z        = evaluator->Evaluate(z       )*_lunit;
   G4double _startphi = evaluator->Evaluate(startphi)*_aunit;
   G4double _deltaphi = evaluator->Evaluate(deltaphi)*_aunit;;

   _z *= 0.5;

   new G4Cons(nameProcess(name),_rmin1,_rmax1,_rmin2,_rmax2,_z,_startphi,_deltaphi);
}

void G4GDMLSolids::ellipsoidRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String ax;
   G4String by;
   G4String cz;
   G4String zcut1;
   G4String zcut2; 

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="ax"   ) ax    = attribute_value; else
      if (attribute_name=="by"   ) by    = attribute_value; else
      if (attribute_name=="cz"   ) cz    = attribute_value; else
      if (attribute_name=="zcut1") zcut1 = attribute_value; else
      if (attribute_name=="zcut2") zcut2 = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);

   G4double _ax    = evaluator->Evaluate(ax   )*_lunit;
   G4double _by    = evaluator->Evaluate(by   )*_lunit;
   G4double _cz    = evaluator->Evaluate(cz   )*_lunit;
   G4double _zcut1 = evaluator->Evaluate(zcut1)*_lunit;
   G4double _zcut2 = evaluator->Evaluate(zcut2)*_lunit; 

   new G4Ellipsoid(nameProcess(name),_ax,_by,_cz,_zcut1,_zcut2);
}

void G4GDMLSolids::eltubeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String dx;
   G4String dy;
   G4String dz;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="dx"   ) dx    = attribute_value; else
      if (attribute_name=="dy"   ) dy    = attribute_value; else
      if (attribute_name=="dz"   ) dz    = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);

   G4double _dx = evaluator->Evaluate(dx)*_lunit;
   G4double _dy = evaluator->Evaluate(dy)*_lunit;
   G4double _dz = evaluator->Evaluate(dz)*_lunit;

   new G4EllipticalTube(nameProcess(name),_dx,_dy,_dz);
}

void G4GDMLSolids::hypeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String rmin;
   G4String rmax;
   G4String inst;
   G4String outst;
   G4String z;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="aunit") aunit = attribute_value; else
      if (attribute_name=="rmin" ) rmin  = attribute_value; else
      if (attribute_name=="rmax" ) rmax  = attribute_value; else
      if (attribute_name=="inst" ) inst  = attribute_value; else
      if (attribute_name=="outst") outst = attribute_value; else
      if (attribute_name=="z"    ) z     = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _rmin  = evaluator->Evaluate(rmin )*_lunit;
   G4double _rmax  = evaluator->Evaluate(rmax )*_lunit;;
   G4double _inst  = evaluator->Evaluate(inst )*_aunit;
   G4double _outst = evaluator->Evaluate(outst)*_aunit;
   G4double _z     = evaluator->Evaluate(z    )*_lunit;

   _z *= 0.5;

   new G4Hype(nameProcess(name),_rmin,_rmax,_inst,_outst,_z);
}

void G4GDMLSolids::loopRead(const xercesc::DOMElement* const element) {

   G4String var;
   G4String from;
   G4String to;
   G4String step;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="var" ) var  = attribute_value; else
      if (attribute_name=="from") from = attribute_value; else
      if (attribute_name=="to"  ) to   = attribute_value; else
      if (attribute_name=="step") step = attribute_value;
   }

   G4double _var  = evaluator->Evaluate(var );
   G4double _from = evaluator->Evaluate(from);
   G4double _to   = evaluator->Evaluate(to  );
   G4double _step = evaluator->Evaluate(step);
   
   if (!from.empty()) _var = _from;

   while (_var <= _to) {
   
      evaluator->setVariable(var,_var);
      Read(element,evaluator,file);

      _var += _step;
   }
}

void G4GDMLSolids::orbRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String r;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="r"    ) r     = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   
   G4double _r = evaluator->Evaluate(r)*_lunit;

   new G4Orb(nameProcess(name),_r);
}

void G4GDMLSolids::paraRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String x;
   G4String y;
   G4String z;
   G4String alpha;
   G4String theta;
   G4String phi;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="aunit") aunit = attribute_value; else
      if (attribute_name=="x"    ) x     = attribute_value; else
      if (attribute_name=="y"    ) y     = attribute_value; else
      if (attribute_name=="z"    ) z     = attribute_value; else
      if (attribute_name=="alpha") alpha = attribute_value; else
      if (attribute_name=="theta") theta = attribute_value; else
      if (attribute_name=="phi"  ) phi   = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _x     = evaluator->Evaluate(x    )*_lunit;
   G4double _y     = evaluator->Evaluate(y    )*_lunit;
   G4double _z     = evaluator->Evaluate(z    )*_lunit;
   G4double _alpha = evaluator->Evaluate(alpha)*_aunit;
   G4double _theta = evaluator->Evaluate(theta)*_aunit;
   G4double _phi   = evaluator->Evaluate(phi  )*_aunit;

   _x *= 0.5;
   _y *= 0.5;
   _z *= 0.5;

   new G4Para(nameProcess(name),_x,_y,_z,_alpha,_theta,_phi);
}

void G4GDMLSolids::polyconeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String startphi;
   G4String deltaphi;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"    ) name     = attribute_value; else
      if (attribute_name=="lunit"   ) lunit    = attribute_value; else
      if (attribute_name=="aunit"   ) aunit    = attribute_value; else
      if (attribute_name=="startphi") startphi = attribute_value; else
      if (attribute_name=="deltaphi") deltaphi = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _startphi = evaluator->Evaluate(startphi)*_aunit;
   G4double _deltaphi = evaluator->Evaluate(deltaphi)*_aunit;

   std::vector<zplaneType> zplaneList;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="zplane") zplaneList.push_back(zplaneRead(child,_lunit));
   }

   G4int numZPlanes = zplaneList.size();

   G4double *rmin_array = new G4double[numZPlanes];
   G4double *rmax_array = new G4double[numZPlanes];
   G4double* z_array    = new G4double[numZPlanes];

   for (G4int i=0;i<numZPlanes;i++) {
   
      rmin_array[i] = zplaneList[i].rmin;
      rmax_array[i] = zplaneList[i].rmax;
      z_array[i]    = zplaneList[i].z;
   }

   new G4Polycone(nameProcess(name),_startphi,_deltaphi,numZPlanes,z_array,rmin_array,rmax_array);
}

void G4GDMLSolids::polyhedraRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String startphi;
   G4String deltaphi;
   G4String numsides;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"    ) { name     = attribute_value; } else
      if (attribute_name=="lunit"   ) { lunit    = attribute_value; } else
      if (attribute_name=="aunit"   ) { aunit    = attribute_value; } else
      if (attribute_name=="startphi") { startphi = attribute_value; } else
      if (attribute_name=="deltaphi") { deltaphi = attribute_value; } else
      if (attribute_name=="numsides") { numsides = attribute_value; }
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _startphi = evaluator->Evaluate(startphi)*_aunit;
   G4double _deltaphi = evaluator->Evaluate(deltaphi)*_aunit;
   G4double _numsides = evaluator->Evaluate(numsides);

   std::vector<zplaneType> zplaneList;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="zplane") zplaneList.push_back(zplaneRead(child,_lunit));
   }

   G4int numZPlanes = zplaneList.size();

   G4double *rmin_array = new G4double[numZPlanes];
   G4double *rmax_array = new G4double[numZPlanes];
   G4double* z_array    = new G4double[numZPlanes];

   for (G4int i=0;i<numZPlanes;i++) {
   
      rmin_array[i] = zplaneList[i].rmin;
      rmax_array[i] = zplaneList[i].rmax;
      z_array[i]    = zplaneList[i].z;
   }

   new G4Polyhedra(nameProcess(name),_startphi,_deltaphi,(G4int)_numsides,numZPlanes,z_array,rmin_array,rmax_array);
}

G4ThreeVector G4GDMLSolids::positionRead(const xercesc::DOMElement* const element) {

   G4String unit;
   G4String x;
   G4String y;
   G4String z;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="unit") { unit = attribute_value; } else
      if (attribute_name=="x"   ) { x    = attribute_value; } else
      if (attribute_name=="y"   ) { y    = attribute_value; } else
      if (attribute_name=="z"   ) { z    = attribute_value; }
   }

   G4double _unit = evaluator->Evaluate(unit);

   G4double _x = evaluator->Evaluate(x)*_unit;
   G4double _y = evaluator->Evaluate(y)*_unit;
   G4double _z = evaluator->Evaluate(z)*_unit;
   
   return G4ThreeVector(_x,_y,_z);
}

G4QuadrangularFacet* G4GDMLSolids::quadrangularRead(const xercesc::DOMElement* const element) {

   G4String v1;
   G4String v2;
   G4String v3;
   G4String v4;
   G4String type;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="v1"  ) { v1   = attribute_value; } else
      if (attribute_name=="v2"  ) { v2   = attribute_value; } else
      if (attribute_name=="v3"  ) { v3   = attribute_value; } else
      if (attribute_name=="v4"  ) { v4   = attribute_value; } else
      if (attribute_name=="type") { type = attribute_value; }
   }

   const G4ThreeVector* ptr1 = define.getPosition(nameProcess(v1));
   const G4ThreeVector* ptr2 = define.getPosition(nameProcess(v2));
   const G4ThreeVector* ptr3 = define.getPosition(nameProcess(v3));
   const G4ThreeVector* ptr4 = define.getPosition(nameProcess(v4));

   return new G4QuadrangularFacet(*ptr1,*ptr2,*ptr3,*ptr4,(type=="RELATIVE")?(RELATIVE):(ABSOLUTE));
}

G4String G4GDMLSolids::refRead(const xercesc::DOMElement* const element) {

   G4String ref;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="ref") ref = attribute_value;
   }

   return ref;
}

void G4GDMLSolids::reflectedSolidRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String solid;
   G4String sx;
   G4String sy;
   G4String sz;
   G4String rx;
   G4String ry;
   G4String rz;
   G4String dx;
   G4String dy;
   G4String dz;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value; else
      if (attribute_name=="aunit") aunit = attribute_value; else
      if (attribute_name=="solid") solid = attribute_value; else
      if (attribute_name=="sx"   ) sx    = attribute_value; else
      if (attribute_name=="sy"   ) sy    = attribute_value; else
      if (attribute_name=="sz"   ) sz    = attribute_value; else
      if (attribute_name=="rx"   ) rx    = attribute_value; else
      if (attribute_name=="ry"   ) ry    = attribute_value; else
      if (attribute_name=="rz"   ) rz    = attribute_value; else
      if (attribute_name=="dx"   ) dx    = attribute_value; else
      if (attribute_name=="dy"   ) dy    = attribute_value; else
      if (attribute_name=="dz"   ) dz    = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _sx = evaluator->Evaluate(sx);
   G4double _sy = evaluator->Evaluate(sy);
   G4double _sz = evaluator->Evaluate(sz);
   G4double _rx = evaluator->Evaluate(rx)*_aunit;
   G4double _ry = evaluator->Evaluate(ry)*_aunit;
   G4double _rz = evaluator->Evaluate(rz)*_aunit;
   G4double _dx = evaluator->Evaluate(dx)*_lunit;
   G4double _dy = evaluator->Evaluate(dy)*_lunit;
   G4double _dz = evaluator->Evaluate(dz)*_lunit;

   G4VSolid* solidPtr = getSolid(nameProcess(solid));

   G4RotationMatrix rot;
   
   rot.rotateX(_rx);
   rot.rotateY(_ry);
   rot.rotateZ(_rz);

   G4ThreeVector trans(_dx,_dy,_dz);

   G4Scale3D scale(_sx,_sy,_sz);

   G4Transform3D transform(rot,trans);
   transform = transform*scale;
          
   new G4ReflectedSolid(nameProcess(name),solidPtr,transform);
}

G4ThreeVector G4GDMLSolids::rotationRead(const xercesc::DOMElement* const element) {

   G4String unit;
   G4String x;
   G4String y;
   G4String z;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="unit") { unit = attribute_value; } else
      if (attribute_name=="x"   ) { x    = attribute_value; } else
      if (attribute_name=="y"   ) { y    = attribute_value; } else
      if (attribute_name=="z"   ) { z    = attribute_value; }
   }

   G4double _unit = evaluator->Evaluate(unit);

   G4double _x = evaluator->Evaluate(x)*_unit;
   G4double _y = evaluator->Evaluate(y)*_unit;
   G4double _z = evaluator->Evaluate(z)*_unit;
   
   return G4ThreeVector(_x,_y,_z);
}

G4ExtrudedSolid::ZSection G4GDMLSolids::sectionRead(const xercesc::DOMElement* const element,G4double _lunit) {

   G4String zPosition;
   G4String xOffset;
   G4String yOffset;
   G4String scalingFactor;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="zPosition"    ) zPosition     = attribute_value; else
      if (attribute_name=="xOffset"      ) xOffset       = attribute_value; else
      if (attribute_name=="yOffset"      ) yOffset       = attribute_value; else
      if (attribute_name=="scalingFactor") scalingFactor = attribute_value;
   }

   G4double _zPosition     = evaluator->Evaluate(zPosition    )*_lunit;
   G4double _xOffset       = evaluator->Evaluate(xOffset      )*_lunit;
   G4double _yOffset       = evaluator->Evaluate(yOffset      )*_lunit;
   G4double _scalingFactor = evaluator->Evaluate(scalingFactor);

   return G4ExtrudedSolid::ZSection(_zPosition,G4TwoVector(_xOffset,_yOffset),_scalingFactor);
}

void G4GDMLSolids::sphereRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String rmin;
   G4String rmax;
   G4String startphi;
   G4String deltaphi;
   G4String starttheta;
   G4String deltatheta;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name")       name       = attribute_value; else
      if (attribute_name=="lunit")      lunit      = attribute_value; else
      if (attribute_name=="aunit")      aunit      = attribute_value; else
      if (attribute_name=="rmin")       rmin       = attribute_value; else
      if (attribute_name=="rmax")       rmax       = attribute_value; else
      if (attribute_name=="startphi")   startphi   = attribute_value; else
      if (attribute_name=="deltaphi")   deltaphi   = attribute_value; else
      if (attribute_name=="starttheta") starttheta = attribute_value; else
      if (attribute_name=="deltatheta") deltatheta = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _rmin       = evaluator->Evaluate(rmin      )*_lunit;
   G4double _rmax       = evaluator->Evaluate(rmax      )*_lunit;
   G4double _startphi   = evaluator->Evaluate(startphi  )*_aunit;
   G4double _deltaphi   = evaluator->Evaluate(deltaphi  )*_aunit;
   G4double _starttheta = evaluator->Evaluate(starttheta)*_aunit;
   G4double _deltatheta = evaluator->Evaluate(deltatheta)*_aunit;

   new G4Sphere(nameProcess(name),_rmin,_rmax,_startphi,_deltaphi,_starttheta,_deltatheta);
}

void G4GDMLSolids::tessellatedRead(const xercesc::DOMElement* const element) {

   G4String name;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name") name = attribute_value;
   }
   
   G4TessellatedSolid *tessellated = new G4TessellatedSolid(nameProcess(name));

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="triangular") tessellated->AddFacet(triangularRead(child)); else
      if (tag=="quadrangular") tessellated->AddFacet(quadrangularRead(child));
   }

   tessellated->SetSolidClosed(true);
}

void G4GDMLSolids::tetRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String vertex1;
   G4String vertex2;
   G4String vertex3;
   G4String vertex4;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name   ") name    = attribute_value; else
      if (attribute_name=="vertex1") vertex1 = attribute_value; else
      if (attribute_name=="vertex2") vertex2 = attribute_value; else
      if (attribute_name=="vertex3") vertex3 = attribute_value; else
      if (attribute_name=="vertex4") vertex4 = attribute_value;
   }
   
   const G4ThreeVector* ptr1 = define.getPosition(nameProcess(vertex1));
   const G4ThreeVector* ptr2 = define.getPosition(nameProcess(vertex2));
   const G4ThreeVector* ptr3 = define.getPosition(nameProcess(vertex3));
   const G4ThreeVector* ptr4 = define.getPosition(nameProcess(vertex4));

   new G4Tet(nameProcess(name),*ptr1,*ptr2,*ptr3,*ptr4);
}

void G4GDMLSolids::torusRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String rmin;
   G4String rmax;
   G4String rtor;
   G4String startphi;
   G4String deltaphi;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"    ) name     = attribute_value; else
      if (attribute_name=="lunit"   ) lunit    = attribute_value; else
      if (attribute_name=="aunit"   ) aunit    = attribute_value; else
      if (attribute_name=="rmin"    ) rmin     = attribute_value; else
      if (attribute_name=="rmax"    ) rmax     = attribute_value; else
      if (attribute_name=="rtor"    ) rtor     = attribute_value; else
      if (attribute_name=="startphi") startphi = attribute_value; else
      if (attribute_name=="deltaphi") deltaphi = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _rmin     = evaluator->Evaluate(rmin    )*_lunit;
   G4double _rmax     = evaluator->Evaluate(rmax    )*_lunit;
   G4double _rtor     = evaluator->Evaluate(rtor    )*_lunit;
   G4double _startphi = evaluator->Evaluate(startphi)*_aunit;
   G4double _deltaphi = evaluator->Evaluate(deltaphi)*_aunit;

   new G4Torus(nameProcess(name),_rmin,_rmax,_rtor,_startphi,_deltaphi);
}

void G4GDMLSolids::trapRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String z;
   G4String theta;
   G4String phi;
   G4String y1;
   G4String x1;
   G4String x2;
   G4String alpha1;
   G4String y2;
   G4String x3;
   G4String x4;
   G4String alpha2;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"  ) name   = attribute_value; else
      if (attribute_name=="lunit" ) lunit  = attribute_value; else
      if (attribute_name=="aunit" ) aunit  = attribute_value; else
      if (attribute_name=="z"     ) z      = attribute_value; else
      if (attribute_name=="theta" ) theta  = attribute_value; else
      if (attribute_name=="phi"   ) phi    = attribute_value; else
      if (attribute_name=="y1"    ) y1     = attribute_value; else
      if (attribute_name=="x1"    ) x1     = attribute_value; else
      if (attribute_name=="x2"    ) x2     = attribute_value; else
      if (attribute_name=="alpha1") alpha1 = attribute_value; else
      if (attribute_name=="y2"    ) y2     = attribute_value; else
      if (attribute_name=="x3"    ) x3     = attribute_value; else
      if (attribute_name=="x4"    ) x4     = attribute_value; else
      if (attribute_name=="alpha2") alpha2 = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _z      = evaluator->Evaluate(z     )*_lunit;
   G4double _theta  = evaluator->Evaluate(theta )*_aunit;
   G4double _phi    = evaluator->Evaluate(phi   )*_aunit;
   G4double _y1     = evaluator->Evaluate(y1    )*_lunit;
   G4double _x1     = evaluator->Evaluate(x1    )*_lunit;
   G4double _x2     = evaluator->Evaluate(x2    )*_lunit;
   G4double _alpha1 = evaluator->Evaluate(alpha1)*_aunit;
   G4double _y2     = evaluator->Evaluate(y2    )*_lunit;
   G4double _x3     = evaluator->Evaluate(x3    )*_lunit;
   G4double _x4     = evaluator->Evaluate(x4    )*_lunit;
   G4double _alpha2 = evaluator->Evaluate(alpha2)*_aunit;

   _z  *= 0.5;
   _y1 *= 0.5;
   _x1 *= 0.5;
   _x2 *= 0.5;
   _y2 *= 0.5;
   _x3 *= 0.5;
   _x4 *= 0.5;

   new G4Trap(nameProcess(name),_z,_theta,_phi,_y1,_x1,_x2,_alpha1,_y2,_x3,_x4,_alpha2);
}

void G4GDMLSolids::trdRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String x1;
   G4String x2;
   G4String y1;
   G4String y2;
   G4String z;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"  ) { name   = attribute_value; } else
      if (attribute_name=="lunit" ) { lunit  = attribute_value; } else
      if (attribute_name=="x1"    ) { x1     = attribute_value; } else
      if (attribute_name=="x2"    ) { x2     = attribute_value; } else
      if (attribute_name=="y1"    ) { y1     = attribute_value; } else
      if (attribute_name=="y2"    ) { y2     = attribute_value; } else
      if (attribute_name=="z"     ) { z      = attribute_value; }
   }

   G4double _lunit = evaluator->Evaluate(lunit);

   G4double _x1 = evaluator->Evaluate(x1)*_lunit;
   G4double _x2 = evaluator->Evaluate(x2)*_lunit;
   G4double _y1 = evaluator->Evaluate(y1)*_lunit;
   G4double _y2 = evaluator->Evaluate(y2)*_lunit;
   G4double _z  = evaluator->Evaluate(z )*_lunit;

   _x1 *= 0.5;
   _x2 *= 0.5;
   _y1 *= 0.5;
   _y2 *= 0.5;
   _z  *= 0.5;

   new G4Trd(nameProcess(name),_x1,_x2,_y1,_y2,_z);
}

G4TriangularFacet* G4GDMLSolids::triangularRead(const xercesc::DOMElement* const element) {

   G4String v1;
   G4String v2;
   G4String v3;
   G4String type;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="v1"  ) v1   = attribute_value; else
      if (attribute_name=="v2"  ) v2   = attribute_value; else
      if (attribute_name=="v3"  ) v3   = attribute_value; else
      if (attribute_name=="type") type = attribute_value;
   }

   const G4ThreeVector* ptr1 = define.getPosition(nameProcess(v1));
   const G4ThreeVector* ptr2 = define.getPosition(nameProcess(v2));
   const G4ThreeVector* ptr3 = define.getPosition(nameProcess(v3));

   return new G4TriangularFacet(*ptr1,*ptr2,*ptr3,(type=="RELATIVE")?(RELATIVE):(ABSOLUTE));
}

void G4GDMLSolids::tubeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;
   G4String aunit;
   G4String rmin;
   G4String rmax;
   G4String z;
   G4String startphi;
   G4String deltaphi;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name"    ) { name     = attribute_value; } else
      if (attribute_name=="lunit"   ) { lunit    = attribute_value; } else
      if (attribute_name=="aunit"   ) { aunit    = attribute_value; } else
      if (attribute_name=="rmin"    ) { rmin     = attribute_value; } else
      if (attribute_name=="rmax"    ) { rmax     = attribute_value; } else
      if (attribute_name=="z"       ) { z        = attribute_value; } else
      if (attribute_name=="startphi") { startphi = attribute_value; } else
      if (attribute_name=="deltaphi") { deltaphi = attribute_value; }
   }

   G4double _lunit = evaluator->Evaluate(lunit);
   G4double _aunit = evaluator->Evaluate(aunit);

   G4double _rmin     = evaluator->Evaluate(rmin    )*_lunit;
   G4double _rmax     = evaluator->Evaluate(rmax    )*_lunit;
   G4double _z        = evaluator->Evaluate(z       )*_lunit;
   G4double _startphi = evaluator->Evaluate(startphi)*_aunit;
   G4double _deltaphi = evaluator->Evaluate(deltaphi)*_aunit;

   _z *= 0.5;

   new G4Tubs(nameProcess(name),_rmin,_rmax,_z,_startphi,_deltaphi);
}

G4TwoVector G4GDMLSolids::twoDimVertexRead(const xercesc::DOMElement* const element,G4double _lunit) {

   G4String x;
   G4String y;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="x") x = attribute_value; else
      if (attribute_name=="y") y = attribute_value;
   }

   G4double _x = evaluator->Evaluate(x)*_lunit;
   G4double _y = evaluator->Evaluate(y)*_lunit;

   return G4TwoVector(_x,_y);
}

void G4GDMLSolids::xtruRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4String lunit;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="name" ) name  = attribute_value; else
      if (attribute_name=="lunit") lunit = attribute_value;
   }

   G4double _lunit = evaluator->Evaluate(lunit);

   std::vector<G4TwoVector> twoDimVertexList;
   std::vector<G4ExtrudedSolid::ZSection> sectionList;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="twoDimVertex") twoDimVertexList.push_back(twoDimVertexRead(child,_lunit)); else
      if (tag=="section") sectionList.push_back(sectionRead(child,_lunit));      
   }

   new G4ExtrudedSolid(nameProcess(name),twoDimVertexList,sectionList);
}

G4GDMLSolids::zplaneType G4GDMLSolids::zplaneRead(const xercesc::DOMElement* const element,G4double _lunit) {

   G4String rmin;
   G4String rmax;
   G4String z;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attribute_name  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attribute_value = xercesc::XMLString::transcode(attribute->getValue());

      if (attribute_name=="rmin" ) rmin  = attribute_value; else
      if (attribute_name=="rmax" ) rmax  = attribute_value; else
      if (attribute_name=="z"    ) z     = attribute_value;
   }

   zplaneType zplane;

   zplane.rmin = evaluator->Evaluate(rmin)*_lunit;
   zplane.rmax = evaluator->Evaluate(rmax)*_lunit;
   zplane.z    = evaluator->Evaluate(z   )*_lunit;

   return zplane;
}

void G4GDMLSolids::Read(const xercesc::DOMElement* const element,G4GDMLEvaluator *evalPtr,const G4String& file0) {

   evaluator = evalPtr;
   
   file = file0;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="box"           ) boxRead           (child); else
      if (tag=="cone"          ) coneRead          (child); else
      if (tag=="ellipsoid"     ) ellipsoidRead     (child); else
      if (tag=="eltube"        ) eltubeRead        (child); else
      if (tag=="hype"          ) hypeRead          (child); else
      if (tag=="loop"          ) loopRead          (child); else
      if (tag=="orb"           ) orbRead           (child); else
      if (tag=="para"          ) paraRead          (child); else
      if (tag=="polycone"      ) polyconeRead      (child); else
      if (tag=="polyhedra"     ) polyhedraRead     (child); else
      if (tag=="sphere"        ) sphereRead        (child); else
      if (tag=="reflectedSolid") reflectedSolidRead(child); else
      if (tag=="tessellated"   ) tessellatedRead   (child); else
      if (tag=="tet"           ) tetRead           (child); else
      if (tag=="torus"         ) torusRead         (child); else
      if (tag=="trap"          ) trapRead          (child); else
      if (tag=="trd"           ) trdRead           (child); else
      if (tag=="tube"          ) tubeRead          (child); else
      if (tag=="xtru"          ) xtruRead          (child); else
      if (tag=="intersection"  ) booleanRead(child,INTERSECTION); else
      if (tag=="subtraction"   ) booleanRead(child,SUBTRACTION ); else
      if (tag=="union"         ) booleanRead(child,UNION       ); else
      G4Exception("GDML: Unknown tag in solids: "+tag);
   }
}

G4VSolid* G4GDMLSolids::getSolid(const G4String& ref) const {

   G4VSolid *solidPtr = G4SolidStore::GetInstance()->GetSolid(ref,false);

   if (!solidPtr) G4Exception("GDML: Referenced solid '"+ref+"' was not found!");

   return solidPtr;
}
