/**
 * Copyright (c) 2009 Guillaume Martres <smarter@ubuntu.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Benjamin Meyer nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "cachedsettings.h"

#include <qstringlist.h>

CachedSettings::CachedSettings()
    : QSettings()
{
}

void CachedSettings::beginGroup(const QString &prefix)
{
    QSettings::beginGroup(prefix);
    QStringList keyList = QSettings::allKeys();
    for (int i = 0; i < keyList.size(); ++i) {
        QString key = keyList.at(i);
        QVariant value = QSettings::value(key);
        m_valueHash.insert(key, value);
    }
}

QString CachedSettings::key(const QString &value) const
{
   return m_valueHash.key(value);
}

QVariant CachedSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (!m_valueHash.contains(key))
        return defaultValue;

    return m_valueHash.value(key);
}

void CachedSettings::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key, qVariantFromValue(value));
    m_valueHash.insert(key, value);
}

void CachedSettings::removeEntry(const QString &key)
{
    QSettings::remove(key);
    m_valueHash.remove(key);
}

bool CachedSettings::hasKey(const QString &key) const
{
    return m_valueHash.contains(key);
}
