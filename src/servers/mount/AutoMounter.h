/*
 * Copyright 2007-2015, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef	AUTO_MOUNTER_H
#define AUTO_MOUNTER_H


#include <DiskDeviceDefs.h>
#include <File.h>
#include <Message.h>
#include <Server.h>


class BPartition;
class BPath;


enum mount_mode {
	kNoVolumes,
	kOnlyBFSVolumes,
	kAllVolumes,
	kRestorePreviousVolumes
};


class AutoMounter : public BServer {
public:
								AutoMounter();
	virtual						~AutoMounter();

	virtual	void				ReadyToRun();
	virtual	void				MessageReceived(BMessage* message);
	virtual	bool				QuitRequested();

private:
			void				_GetSettings(BMessage* message);

			void				_MountVolumes(mount_mode normal,
									mount_mode removable,
									bool initialRescan = false,
									partition_id deviceID = -1);
			void				_MountVolume(const BMessage* message);
			bool				_SuggestForceUnmount(const char* name,
									status_t error);
			void				_ReportUnmountError(const char* name,
									status_t error);
			void				_UnmountAndEjectVolume(BPartition* partition,
									BPath& mountPoint, const char* name);
			void				_UnmountAndEjectVolume(BMessage* message);

			void				_FromMode(mount_mode mode, bool& all,
									bool& bfs, bool& restore);
			mount_mode			_ToMode(bool all, bool bfs,
									bool restore = false);

			void				_UpdateSettingsFromMessage(BMessage* message);
			void				_ReadSettings();
			void				_WriteSettings();

	static	bool				_SuggestMountFlags(const BPartition* partition,
									uint32* _flags);

		friend class MountVisitor;

private:
			mount_mode			fNormalMode;
			mount_mode			fRemovableMode;
			bool				fEjectWhenUnmounting;

			BFile				fPrefsFile;
			BMessage			fSettings;
};


#endif // AUTO_MOUNTER_H
