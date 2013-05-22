/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef QmitkBooleanOperationsWidget_h
#define QmitkBooleanOperationsWidget_h

#include <QWidget>
#include <ui_QmitkBooleanOperationsWidgetControls.h>

class QmitkBooleanOperationsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QmitkBooleanOperationsWidget(QWidget* parent = NULL);
  ~QmitkBooleanOperationsWidget();

private:
  Ui::QmitkBooleanOperationsWidgetControls m_Controls;
};

#endif
