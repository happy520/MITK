/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "QmitkNodeSelectionDialog.h"

#include <mitkDataStorageInspectorGenerator.h>
#include <QmitkNodeSelectionPreferenceHelper.h>
#include <QmitkDataStorageSelectionHistoryInspector.h>

QmitkNodeSelectionDialog::QmitkNodeSelectionDialog(QWidget* parent, QString title, QString hint) : QDialog(parent),
  m_NodePredicate(nullptr), m_SelectOnlyVisibleNodes(false), m_SelectedNodes(NodeList()), m_SelectionMode(QAbstractItemView::SingleSelection)
{
  m_Controls.setupUi(this);

  m_CheckFunction = [](const NodeList &) { return ""; };

  auto providers = mitk::DataStorageInspectorGenerator::GetProviders();
  auto visibleProviders = mitk::GetVisibleDataStorageInspectors();
  auto favoriteID = mitk::GetFavoriteDataStorageInspector();

  if (visibleProviders.empty())
  {
    MITK_DEBUG << "No presets for visible node selection inspectors available. Use fallback (show all available inspectors)";
    unsigned int order = 0;
    for (auto proIter : providers)
    {
      visibleProviders.insert(std::make_pair(order, proIter.first));
      ++order;
    }
  }

  int favIndex = 0;
  bool favoriteFound = false;
  for (auto proIter : visibleProviders)
  {
    auto finding = providers.find(proIter.second);
    if (finding != providers.end())
    {
      auto inspector = finding->second->CreateInspector();
      QString name = QString::fromStdString(finding->second->GetInspectorDisplayName());
      QString desc = QString::fromStdString(finding->second->GetInspectorDescription());
      AddPanel(inspector, name, desc);

      favoriteFound = favoriteFound || proIter.second == favoriteID;
      if (!favoriteFound)
      {
        ++favIndex;
      }
    }
    else
    {
      MITK_DEBUG << "No provider registered for inspector that is defined as visible in the preferences. Illegal inspector ID: " << proIter.second;
    }
  }

  m_Controls.tabWidget->setCurrentIndex(favIndex);
  this->setWindowTitle(title);
  this->setToolTip(hint);

  m_Controls.hint->setText(hint);
  m_Controls.hint->setVisible(!hint.isEmpty());

  m_FavoriteNodesButton = new QPushButton("Add to favorites");
  m_Controls.buttonBox->addButton(m_FavoriteNodesButton, QDialogButtonBox::ActionRole);

  connect(m_FavoriteNodesButton, &QPushButton::clicked, this, &QmitkNodeSelectionDialog::OnFavoriteNodesButtonClicked);
  connect(m_Controls.buttonBox, SIGNAL(accepted()), this, SLOT(OnOK()));
  connect(m_Controls.buttonBox, SIGNAL(rejected()), this, SLOT(OnCancel()));
}

void QmitkNodeSelectionDialog::SetDataStorage(mitk::DataStorage* dataStorage)
{
  if (m_DataStorage != dataStorage)
  {
    m_DataStorage = dataStorage;

    if (!m_DataStorage.IsExpired())
    {
      for (auto panel : m_Panels)
      {
        panel->SetDataStorage(dataStorage);
      }
    }
  }
}

void QmitkNodeSelectionDialog::SetNodePredicate(const mitk::NodePredicateBase* nodePredicate)
{
  if (m_NodePredicate != nodePredicate)
  {
    m_NodePredicate = nodePredicate;

    for (auto panel : m_Panels)
    {
      panel->SetNodePredicate(m_NodePredicate);
    }
  }
}

const mitk::NodePredicateBase* QmitkNodeSelectionDialog::GetNodePredicate() const
{
  return m_NodePredicate;
}

QmitkNodeSelectionDialog::NodeList QmitkNodeSelectionDialog::GetSelectedNodes() const
{
  return m_SelectedNodes;
}

void QmitkNodeSelectionDialog::SetSelectionCheckFunction(const SelectionCheckFunctionType &checkFunction)
{
  m_CheckFunction = checkFunction;
  auto checkResponse = m_CheckFunction(m_SelectedNodes);

  m_Controls.hint->setText(QString::fromStdString(checkResponse));
  m_Controls.hint->setVisible(!checkResponse.empty());
  m_Controls.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(checkResponse.empty());
}

bool QmitkNodeSelectionDialog::GetSelectOnlyVisibleNodes() const
{
  return m_SelectOnlyVisibleNodes;
}

void QmitkNodeSelectionDialog::SetSelectionMode(SelectionMode mode)
{
  m_SelectionMode = mode;
  for (auto panel : m_Panels)
  {
    panel->SetSelectionMode(mode);
  }
}

QmitkNodeSelectionDialog::SelectionMode QmitkNodeSelectionDialog::GetSelectionMode() const
{
  return m_SelectionMode;
}

void QmitkNodeSelectionDialog::SetSelectOnlyVisibleNodes(bool selectOnlyVisibleNodes)
{
  if (m_SelectOnlyVisibleNodes != selectOnlyVisibleNodes)
  {
    m_SelectOnlyVisibleNodes = selectOnlyVisibleNodes;

    for (auto panel : m_Panels)
    {
      panel->SetSelectOnlyVisibleNodes(m_SelectOnlyVisibleNodes);
    }
  }
}

void QmitkNodeSelectionDialog::SetCurrentSelection(NodeList selectedNodes)
{
  m_SelectedNodes = selectedNodes;
  auto checkResponse = m_CheckFunction(m_SelectedNodes);

  m_Controls.hint->setText(QString::fromStdString(checkResponse));
  m_Controls.hint->setVisible(!checkResponse.empty());
  m_Controls.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(checkResponse.empty());

  for (auto panel : m_Panels)
  {
    panel->SetCurrentSelection(selectedNodes);
  }
}

void QmitkNodeSelectionDialog::OnSelectionChanged(NodeList selectedNodes)
{
  SetCurrentSelection(selectedNodes);
  emit CurrentSelectionChanged(selectedNodes);
}

void QmitkNodeSelectionDialog::OnFavoriteNodesButtonClicked()
{
  for (auto node : m_SelectedNodes)
  {
    node->SetBoolProperty("org.mitk.selection.favorite", true);
  }
}

void QmitkNodeSelectionDialog::OnOK()
{
  for (auto node : m_SelectedNodes)
  {
    QmitkDataStorageSelectionHistoryInspector::AddNodeToHistory(node);
  }

  this->accept();
}

void QmitkNodeSelectionDialog::OnCancel()
{
  this->reject();
}

void QmitkNodeSelectionDialog::AddPanel(QmitkAbstractDataStorageInspector* view, QString name, QString desc)
{
  view->setParent(this);
  view->SetSelectionMode(m_SelectionMode);

  auto tabPanel = new QWidget();
  tabPanel->setObjectName(QString("tab_") + name);
  tabPanel->setToolTip(desc);
  m_Controls.tabWidget->insertTab(m_Controls.tabWidget->count(), tabPanel, name);

  auto verticalLayout = new QVBoxLayout(tabPanel);
  verticalLayout->setSpacing(0);
  verticalLayout->setContentsMargins(0, 0, 0, 0);
  verticalLayout->addWidget(view);

  m_Panels.push_back(view);
  connect(view, &QmitkAbstractDataStorageInspector::CurrentSelectionChanged, this, &QmitkNodeSelectionDialog::OnSelectionChanged);
}
