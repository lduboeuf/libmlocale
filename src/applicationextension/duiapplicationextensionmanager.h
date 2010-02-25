/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef DUIAPPLICATIONEXTENSIONMANAGER_H
#define DUIAPPLICATIONEXTENSIONMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>
#include <QSharedPointer>
#include <QHash>

class DuiApplicationExtensionMetaData;
class DuiDataStore;
class DuiWidget;
class DuiExtensionHandle;
class DuiApplicationExtensionInterface;
class DuiFileDataStore;

class DuiApplicationExtensionManager : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a DuiApplicationExtensionManager.
     * \param interface The interface the extensions to be instantiated by this manager should implement
     * \param enableInProcessExtensions \c true if in process extensions should be loaded. \c false otherwise
     */
    DuiApplicationExtensionManager(const QString &interface, bool enableInProcessExtensions = true);

    //! Destroys the DuiApplicationExtensionManager object
    virtual ~DuiApplicationExtensionManager();

    /*!
      * Initializes the application extension manager
      *
      * \return true if initialization succeeds, false otherwise
      */
    bool init();

    /*!
      * Returns a list of in process extensions loaded by this manager
      *
      * \return List of application extension interface pointers. Receving party should appropriately cast them correctly.
      */
    QList<DuiApplicationExtensionInterface*> extensions();

Q_SIGNALS:
    /*!
     * Signal sent when an extension has been instantiated.
     *
     * \param extension the extension that was instantiated
     */
    void extensionInstantiated(DuiApplicationExtensionInterface *extension);

    /*!
     * Signal that is sent when an extension has been removed.
     * \param extension the extension that was removed
     */
    void extensionRemoved(DuiApplicationExtensionInterface* extension);

    /*!
     * Signal sent when an extension widget is removed
     * \param widget the extension that was removed
     */
    void widgetRemoved(DuiWidget* widget);

    /*!
     * Signal for instantiating an extension widget and it's datastore
     * \param widget Widget to be added onto the DuiExtensionArea.
     * \param store This DuiDataStore object can be used to store permanent extension area data related
     * to this particular application extension instance.
     */
     void widgetCreated(DuiWidget *widget, DuiDataStore &store);

private slots:
    /*!
     * \brief Slot for refreshing the list of available application extensions
     * \param path the path of the extensions
     */
    void updateAvailableExtensions(const QString &path);

private:
    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher watcher;

    //! The name of the interface extensions to be instantiated should implement
    QString interface;

    //! Data store for application extension manager
    QSharedPointer <DuiDataStore> extensionDataStore;

    //! Keeps track of whether in process extensions should be loaded or not
    const bool enableInProcessExtensions;

    typedef QPair<DuiApplicationExtensionInterface*, DuiWidget*> InProcessExtensionData;

    //! Instantiated in-process extensions. A map from the shared library name to the extension instance.
    QHash<QString, InProcessExtensionData> inProcessExtensions;

    //! Hash of desktop file name versus the widget (handle) created
    QHash<QString, DuiExtensionHandle*> outOfProcessHandles;

    typedef QHash<QString, QSharedPointer<DuiApplicationExtensionMetaData> > MetaDataEntryHash;
    /*!
     * A container for application extension desktop entries with the correct application extension interface defined.
     * This container also only contains the entries that succeeded to instantiate.
     * This maps from the desktop entry file name (without path) to the desktop entry object.
     */
    MetaDataEntryHash extensionMetaDatas;


    /*!
     * Gives the path where the application extension specific data is kept
     */
    QString dataPath() const;

    /*!
     * Determines whether the extension represented by the meta data object is an in-process
     * extension or not.
     * \param metaData the meta data object of the extension
     * \return \c true if the extension should be run in-process, \c false otherwise
     */
    static bool isInProcess(const DuiApplicationExtensionMetaData &metaData);

    /*!
     * Creates the datastore for application extension manager for storing the layouting information of the extensions
     */
    bool createDataStore();

    /*!
     * Instantiate an extension for an application extension metadata
     * \param metadata the metadata object of the extension
     * \return \c true if extension instantiated succesfully, \c false otherwise
     */
    bool instantiateExtension(const DuiApplicationExtensionMetaData &metadata);

    /*!
     * Instantiates an in process extension.
     *
     * \param binary the name of the in process extension binary
     * \return \c true if the instantiation succeeds, \c false otherwise
     */
    bool instantiateInProcessExtension(const QString &binary);

    /*!
     * Instantiate out of process extensions
     * \param metadata for the extension
     * \return bool True if extension instantiated, else return false
     */
    bool instantiateOutOfProcessExtension(const DuiApplicationExtensionMetaData &metadata);

    /*!
     * Removes an extension that was defined in the desktop entry given as a parameter.
     * \param metadata the relevant metadata.
     */
    void removeExtension(const DuiApplicationExtensionMetaData &metadata);

    /*!
     * Removes an in process extension that was loaded from the specified library.
     * \param library the path name to the library
     */
    void removeInProcessExtension(const QString &library);

    /*!
     * Removes an out of process extension that was loaded from the specified metadata
     * \param metadata for the extension
     */
    void removeOutOfProcessExtension(const DuiApplicationExtensionMetaData &metadata);

    /*!
     * Returns a file name for an application extension data file. Every application extension
     * has its own data file.
     *
     * \param interface The interface from which the extension needs to be instantiated by the manager
     * \return data file path of each application extension
     */
    QString createApplicationExtensionDataFileName(const QString& interface) const;

#ifdef UNIT_TEST
    friend class Ut_DuiApplicationExtensionManager;
#endif
};

#endif // DUIAPPLICATIONEXTENSIONMANAGER_H
