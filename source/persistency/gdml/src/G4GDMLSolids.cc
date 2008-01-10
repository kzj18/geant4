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
// $Id: G4GDMLSolids.cc,v 1.33 2008-01-10 12:12:32 ztorzsok Exp $
// GEANT4 tag $ Name:$
//
// class G4GDMLSolids Implementation
//
// Original author: Zoltan Torzsok, November 2007
//
// --------------------------------------------------------------------

#include "G4GDMLSolids.hh"

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

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue);
   }

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="first") first = refRead(child); else
      if (tag=="second") second = refRead(child); else
      if (tag=="position") position = vectorRead(child); else
      if (tag=="rotation") rotation = vectorRead(child); else
      G4Exception("GDML: Unknown tag in boolean solid: "+tag);
   }

   G4RotationMatrix rot;

   rot.rotateX(rotation.x());
   rot.rotateY(rotation.y());
   rot.rotateZ(rotation.z());

   G4Transform3D transform(rot,position);

   G4VSolid* firstSolid = getSolid(GenerateName(first));
   G4VSolid* secondSolid = getSolid(GenerateName(second));

   if (op==UNION) new G4UnionSolid(name,firstSolid,secondSolid,transform); else
   if (op==SUBTRACTION) new G4SubtractionSolid(name,firstSolid,secondSolid,transform); else
   if (op==INTERSECTION) new G4IntersectionSolid(name,firstSolid,secondSolid,transform);
}

void G4GDMLSolids::boxRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double PhiTwist = 0.0;
   G4double x = 0.0;
   G4double y = 0.0;
   G4double z = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="PhiTwist") PhiTwist = eval.Evaluate(attValue); else
      if (attName=="x") x = eval.Evaluate(attValue); else
      if (attName=="y") y = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue);
   }

   PhiTwist *= aunit;
   x *= 0.5*lunit;
   y *= 0.5*lunit;
   z *= 0.5*lunit;

   if (PhiTwist==0.0) new G4Box(name,x,y,z);
   else new G4TwistedBox(name,PhiTwist,x,y,z);
}

void G4GDMLSolids::coneRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double rmin1 = 0.0;
   G4double rmax1 = 0.0;
   G4double rmin2 = 0.0;
   G4double rmax2 = 0.0;
   G4double z = 0.0;
   G4double startphi = 0.0;
   G4double deltaphi = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="rmin1") rmin1 = eval.Evaluate(attValue); else
      if (attName=="rmax1") rmax1 = eval.Evaluate(attValue); else
      if (attName=="rmin2") rmin2 = eval.Evaluate(attValue); else
      if (attName=="rmax2") rmax2 = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue); else
      if (attName=="startphi") startphi = eval.Evaluate(attValue); else
      if (attName=="deltaphi") deltaphi = eval.Evaluate(attValue);
   }

   rmin1 *= lunit;
   rmax1 *= lunit;
   rmin2 *= lunit;
   rmax2 *= lunit;
   z *= 0.5*lunit;
   startphi *= aunit;
   deltaphi *= aunit;

   new G4Cons(name,rmin1,rmax1,rmin2,rmax2,z,startphi,deltaphi);
}

void G4GDMLSolids::ellipsoidRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double ax = 0.0;
   G4double by = 0.0;
   G4double cz = 0.0;
   G4double zcut1 = 0.0;
   G4double zcut2 = 0.0; 

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name  = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="ax") ax = eval.Evaluate(attValue); else
      if (attName=="by") by = eval.Evaluate(attValue); else
      if (attName=="cz") cz = eval.Evaluate(attValue); else
      if (attName=="zcut1") zcut1 = eval.Evaluate(attValue); else
      if (attName=="zcut2") zcut2 = eval.Evaluate(attValue);
   }

   ax *= lunit;
   by *= lunit;
   cz *= lunit;
   zcut1 *= lunit;
   zcut2 *= lunit; 

   new G4Ellipsoid(name,ax,by,cz,zcut1,zcut2);
}

void G4GDMLSolids::eltubeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double dx = 0.0;
   G4double dy = 0.0;
   G4double dz = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="dx") dx = eval.Evaluate(attValue); else
      if (attName=="dy") dy = eval.Evaluate(attValue); else
      if (attName=="dz") dz = eval.Evaluate(attValue);
   }

   dx *= lunit;
   dy *= lunit;
   dz *= lunit;

   new G4EllipticalTube(name,dx,dy,dz);
}

void G4GDMLSolids::hypeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double rmin = 0.0;
   G4double rmax = 0.0;
   G4double inst = 0.0;
   G4double outst = 0.0;
   G4double z = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="rmin") rmin = eval.Evaluate(attValue); else
      if (attName=="rmax") rmax = eval.Evaluate(attValue); else
      if (attName=="inst") inst = eval.Evaluate(attValue); else
      if (attName=="outst") outst = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue);
   }

   rmin *= lunit;
   rmax *= lunit;
   inst *= aunit;
   outst *= aunit;
   z *= 0.5*lunit;

   new G4Hype(name,rmin,rmax,inst,outst,z);
}

void G4GDMLSolids::orbRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double r = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="r") r = eval.Evaluate(attValue);
   }

   r *= lunit;

   new G4Orb(name,r);
}

void G4GDMLSolids::paraRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double x = 0.0;
   G4double y = 0.0;
   G4double z = 0.0;
   G4double alpha = 0.0;
   G4double theta = 0.0;
   G4double phi = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="x") x = eval.Evaluate(attValue); else
      if (attName=="y") y = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue); else
      if (attName=="alpha") alpha = eval.Evaluate(attValue); else
      if (attName=="theta") theta = eval.Evaluate(attValue); else
      if (attName=="phi") phi = eval.Evaluate(attValue);
   }

   x = 0.5*lunit;
   y = 0.5*lunit;
   z = 0.5*lunit;
   alpha = aunit;
   theta = aunit;
   phi = aunit;

   new G4Para(name,x,y,z,alpha,theta,phi);
}

void G4GDMLSolids::polyconeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double startphi = 0.0;
   G4double deltaphi = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="startphi") startphi = eval.Evaluate(attValue); else
      if (attName=="deltaphi") deltaphi = eval.Evaluate(attValue);
   }

   startphi *= aunit;
   deltaphi *= aunit;

   std::vector<zplaneType> zplaneList;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="zplane") zplaneList.push_back(zplaneRead(child));
   }

   G4int numZPlanes = zplaneList.size();

   G4double* rmin_array = new G4double[numZPlanes];
   G4double* rmax_array = new G4double[numZPlanes];
   G4double* z_array    = new G4double[numZPlanes];

   for (G4int i=0;i<numZPlanes;i++) {
   
      rmin_array[i] = zplaneList[i].rmin*lunit;
      rmax_array[i] = zplaneList[i].rmax*lunit;
      z_array[i]    = zplaneList[i].z*lunit;
   }

   new G4Polycone(name,startphi,deltaphi,numZPlanes,z_array,rmin_array,rmax_array);
}

void G4GDMLSolids::polyhedraRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double startphi = 0.0;
   G4double deltaphi = 0.0;
   G4int numsides = 0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="startphi") startphi = eval.Evaluate(attValue); else
      if (attName=="deltaphi") deltaphi = eval.Evaluate(attValue); else
      if (attName=="numsides") numsides = eval.EvaluateInteger(attValue);
   }

   startphi *= aunit;
   deltaphi *= aunit;

   std::vector<zplaneType> zplaneList;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="zplane") zplaneList.push_back(zplaneRead(child));
   }

   G4int numZPlanes = zplaneList.size();

   G4double* rmin_array = new G4double[numZPlanes];
   G4double* rmax_array = new G4double[numZPlanes];
   G4double* z_array = new G4double[numZPlanes];

   for (G4int i=0;i<numZPlanes;i++) {
   
      rmin_array[i] = zplaneList[i].rmin*lunit;
      rmax_array[i] = zplaneList[i].rmax*lunit;
      z_array[i] = zplaneList[i].z*lunit;
   }

   new G4Polyhedra(name,startphi,deltaphi,numsides,numZPlanes,z_array,rmin_array,rmax_array);
}

G4QuadrangularFacet* G4GDMLSolids::quadrangularRead(const xercesc::DOMElement* const element) {

   G4ThreeVector* ptr1 = 0;
   G4ThreeVector* ptr2 = 0;
   G4ThreeVector* ptr3 = 0;
   G4ThreeVector* ptr4 = 0;

   G4String type;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="vertex1") ptr1 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex2") ptr2 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex3") ptr3 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex4") ptr4 = getPosition(GenerateName(attValue)); else
      if (attName=="type") type = attValue;
   }

   return new G4QuadrangularFacet(*ptr1,*ptr2,*ptr3,*ptr4,(type=="RELATIVE")?(RELATIVE):(ABSOLUTE));
}

void G4GDMLSolids::reflectedSolidRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4String solid;
   G4double sx = 1.0;
   G4double sy = 1.0;
   G4double sz = 1.0;
   G4double rx = 0.0;
   G4double ry = 0.0;
   G4double rz = 0.0;
   G4double dx = 0.0;
   G4double dy = 0.0;
   G4double dz = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="solid") solid = GenerateName(attValue); else
      if (attName=="sx") sx = eval.Evaluate(attValue); else
      if (attName=="sy") sy = eval.Evaluate(attValue); else
      if (attName=="sz") sz = eval.Evaluate(attValue); else
      if (attName=="rx") rx = eval.Evaluate(attValue); else
      if (attName=="ry") ry = eval.Evaluate(attValue); else
      if (attName=="rz") rz = eval.Evaluate(attValue); else
      if (attName=="dx") dx = eval.Evaluate(attValue); else
      if (attName=="dy") dy = eval.Evaluate(attValue); else
      if (attName=="dz") dz = eval.Evaluate(attValue);
   }

   rx *= aunit;
   ry *= aunit;
   rz *= aunit;
   dx *= lunit;
   dy *= lunit;
   dz *= lunit;

   G4RotationMatrix rot;
   
   rot.rotateX(rx);
   rot.rotateY(ry);
   rot.rotateZ(rz);

   G4ThreeVector trans(dx,dy,dz);

   G4Scale3D scale(sx,sy,sz);

   G4Transform3D transform(rot,trans);
   transform = transform*scale;
          
   new G4ReflectedSolid(name,getSolid(solid),transform);
}

G4ExtrudedSolid::ZSection G4GDMLSolids::sectionRead(const xercesc::DOMElement* const element,G4double lunit) {

   G4double zPosition = 0.0;
   G4TwoVector Offset;
   G4double scalingFactor = 1.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="zPosition") zPosition = eval.Evaluate(attValue)*lunit; else
      if (attName=="xOffset") Offset.setX(eval.Evaluate(attValue)*lunit); else
      if (attName=="yOffset") Offset.setY(eval.Evaluate(attValue)*lunit); else
      if (attName=="scalingFactor") scalingFactor = eval.Evaluate(attValue);
   }

   return G4ExtrudedSolid::ZSection(zPosition,Offset,scalingFactor);
}

void G4GDMLSolids::sphereRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double rmin = 0.0;
   G4double rmax = 0.0;
   G4double startphi = 0.0;
   G4double deltaphi = 0.0;
   G4double starttheta = 0.0;
   G4double deltatheta = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="rmin") rmin = eval.Evaluate(attValue); else
      if (attName=="rmax") rmax = eval.Evaluate(attValue); else
      if (attName=="startphi") startphi = eval.Evaluate(attValue); else
      if (attName=="deltaphi") deltaphi = eval.Evaluate(attValue); else
      if (attName=="starttheta") starttheta = eval.Evaluate(attValue); else
      if (attName=="deltatheta") deltatheta = eval.Evaluate(attValue);
   }

   rmin *= lunit;
   rmax *= lunit;
   startphi *= aunit;
   deltaphi *= aunit;
   starttheta *= aunit;
   deltatheta *= aunit;

   new G4Sphere(name,rmin,rmax,startphi,deltaphi,starttheta,deltatheta);
}

void G4GDMLSolids::tessellatedRead(const xercesc::DOMElement* const element) {

   G4String name;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue);
   }
   
   G4TessellatedSolid *tessellated = new G4TessellatedSolid(name);

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
   
   G4ThreeVector* ptr1 = 0;
   G4ThreeVector* ptr2 = 0;
   G4ThreeVector* ptr3 = 0;
   G4ThreeVector* ptr4 = 0;
   
   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="vertex1") ptr1 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex2") ptr2 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex3") ptr3 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex4") ptr4 = getPosition(GenerateName(attValue));
   }

   new G4Tet(name,*ptr1,*ptr2,*ptr3,*ptr4);
}

void G4GDMLSolids::torusRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double rmin = 0.0;
   G4double rmax = 0.0;
   G4double rtor = 0.0;
   G4double startphi = 0.0;
   G4double deltaphi = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="rmin") rmin = eval.Evaluate(attValue); else
      if (attName=="rmax") rmax = eval.Evaluate(attValue); else
      if (attName=="rtor") rtor = eval.Evaluate(attValue); else
      if (attName=="startphi") startphi = eval.Evaluate(attValue); else
      if (attName=="deltaphi") deltaphi = eval.Evaluate(attValue);
   }

   rmin *= lunit;
   rmax *= lunit;
   rtor *= lunit;
   startphi *= aunit;
   deltaphi *= aunit;

   new G4Torus(name,rmin,rmax,rtor,startphi,deltaphi);
}

void G4GDMLSolids::trapRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double PhiTwist = 0.0;
   G4double z = 0.0;
   G4double theta = 0.0;
   G4double phi = 0.0;
   G4double y1 = 0.0;
   G4double x1 = 0.0;
   G4double x2 = 0.0;
   G4double alpha1 = 0.0;
   G4double y2 = 0.0;
   G4double x3 = 0.0;
   G4double x4 = 0.0;
   G4double alpha2 = 0.0;
   G4double Alph = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="PhiTwist") PhiTwist = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue); else
      if (attName=="theta") theta = eval.Evaluate(attValue); else
      if (attName=="phi") phi = eval.Evaluate(attValue); else
      if (attName=="y1") y1 = eval.Evaluate(attValue); else
      if (attName=="x1") x1 = eval.Evaluate(attValue); else
      if (attName=="x2") x2 = eval.Evaluate(attValue); else
      if (attName=="alpha1") alpha1 = eval.Evaluate(attValue); else
      if (attName=="y2") y2 = eval.Evaluate(attValue); else
      if (attName=="x3") x3 = eval.Evaluate(attValue); else
      if (attName=="x4") x4 = eval.Evaluate(attValue); else
      if (attName=="alpha2") alpha2 = eval.Evaluate(attValue);
      if (attName=="Alph") Alph = eval.Evaluate(attValue);
   }

   PhiTwist *= aunit;
   z *= 0.5*lunit;
   theta *= aunit;
   phi *= aunit;
   y1 *= 0.5*lunit;
   x1 *= 0.5*lunit;
   x2 *= 0.5*lunit;
   alpha1 *= aunit;
   y2 *= 0.5*lunit;
   x3 *= 0.5*lunit;
   x4 *= 0.5*lunit;
   alpha2 *= aunit;

   if (PhiTwist == 0.0) new G4Trap(name,z,theta,phi,y1,x1,x2,alpha1,y2,x3,x4,alpha2);
   else new G4TwistedTrap(name,PhiTwist,z,theta,phi,y1,x1,x2,y2,x3,x4,Alph);
}

void G4GDMLSolids::trdRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double x1 = 0.0;
   G4double x2 = 0.0;
   G4double y1 = 0.0;
   G4double y2 = 0.0;
   G4double z = 0.0;
   G4double PhiTwist = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="x1") x1 = eval.Evaluate(attValue); else
      if (attName=="x2") x2 = eval.Evaluate(attValue); else
      if (attName=="y1") y1 = eval.Evaluate(attValue); else
      if (attName=="y2") y2 = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue); else
      if (attName=="PhiTwist") PhiTwist = eval.Evaluate(attValue);
   }

   x1 *= 0.5*lunit;
   x2 *= 0.5*lunit;
   y1 *= 0.5*lunit;
   y2 *= 0.5*lunit;
   z *= 0.5*lunit;
   PhiTwist *= aunit;

   if (PhiTwist==0.0) new G4Trd(name,x1,x2,y1,y2,z);
   else new G4TwistedTrd(name,x1,x2,y1,y2,z,PhiTwist);
}

G4TriangularFacet* G4GDMLSolids::triangularRead(const xercesc::DOMElement* const element) {

   G4ThreeVector* ptr1 = 0;
   G4ThreeVector* ptr2 = 0;
   G4ThreeVector* ptr3 = 0;

   G4String type;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="vertex1") ptr1 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex2") ptr2 = getPosition(GenerateName(attValue)); else
      if (attName=="vertex3") ptr3 = getPosition(GenerateName(attValue)); else
      if (attName=="type") type = attValue;
   }

   return new G4TriangularFacet(*ptr1,*ptr2,*ptr3,(type=="RELATIVE")?(RELATIVE):(ABSOLUTE));
}

void G4GDMLSolids::tubeRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;
   G4double aunit = 1.0;
   G4double rmin = 0.0;
   G4double rmax = 0.0;
   G4double z = 0.0;
   G4double startphi = 0.0;
   G4double deltaphi = 0.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue); else
      if (attName=="aunit") aunit = eval.Evaluate(attValue); else
      if (attName=="rmin") rmin = eval.Evaluate(attValue); else
      if (attName=="rmax") rmax = eval.Evaluate(attValue); else
      if (attName=="z") z = eval.Evaluate(attValue); else
      if (attName=="startphi") startphi = eval.Evaluate(attValue); else
      if (attName=="deltaphi") deltaphi = eval.Evaluate(attValue);
   }

   rmin *= lunit;
   rmax *= lunit;
   z *= 0.5*lunit;
   startphi *= aunit;
   deltaphi *= aunit;

   new G4Tubs(name,rmin,rmax,z,startphi,deltaphi);
}

G4TwoVector G4GDMLSolids::twoDimVertexRead(const xercesc::DOMElement* const element,G4double lunit) {

   G4TwoVector vec;
   
   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="x") vec.setX(eval.Evaluate(attValue)*lunit); else
      if (attName=="y") vec.setY(eval.Evaluate(attValue)*lunit);
   }

   return vec;
}

void G4GDMLSolids::xtruRead(const xercesc::DOMElement* const element) {

   G4String name;
   G4double lunit = 1.0;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = GenerateName(attValue); else
      if (attName=="lunit") lunit = eval.Evaluate(attValue);
   }

   std::vector<G4TwoVector> twoDimVertexList;
   std::vector<G4ExtrudedSolid::ZSection> sectionList;

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="twoDimVertex") twoDimVertexList.push_back(twoDimVertexRead(child,lunit)); else
      if (tag=="section") sectionList.push_back(sectionRead(child,lunit));      
   }

   new G4ExtrudedSolid(name,twoDimVertexList,sectionList);
}

G4GDMLSolids::zplaneType G4GDMLSolids::zplaneRead(const xercesc::DOMElement* const element) {

   zplaneType zplane;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* node = attributes->item(attribute_index);

      if (node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="rmin") zplane.rmin = eval.Evaluate(attValue); else
      if (attName=="rmax") zplane.rmax = eval.Evaluate(attValue); else
      if (attName=="z") zplane.z = eval.Evaluate(attValue);
   }

   return zplane;
}

void G4GDMLSolids::solidsRead(const xercesc::DOMElement* const element) {

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag=="box") boxRead(child); else
      if (tag=="cone") coneRead(child); else
      if (tag=="ellipsoid") ellipsoidRead(child); else
      if (tag=="eltube") eltubeRead(child); else
      if (tag=="hype") hypeRead(child); else
      if (tag=="orb") orbRead(child); else
      if (tag=="para") paraRead(child); else
      if (tag=="polycone") polyconeRead(child); else
      if (tag=="polyhedra") polyhedraRead(child); else
      if (tag=="sphere") sphereRead(child); else
      if (tag=="reflectedSolid") reflectedSolidRead(child); else
      if (tag=="tessellated") tessellatedRead(child); else
      if (tag=="tet") tetRead(child); else
      if (tag=="torus") torusRead(child); else
      if (tag=="trap") trapRead(child); else
      if (tag=="trd") trdRead(child); else
      if (tag=="tube") tubeRead(child); else
      if (tag=="twistedbox") boxRead(child); else
      if (tag=="twistedtrap") trapRead(child); else
      if (tag=="twistedtrd") trdRead(child); else
      if (tag=="xtru") xtruRead(child); else
      if (tag=="intersection") booleanRead(child,INTERSECTION); else
      if (tag=="subtraction") booleanRead(child,SUBTRACTION); else
      if (tag=="union") booleanRead(child,UNION); else
      if (tag=="loop") loopRead(child,&G4GDMLBase::solidsRead); else
      G4Exception("GDML: Unknown tag in solids: "+tag);
   }
}

G4ThreeVector G4GDMLSolids::vectorRead(const xercesc::DOMElement* const element) {

   G4double unit = 1.0;

   G4ThreeVector vec;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="unit") unit = eval.Evaluate(attValue); else
      if (attName=="x") vec.setX(eval.Evaluate(attValue)); else
      if (attName=="y") vec.setY(eval.Evaluate(attValue)); else
      if (attName=="z") vec.setZ(eval.Evaluate(attValue));
   }

   return vec*unit;
}

G4String G4GDMLSolids::refRead(const xercesc::DOMElement* const element) {

   G4String ref;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="ref") ref = attValue;
   }

   return ref;
}

G4VSolid* G4GDMLSolids::getSolid(const G4String& ref) const {

   G4VSolid* solidPtr = G4SolidStore::GetInstance()->GetSolid(ref,false);

   if (!solidPtr) G4Exception("GDML: Referenced solid '"+ref+"' was not found!");

   return solidPtr;
}
