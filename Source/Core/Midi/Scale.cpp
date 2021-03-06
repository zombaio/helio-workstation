/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Common.h"
#include "Scale.h"
#include "SerializationKeys.h"

#define CHROMATIC_SCALE_SIZE 12

enum Key
{
    I = 0,
    II = 1,
    III = 2,
    IV = 3,
    V = 4,
    VI = 5,
    VII = 6
};

Scale::Scale() {}
Scale::Scale(const String &name) : name(name) {}

//===----------------------------------------------------------------------===//
// Helpers
//===----------------------------------------------------------------------===//

int Scale::getSize() const noexcept
{
    return this->keys.size();
}

bool Scale::isValid() const noexcept
{
    return !this->keys.isEmpty() && !this->name.isEmpty();
}

String Scale::getName() const noexcept
{
    return this->name;
}

String Scale::getLocalizedName() const
{
    return TRANS(this->name);
}

Array<int> Scale::getPowerChord(Function fun, bool oneOctave) const
{
    return {
        this->getKey(Key::I + fun, oneOctave),
        this->getKey(Key::V + fun, oneOctave) };
}

Array<int> Scale::getTriad(Function fun, bool oneOctave) const
{
    return {
        this->getKey(Key::I + fun, oneOctave),
        this->getKey(Key::III + fun, oneOctave),
        this->getKey(Key::V + fun, oneOctave) };
}

Array<int> Scale::getSeventhChord(Function fun, bool oneOctave) const
{
    return {
        this->getKey(Key::I + fun, oneOctave),
        this->getKey(Key::III + fun, oneOctave),
        this->getKey(Key::V + fun, oneOctave),
        this->getKey(Key::VII + fun, oneOctave) };
}

bool Scale::seemsMinor() const
{
    return this->getKey(Key::III) == 3;
}

int Scale::getKey(int key, bool shouldRestrictToOneOctave /*= false*/) const
{
    jassert(key >= 0);
    const int idx = this->keys[key % this->getSize()];
    return shouldRestrictToOneOctave ? idx :
        idx + (CHROMATIC_SCALE_SIZE * (key / this->getSize()));
}

//===----------------------------------------------------------------------===//
// Serializable
//===----------------------------------------------------------------------===//

XmlElement *Scale::serialize() const
{
    auto xml = new XmlElement(Serialization::Core::scale);
    xml->setAttribute(Serialization::Core::scaleName, this->name);

    int prevKey = 0;
    String intervals;
    for (auto key : this->keys)
    {
        if (key > 0)
        {
            intervals += String(key - prevKey) + " ";
            prevKey = key;
        }
    }

    intervals += String(CHROMATIC_SCALE_SIZE - prevKey);
    xml->setAttribute(Serialization::Core::scaleIntervals, intervals);

    return xml;
}

void Scale::deserialize(const XmlElement &xml)
{
    const XmlElement *root = (xml.getTagName() == Serialization::Core::scale) ?
        &xml : xml.getChildByName(Serialization::Core::scale);

    if (root == nullptr) { return; }

    this->reset();

    this->name = root->getStringAttribute(Serialization::Core::scaleName, this->name);
    const String intervals = root->getStringAttribute(Serialization::Core::scaleIntervals);
    StringArray tokens;
    tokens.addTokens(intervals, true);
    int key = 0;
    for (auto token : tokens)
    {
        this->keys.add(key);
        key += token.getIntValue();
    }
}

void Scale::reset()
{
    this->keys.clearQuick();
    this->name = {};
}
