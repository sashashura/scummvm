/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef COMMON_WINEXE_H
#define COMMON_WINEXE_H

#include "common/hash-str.h"
#include "common/str.h"
#include "common/types.h"

namespace Common {

/**
 * @defgroup common_winexe Windows resources
 * @ingroup common
 *
 * @brief API for managing Windows resources.
 *
 * @{
 */

class SeekableReadStream;

/** The default Windows resources. */
enum WinResourceType {
	kWinCursor       = 0x01,
	kWinBitmap       = 0x02,
	kWinIcon         = 0x03,
	kWinMenu         = 0x04,
	kWinDialog       = 0x05,
	kWinString       = 0x06,
	kWinFontDir      = 0x07,
	kWinFont         = 0x08,
	kWinAccelerator  = 0x09,
	kWinRCData       = 0x0A,
	kWinMessageTable = 0x0B,
	kWinGroupCursor  = 0x0C,
	kWinGroupIcon    = 0x0E,
	kWinVersion      = 0x10,
	kWinDlgInclude   = 0x11,
	kWinPlugPlay     = 0x13,
	kWinVXD          = 0x14,
	kWinAniCursor    = 0x15,
	kWinAniIcon      = 0x16,
	kWinHTML         = 0x17,
	kWinManifest     = 0x18
};

class WinResourceID {
public:
	WinResourceID() { _idType = kIDTypeNull; _id = 0; }
	WinResourceID(String x) { _idType = kIDTypeString; _name = x; }
	WinResourceID(uint32 x) { _idType = kIDTypeNumerical; _id = x; }

	WinResourceID &operator=(const String &x);
	WinResourceID &operator=(uint32 x);

	bool operator==(const String &x) const;
	bool operator==(const uint32 &x) const;
	bool operator==(const WinResourceID &x) const;

	String getString() const;
	uint32 getID() const;
	String toString() const;

private:
	/** An ID Type. */
	enum IDType {
		kIDTypeNull,      ///< No type set
		kIDTypeNumerical, ///< A numerical ID.
		kIDTypeString     ///< A string ID.
	} _idType;

	String _name;         ///< The resource's string ID.
	uint32 _id;           ///< The resource's numerical ID.
};

struct WinResourceID_Hash {
	uint operator()(const WinResourceID &id) const { return id.toString().hash(); }
};

struct WinResourceID_EqualTo {
	bool operator()(const WinResourceID &id1, const WinResourceID &id2) const { return id1 == id2; }
};

/**
 * A class able to load resources from a Windows Executable, such
 * as cursors, bitmaps, and sounds.
 */
class WinResources {
public:
	virtual ~WinResources() {};

	/** Clear all information. */
	virtual void clear() = 0;

	/** Load from an EXE file. */
	virtual bool loadFromEXE(const String &fileName);

	/** Load from a Windows compressed EXE file. */
	virtual bool loadFromCompressedEXE(const String &fileName);

	/** Load from a stream. */
	virtual bool loadFromEXE(SeekableReadStream *stream, DisposeAfterUse::Flag disposeFileHandle = DisposeAfterUse::YES) = 0;

	/** Return a list of IDs for a given type. */
	virtual const Array<WinResourceID> getIDList(const WinResourceID &type) const = 0;

	/** Return a list of languages for a given type and ID. */
	virtual const Array<WinResourceID> getLangList(const WinResourceID &type, const WinResourceID &id) const {
		Array<WinResourceID> array;
		return array;
	}

	/** Return a stream to the specified resource, taking the first language found (or 0 if non-existent). */
	virtual SeekableReadStream *getResource(const WinResourceID &type, const WinResourceID &id) = 0;

	/** Return a stream to the specified resource (or 0 if non-existent). */
	virtual SeekableReadStream *getResource(const WinResourceID &type, const WinResourceID &id, const WinResourceID &lang) {
		return getResource(type, id);
	}

	static WinResources *createFromEXE(const String &fileName);
	static WinResources *createFromEXE(SeekableReadStream *stream);

	typedef Common::HashMap<Common::String, Common::U32String, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> VersionHash;

	/** The structure of the version resource inside an NE EXE */
	struct VersionInfo {
		VersionInfo();

		uint16 fileVersion[4];
		uint16 productVersion[4];
		uint32 fileFlagsMask;
		uint32 fileFlags;
		uint32 fileOS;
		uint32 fileType;
		uint32 fileSubtype;
		uint32 fileDate[2];

		VersionHash hash;

		bool readVSVersionInfo(SeekableReadStream *res);
	};

	VersionInfo *getVersionResource(const WinResourceID &id);

	/** Get a string from a string resource. */
	virtual String loadString(uint32 stringID) = 0;

protected:
	virtual VersionInfo *parseVersionInfo(SeekableReadStream *stream) = 0;
};

/** @} */

} // End of namespace Common

#endif
