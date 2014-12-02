/*
 * G4ITTrackHolder.hh
 *
 *  Created on: 17 sept. 2014
 *      Author: kara
 */

#ifndef G4ITTRACKHOLDER_HH
#define G4ITTRACKHOLDER_HH

#include "G4IT.hh"
#include "G4TrackList.hh"
#include "G4VITTrackHolder.hh"
#include <iostream>

class PriorityList : public G4TrackList::Watcher
{
public:
  enum Type
  {
    MainList = 0,
    SecondariesList = 1,
    WaitingList = 2,
    Undefined = -1
  };

  PriorityList();
  PriorityList(const PriorityList& right);
  virtual ~PriorityList();

  virtual void NotifyDeletingList(G4TrackList* __list);

  void NewMainList(G4TrackList* __list, G4TrackManyList& allMainList);

  G4TrackList* NewMainList(G4TrackManyList& allMainList);

  void PushToMainList(G4Track* __track, G4TrackManyList& allMainList);

  void TransferToMainList(G4TrackList*& __list, G4TrackManyList& allMainList);

  void PushToListOfSecondaries(G4Track* __track,
                               G4TrackManyList& listOfAllSecondaries);

  void PushToWaitingList(G4Track* __track);

  void TransferSecondariesToMainList();

  void PushToMainList(G4Track* track);

  void MergeWithMainList(G4TrackList* trackList);

  inline G4TrackList* GetMainList()
  {
    return fpMainList;
  }

  inline G4TrackList* GetSecondariesList()
  {
    return &fSecondaries;
  }

  inline void SetWaitingList(G4TrackList* __list)
  {
    fpWaitingList = __list;
  }

  inline G4TrackList* Get(Type type)
  {
    switch (type)
    {
      case MainList:
        return fpMainList;
        break;
      case SecondariesList:
        return &fSecondaries;
        break;
      case WaitingList:
        return fpWaitingList;
        break;
      case Undefined:
        return 0;
    }
    return 0;
  }

  int GetNTracks();

private:
  G4TrackList* fpMainList;
  G4TrackList fSecondaries;
  // to merge with fpMainList
  G4TrackList* fpWaitingList;
  // Waiting queue of currentList
};

class G4ITTrackHolder : public G4VITTrackHolder
{
  /* UR:
   * Push on time
   * Push delayed
   * Exception when going back
   * Get all tracks
   */

  static G4ThreadLocal G4ITTrackHolder* fgInstance;
  static G4ITTrackHolder* fgMasterInstance;
  friend class G4Scheduler;

public:
  //----- typedefs -----
  typedef int Key; //TODO
  typedef std::map<Key, PriorityList*> MapOfPriorityLists;
  typedef std::map<double, std::map<Key, G4TrackList*> > MapOfDelayedLists;

  //----- Access singletons + constructors/destructors-----

  static G4ITTrackHolder* Instance();
  static G4ITTrackHolder* MasterInstance();

  G4ITTrackHolder();
  virtual
  ~G4ITTrackHolder();

  //----- Time of the next set of tracks -----
  inline double GetNextTime()
  {
    if (fDelayedList.empty()) return DBL_MAX;
    return fDelayedList.begin()->first;
  }

  //----- Add new tracks to the list -----
  virtual void Push(G4Track*);
  static void PushToMaster(G4Track*);

  //----- Operations between lists -----

  inline void PushToKill(G4Track* track)
  {
    fToBeKilledList.push_back(track);
  }

  bool MergeNextTimeToMainList(double& time);
  void MergeSecondariesWithMainList();
  void MoveMainToWaitingList();

  // ----- To call at the end of the step -----
  void KillTracks();
  void Clear();

  // ----- Add a watcher to a specific track list -----
  // comment: to stop watching, just call StopWatching from your watcher class
  bool AddWatcher(int,
      G4TrackList::Watcher*,
      PriorityList::Type = PriorityList::MainList);

  // ----- Access track lists -----
  inline MapOfPriorityLists& GetLists()
  { return fLists;}
  PriorityList* GetPriorityList(Key);
  G4TrackList* GetMainList(Key);
  inline G4TrackManyList* GetMainList()
  {
    return &fAllMainList;
  }

  inline G4TrackManyList* GetSecondariesList()
  {
    return &fAllSecondariesList;
  }

  inline MapOfDelayedLists& GetDelayedLists()
  {
    return fDelayedList;
  }

  virtual size_t GetNTracks();

  // ----- Check track lists are NOT empty -----
  // comment: checking NOT empty faster than checking IS empty
  inline bool MainListsNOTEmpty()
  {
    return CheckMapIsNOTEmpty(fLists, PriorityList::MainList);
  }

  inline bool SecondaryListsNOTEmpty()
  {
    return CheckMapIsNOTEmpty(fLists, PriorityList::SecondariesList);
  }

  bool DelayListsNOTEmpty();

  bool CheckMapIsNOTEmpty(MapOfPriorityLists& mapOfLists,
      PriorityList::Type type);

  inline void SetVerbose(int verbose)
  {
    fVerbose = verbose;
  }

protected:
  void AddTrackID(G4Track* track);
  void _PushTrack(G4Track* track);
  void PushTo(G4Track*, PriorityList::Type);
  void PushDelayed(G4Track* track);

protected:
  std::map<Key, PriorityList*> fLists;
  MapOfDelayedLists fDelayedList;
  G4TrackList fToBeKilledList;
  bool fMainListHaveBeenSet;
  int fVerbose;
  int fNbTracks;

  double fPostActivityGlobalTime;
  //  double fPreActivityGlobalTime ;

  G4TrackManyList fAllMainList;
  G4TrackManyList fAllSecondariesList;
};

#endif /* G4MIMOLECULARTRACKS_HH_ */
