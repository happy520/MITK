/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#ifndef QMITK_CALLBACK_WITHIN_GUI_TREAD_H_INCLUDGEWQ
#define QMITK_CALLBACK_WITHIN_GUI_TREAD_H_INCLUDGEWQ

#include "mitkCallbackFromGUIThread.h"

#include <qobject.h>

/*!
  \brief Qt specific implementation of mitk::CallbackFromGUIThreadImplementation
*/
class QmitkCallbackFromGUIThread : public QObject, public mitk::CallbackFromGUIThreadImplementation
{

  Q_OBJECT

  public:
    
    /// Change the current application cursor
    virtual void CallThisFromGUIThread(itk::Command*);

    QmitkCallbackFromGUIThread();
    virtual ~QmitkCallbackFromGUIThread();

    virtual bool event( QEvent* e );

  protected:
  private:
};
  
#endif

