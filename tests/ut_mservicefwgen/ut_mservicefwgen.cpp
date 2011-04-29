/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#include <MLocale>
#include "ut_mservicefwgen.h"

#include <MApplication>
#include <MApplicationWindow>

void Ut_MServiceFwGen::init()
{
}

void Ut_MServiceFwGen::cleanup()
{
}

QCoreApplication *Ut_MServiceFwGen::buildApp()
{
    QCoreApplication *retVal = 0;

    QChar sep(' ');
    static char *argv[MAX_PARAMS];
    static int count=1;
    argv[0]=(char *)"ut_mservicefwgen";

    retVal = new QCoreApplication(count, argv);

    return retVal;
}

// this test basically tests that the code generated by the libmeegotouch/tools/m-servicefwgen tool actually compiles
// it's here to catch the case when methods are removed or renamed, since it can seem like nothing is actually using them
void Ut_MServiceFwGen::testServiceFwMethods()
{
    app = buildApp();
    delete app;
}

QTEST_APPLESS_MAIN(Ut_MServiceFwGen);
