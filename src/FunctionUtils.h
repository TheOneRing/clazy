/*
   This file is part of the clazy static checker.

  Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Sérgio Martins <sergio.martins@kdab.com>

  Copyright (C) 2015 Sergio Martins <smartins@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef FUNCTION_UTILS_H
#define FUNCTION_UTILS_H

// Contains utility functions regarding functions and methods

#include "Utils.h"
#include "HierarchyUtils.h"

#include <clang/AST/Decl.h>
#include <string>

namespace FunctionUtils {

inline bool hasCharPtrArgument(clang::FunctionDecl *func, int expected_arguments = -1)
{
    if (expected_arguments != -1 && (int)func->param_size() != expected_arguments)
        return false;

    for (auto param : Utils::functionParameters(func)) {
        clang::QualType qt = param->getType();
        const clang::Type *t = qt.getTypePtrOrNull();
        if (!t)
            continue;

        const clang::Type *realT = t->getPointeeType().getTypePtrOrNull();

        if (!realT)
            continue;

        if (realT->isCharType())
            return true;
    }

    return false;
}

inline clang::ValueDecl *valueDeclForCallArgument(clang::CallExpr *call, unsigned int argIndex)
{
    if (!call || call->getNumArgs() <= argIndex)
        return nullptr;

    clang::Expr *firstArg = call->getArg(argIndex);
    auto declRef = llvm::isa<clang::DeclRefExpr>(firstArg) ? llvm::cast<clang::DeclRefExpr>(firstArg)
                                                           : HierarchyUtils::getFirstChildOfType2<clang::DeclRefExpr>(firstArg);
    if (!declRef)
        return nullptr;

    return declRef->getDecl();
}


}
#endif
