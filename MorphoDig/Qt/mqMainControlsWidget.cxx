/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsWidget.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "mqMainControlsWidget.h"
#include "ui_mqMainControlsWidget.h"

// For later!
#include "mqSaveNTWDialogReaction.h"
#include "mqUndoRedoReaction.h"

#include "mqMorphoDigCore.h"
#include "mqOpenDataReaction.h"



#include <QToolButton>


//-----------------------------------------------------------------------------
void mqMainControlsWidget::constructor()
{
 // Ui::mqMainControlsWidget ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqMainControlsWidget;
  this->ui->setupUi(this);
 


  QAction* actionSaveData = new QAction(tr("&SaveDATA"), this);
  actionSaveData->setToolTip(tr("Save project."));
  this->ui->SaveData->addAction(actionSaveData);
  this->ui->SaveData->setDefaultAction(actionSaveData);
  QIcon icon;
  icon.addFile(QStringLiteral(":/Icons/fileopen22.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionSaveData->setIcon(icon);
  
  new mqSaveNTWDialogReaction(actionSaveData);
  
  QAction* actionOpenData = new QAction(tr("&OpenDATA"), this);
  actionOpenData->setToolTip(tr("Save project."));
  this->ui->OpenData->addAction(actionOpenData);
  this->ui->OpenData->setDefaultAction(actionOpenData);
  QIcon icon2;
  icon2.addFile(QStringLiteral(":/Icons/filesave22.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionOpenData->setIcon(icon2);

  new mqOpenDataReaction(actionOpenData, 0);//0= open data (generic)
 

  QAction* actionUndo= new QAction(tr("&Undo"), this);
  actionUndo->setToolTip(tr("Undo last action."));
  this->ui->Undo->addAction(actionUndo);
  this->ui->Undo->setDefaultAction(actionUndo);
  QIcon icon3;
  icon3.addFile(QStringLiteral(":/Icons/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionUndo->setIcon(icon3);

  new mqUndoRedoReaction(actionUndo, true);

  QAction* actionRedo = new QAction(tr("&Redo"), this);
  actionRedo->setToolTip(tr("Redo last action."));
  this->ui->Redo->addAction(actionRedo);
  this->ui->Redo->setDefaultAction(actionRedo);
  QIcon icon4;
  icon4.addFile(QStringLiteral(":/Icons/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionRedo->setIcon(icon4);
  new mqUndoRedoReaction(actionRedo, false);
  
  


  
  
}





