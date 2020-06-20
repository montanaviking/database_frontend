//
// Created by viking on 7/11/18.
// Logic operators for queries
#include <sqlsetup.h>

const auto and_(const QString &conditions)
{
    auto cond=conditions.split(",",QString::SkipEmptyParts);
    auto ret=cond.at(0);
    cond.removeFirst();
    if (cond.length()>1) ret="( "+ret;
    foreach(auto c, cond)
        {
            ret+=" AND "+c;
        }
    if (cond.length()>1) ret=ret+" )";
    return ret;
}

const auto or_(const QString &conditions)
{
    auto cond=conditions.split(",",QString::SkipEmptyParts);
    auto ret=cond.at(0);
    cond.removeFirst();
    if (cond.length()>1) ret="( "+ret;
            foreach(auto c, cond)
        {
            ret+=" OR "+c;
        }
    if (cond.length()>1) ret=ret+" )";
    return ret;
}

const auto not_(const QString &conditions)
{
    auto ret = " NOT "+conditions;
    return ret;
}

