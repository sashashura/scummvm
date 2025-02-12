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

#ifndef SCUMM_SCRIPT_V8_H
#define SCUMM_SCRIPT_V8_H

#ifdef ENABLE_SCUMM_7_8

#include "scumm/scumm_v7.h"

namespace Scumm {

class ScummEngine_v8 : public ScummEngine_v7 {
protected:
	struct ObjectNameId {
		char name[40];
		int id;
	};
	int _objectIDMapSize;
	ObjectNameId *_objectIDMap;

	struct StampShot {
		int slot;
		int boxX;
		int boxY;
		int boxWidth;
		int boxHeight;
		int brightness;
	};

	int _savegameThumbnailPalette[256];
	byte _savegameThumbnail[160 * 120]; // One fourth of the nominal 640x480 resolution
	StampShot _stampShots[20];
	int _stampShotsInQueue = 0;

	int _keyScriptKey, _keyScriptNo;

public:
	ScummEngine_v8(OSystem *syst, const DetectorResult &dr);
	~ScummEngine_v8() override;

	void setKeyScriptVars(int _keyScriptKey, int _keyScriptNo);
	void stampShotDequeue();

protected:
	void setupOpcodes() override;

	void printString(int m, const byte *msg) override;

	void scummLoop_handleSaveLoad() override;

	void setupScummVars() override;
	void resetScummVars() override;
	void decodeParseString(int m, int n) override;
	void readArrayFromIndexFile() override;

	void readMAXS(int blockSize) override;
	void readGlobalObjects() override;

	uint fetchScriptWord() override;
	int fetchScriptWordSigned() override;

	int readVar(uint var) override;
	void writeVar(uint var, int value) override;

	int getObjectIdFromOBIM(const byte *obim) override;

	void processKeyboard(Common::KeyState lastKeyHit) override;
	void updateCursor() override;
	void setCursorTransparency(int a) override;
	void desaturatePalette(int hueScale, int satScale, int lightScale, int startColor, int endColor);

	void stampShotEnqueue(int slot, int boxX, int boxY, int boxWidth, int boxHeight, int brightness);
	void stampScreenShot(int slot, int boxX, int boxY, int boxWidth, int boxHeight, int brightness);
	void saveLoadWithSerializer(Common::Serializer &s) override;
	void createInternalSaveStateThumbnail();
	bool fetchInternalSaveStateThumbnail(int slotId, bool isHeapSave);
	uint32 *fetchScummVMSaveStateThumbnail(int slotId, bool isHeapSave, int brightness);

	/* Version 8 script opcodes */
	void o8_mod();
	void o8_wait();

	void o8_dimArray();
	void o8_dim2dimArray();
	void o8_arrayOps();
	void o8_blastText();

	void o8_cursorCommand();
	void o8_resourceRoutines();
	void o8_roomOps();
	void o8_actorOps();
	void o8_cameraOps();
	void o8_verbOps();

	void o8_systemOps();
	void o8_startVideo();
	void o8_kernelSetFunctions();
	void o8_kernelGetFunctions();

	void o8_getActorChore();
	void o8_getActorZPlane();

	void o8_drawObject();
	void o8_getObjectImageX();
	void o8_getObjectImageY();
	void o8_getObjectImageWidth();
	void o8_getObjectImageHeight();

	void o8_getStringWidth();

	byte VAR_LANGUAGE;
};

} // End of namespace Scumm

#endif // ENABLE_SCUMM_7_8

#endif
