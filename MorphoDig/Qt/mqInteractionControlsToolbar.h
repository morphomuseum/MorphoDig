/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsToolbar.h

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
#ifndef mqInteractionControlsToolbar_h
#define mqInteractionControlsToolbar_h


#include <QToolBar>
class Ui_mqInteractionControlsToolbar;

/**
* mqInteractionControlsToolbar is the toolbar with actions (and reactions) for the
* "Interaction Controls" toolbar in MophoDig. 
* Simply instantiate this and put it in your application UI file or
* QMainWindow to use it.
*/
class  mqInteractionControlsToolbar : public QToolBar
{
  Q_OBJECT
  typedef QToolBar Superclass;

public:
  mqInteractionControlsToolbar(const QString& title, QWidget* parentObject = 0)
    : Superclass(title, parentObject)
  {
    this->constructor();
  }
  mqInteractionControlsToolbar(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
  

  public slots :
  virtual void slotLandmarkNormalMode();
  virtual void slotLandmarkTargetMode();
  virtual void slotLandmarkNodeMode();
  virtual void slotLandmarkHandleMode();
  virtual void slotFlagMode();
  virtual void slotMoveAll();
  virtual void slotMoveOnlyLandmarks();
private:
	Q_DISABLE_COPY(mqInteractionControlsToolbar)
  
  Ui_mqInteractionControlsToolbar *ui;
  void constructor();
};

#endif
