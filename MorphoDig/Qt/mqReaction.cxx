/*=========================================================================

   Program: MorphoDig
   Module:    mqReaction.cxx

   

========================================================================*/
#include "mqReaction.h"
#include "mqCoreUtilities.h"
//-----------------------------------------------------------------------------

mqReaction::mqReaction(QAction* parentObject, Qt::ConnectionType type)
  : Superclass(parentObject)
{
  Q_ASSERT(parentObject != NULL);

  QObject::connect(parentObject, SIGNAL(triggered(bool)), this, SLOT(onTriggered()), type);

  // Deal with master/slave enable/disable
  // In Paraview : pqApplicationCore::instance() is called which is a QObject.
  
 // QObject::connect(mtApplicationCore::instance(), SIGNAL(updateMasterEnableState(bool)), this,
  // SLOT(updateMasterEnableState(bool)));

  this->IsMaster = true;
}

//-----------------------------------------------------------------------------
mqReaction::~mqReaction()
{
}

//-----------------------------------------------------------------------------
void mqReaction::updateMasterEnableState(bool isMaster)
{
  this->IsMaster = isMaster;
  updateEnableState();
}
