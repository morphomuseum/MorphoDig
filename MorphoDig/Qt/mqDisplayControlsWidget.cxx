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

