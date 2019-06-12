/*=========================================================================

   Program: MorphoDig
 
========================================================================*/
#include "mqDisplayControlsWidget.h"
#include "ui_mqDisplayControlsWidget.h"

// For later!

#include "mqMorphoDigCore.h"

#include "mqDisplayReaction.h"
#include <vtkRenderer.h>
#include <QToolButton>


//-----------------------------------------------------------------------------
void mqDisplayControlsWidget::constructor()
{
 // Ui::mqDisplayControlsWidget ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqDisplayControlsWidget;
  this->ui->setupUi(this);
  
   

  this->ui->RendererAnaglyphToggle->setCheckable(true);
  this->ui->OrientationHelperToggle->setCheckable(true);
  this->ui->GridToggle->setCheckable(true);

 


  QAction* gridAction = new QAction(tr("&Grid"), this);
  gridAction->setToolTip(tr("Display grid on/off."));
  gridAction->setCheckable(true);
  this->ui->GridToggle->addAction(gridAction);
  this->ui->GridToggle->setDefaultAction(gridAction);
  QIcon icon;
  icon.addFile(QStringLiteral(":/Icons/grid.png"), QSize(), QIcon::Normal, QIcon::Off);
  //  exportColorMap->setIcon(icon);
  gridAction->setIcon(icon);
  


  new mqDisplayReaction(gridAction, 0); //0 = display Grid Toggle


  QAction* orientationAction = new QAction(tr("&Orientation"), this);
  orientationAction->setToolTip(tr("Display orientation helper on/off."));
  orientationAction->setCheckable(true);
  this->ui->OrientationHelperToggle->addAction(orientationAction);
  this->ui->OrientationHelperToggle->setDefaultAction(orientationAction);
  
  QIcon icon2;
  icon2.addFile(QStringLiteral(":/Icons/orientation_helper.png"), QSize(), QIcon::Normal, QIcon::Off);
  //  exportColorMap->setIcon(icon);
  orientationAction->setIcon(icon2);



  new mqDisplayReaction(orientationAction, 1); //1 = display Orientation Helper Toggle


  QAction* anaglyphAction = new QAction(tr("&Anaglyph"), this);
  anaglyphAction->setToolTip(tr("Toggle renderer : normal or anaglyph (red-blue)."));
  anaglyphAction->setCheckable(true);
  this->ui->RendererAnaglyphToggle->addAction(anaglyphAction);
  this->ui->RendererAnaglyphToggle->setDefaultAction(anaglyphAction);
  QIcon icon3;
  icon3.addFile(QStringLiteral(":/Icons/renderer_anaglyph.png"), QSize(), QIcon::Normal, QIcon::Off);
  //  exportColorMap->setIcon(icon);
  anaglyphAction->setIcon(icon3);

  new mqDisplayReaction(anaglyphAction, 2); //2 = display Anaglyph mode Toggle

  if (mqMorphoDigCore::instance()->Getmui_Anaglyph() == 1)
  {

	  this->ui->RendererAnaglyphToggle->setChecked(true);
  }

  if (mqMorphoDigCore::instance()->Getmui_ShowGrid() == 1)
  {

	  this->ui->GridToggle->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_ShowOrientationHelper() == 1)
  {

	  this->ui->OrientationHelperToggle->setChecked(true);
  }

  connect(this->ui->BackFaceCullingOnOff, SIGNAL(pressed()), this, SLOT(slotBackfaceCullingOnOff()));
  connect(this->ui->ClippingPlaneOnOff, SIGNAL(pressed()), this, SLOT(slotClippingPlaneOnOff()));

  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 1)
  {

	  this->ui->pointNormals->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 0)
  {

	  this->ui->cellNormals->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 1)
  {

	  this->ui->pointNormals->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 2)
  {

	  this->ui->wireframe->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 3)
  {

	  this->ui->points->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 5)
  {

	  this->ui->pointNormals2->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_DisplayMode() == 4)
  {

	  this->ui->cellNormals2->setChecked(true);
  }
  connect(this->ui->cellNormals, SIGNAL(pressed()), this, SLOT(slotDisplayCellNormals()));
  connect(this->ui->pointNormals, SIGNAL(pressed()), this, SLOT(slotDisplayPointNormals())); 
  connect(this->ui->cellNormals2, SIGNAL(pressed()), this, SLOT(slotDisplayCellNormals2()));
  connect(this->ui->pointNormals2, SIGNAL(pressed()), this, SLOT(slotDisplayPointNormals2()));
  connect(this->ui->wireframe, SIGNAL(pressed()), this, SLOT(slotDisplayWireframe()));
  connect(this->ui->points, SIGNAL(pressed()), this, SLOT(slotDisplayPoints()));
  /*virtual void slotDisplayCellNormals();
  virtual void slotDisplayPointNormals();
  virtual void slotDisplayWireframe();
  virtual void slotDisplayPoints();*/
  
  
}


void mqDisplayControlsWidget::slotDisplayCellNormals()
{
	if (this->ui->cellNormals->isChecked())
	{
		this->ui->cellNormals->setChecked(false);
	}
	this->ui->cellNormals2->setChecked(false);
	this->ui->pointNormals->setChecked(false);
	this->ui->pointNormals2->setChecked(false);
	this->ui->wireframe->setChecked(false);
	this->ui->points->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_DisplayMode(0);
	mqMorphoDigCore::instance()->Render();
	
}
void mqDisplayControlsWidget::slotDisplayCellNormals2()
{
	if (this->ui->cellNormals2->isChecked())
	{
		this->ui->cellNormals2->setChecked(false);
	}
	this->ui->cellNormals->setChecked(false);
	this->ui->pointNormals->setChecked(false);
	this->ui->pointNormals2->setChecked(false);
	this->ui->wireframe->setChecked(false);
	this->ui->points->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_DisplayMode(4);
	
	mqMorphoDigCore::instance()->Render();

}


void mqDisplayControlsWidget::slotDisplayPointNormals()
{
	if (this->ui->pointNormals->isChecked())
	{
		this->ui->pointNormals->setChecked(false);
	}	
	this->ui->pointNormals2->setChecked(false);
	this->ui->cellNormals->setChecked(false);	
	this->ui->cellNormals2->setChecked(false);
	this->ui->wireframe->setChecked(false);
	this->ui->points->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_DisplayMode(1);
	mqMorphoDigCore::instance()->Render();

}
void mqDisplayControlsWidget::slotDisplayPointNormals2()
{
	if (this->ui->pointNormals2->isChecked())
	{
		this->ui->pointNormals2->setChecked(false);
	}
	this->ui->pointNormals->setChecked(false);
	this->ui->cellNormals->setChecked(false);	
	this->ui->cellNormals2->setChecked(false);
	this->ui->wireframe->setChecked(false);
	this->ui->points->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_DisplayMode(5);
	mqMorphoDigCore::instance()->Render();

}
void mqDisplayControlsWidget::slotDisplayWireframe()
{
	if (this->ui->wireframe->isChecked())
	{
		this->ui->wireframe->setChecked(false);
	}
	this->ui->cellNormals->setChecked(false);
	this->ui->pointNormals->setChecked(false);
	this->ui->cellNormals2->setChecked(false);
	this->ui->pointNormals2->setChecked(false);
	this->ui->points->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_DisplayMode(2);

	mqMorphoDigCore::instance()->Render();

}

void mqDisplayControlsWidget::slotDisplayPoints()
{
	if (this->ui->points->isChecked())
	{
		this->ui->points->setChecked(false);
	}
	this->ui->cellNormals2->setChecked(false);
	this->ui->pointNormals2->setChecked(false);
	this->ui->cellNormals->setChecked(false);
	this->ui->wireframe->setChecked(false);
	this->ui->pointNormals->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_DisplayMode(3);
	mqMorphoDigCore::instance()->Render();

}



void mqDisplayControlsWidget::slotClippingPlaneOnOff()
{
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ChangeClippingPlane();
	mqMorphoDigCore::instance()->Render();
	//mqMorphoDigCore::instance()->Render();
}

void mqDisplayControlsWidget::slotBackfaceCullingOnOff()
{

	mqMorphoDigCore::instance()->ChangeBackfaceCulling();
	mqMorphoDigCore::instance()->Render();
	//mqMorphoDigCore::instance()->Render();
}

