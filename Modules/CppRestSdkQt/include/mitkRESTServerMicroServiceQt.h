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

#ifndef mitkRESTServerMicroServiceQt_h
#define mitkRESTServerMicroServiceQt_h

#include <QThread>
#include <mitkRESTServerMicroService.h>

namespace mitk
{
  class RESTServerMicroServiceQt : public QObject, public RESTServerMicroService
  {
    Q_OBJECT 


  public:
    /**
     * @brief Creates an server listening to the given URI
     *
     * @param uri the URI at which the server is listening for requests
     */
    RESTServerMicroServiceQt(web::uri uri);
    ~RESTServerMicroServiceQt();

  public slots:
    /**
     * @brief Opens the listener and starts the listening process
     */
    void OpenListener() override;

    /**
     * @brief Closes the listener and stops the listening process
     */
    void CloseListener() override;
  };
} // namespace mitk
#endif