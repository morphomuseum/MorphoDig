/*=========================================================================

   Program: MorphoDig
   Module:    mqDisplayReaction.cxx

========================================================================*/
#include "mqDisplayReaction.h"

#include "mqMorphoDigCore.h"



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
mqDisplayReaction::mqDisplayReaction(QAction* parentObject, int _mode)
	: Superclass(parentObject)
{
	this->parentObject = parentObject;
	this->Mode = _mode;
	this->MainWindow = mqMorphoDigCore::instance()->GetMainWindow();

}

void mqDisplayReaction::GridToggle() {

	
	if (mqMorphoDigCore::instance()->Getmui_ShowGrid() == 1)
	{
		this->parentObject->setChecked(false);
		mqMorphoDigCore::instance()->Setmui_ShowGrid(0);
	}
	else
	{
		this->parentObject->setChecked(true);
		mqMorphoDigCore::instance()->Setmui_ShowGrid(1);
	}
	mqMorphoDigCore::instance()->SetGridVisibility();
	mqMorphoDigCore::instance()->SetGridInfos();
}//0
void mqDisplayReaction::OrientationHelperToggle() {
	
	if (mqMorphoDigCore::instance()->Getmui_ShowOrientationHelper() == 1)
	{
		this->parentObject->setChecked(false);
		mqMorphoDigCore::instance()->Setmui_ShowOrientationHelper(0);
	}
	else
	{
		this->parentObject->setChecked(true);
		mqMorphoDigCore::instance()->Setmui_ShowOrientationHelper(1);
	}
	mqMorphoDigCore::instance()->SetOrientationHelperVisibility();

}//1
void mqDisplayReaction::RendererAnaglyphToggle() {
	
	if (mqMorphoDigCore::instance()->Getmui_Anaglyph() == 1)
	{
		this->parentObject->setChecked(false);
		mqMorphoDigCore::instance()->Setmui_Anaglyph(0);
	}
	else
	{
		this->parentObject->setChecked(true);
		mqMorphoDigCore::instance()->Setmui_Anaglyph(1);
	}

}//2
