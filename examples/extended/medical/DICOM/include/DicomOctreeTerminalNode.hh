#ifndef DicomOctreeTerminalNode_h
#define DicomOctreeTerminalNode_h

//class Octree;
class  DicomOctreeTerminalNode : public DicomOctreeNode
{
public:

   DicomOctreeTerminalNode(DicomOctreeNode* pParent );
  ~ DicomOctreeTerminalNode();

public:
  DicomOctreeNode*& operator []( G4int index );
  G4int MemSize();

  OctreeNodeType Type()                      {return TERMINAL_NODE;}
  G4int FindChild( const DicomOctreeNode* pNode ) {return -1;}

private:
  static DicomOctreeNode* mNull;
};
#endif
