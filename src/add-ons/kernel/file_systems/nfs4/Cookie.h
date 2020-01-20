/*
 * Copyright 2012 Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Paweł Dziepak, pdziepak@quarnos.org
 */
#ifndef COOKIE_H
#define COOKIE_H


#include <SupportDefs.h>

#include "DirectoryCache.h"
#include "FileSystem.h"


struct OpenState;

struct LockOwner {
			uint64			fClientId;

			uint32			fStateId[3];
			uint32			fStateSeq;
			uint32			fSequence;

			uint32			fOwner;

			uint32			fUseCount;

			mutex			fLock;

			LockOwner*		fNext;
			LockOwner*		fPrev;

							LockOwner(uint32 owner);
							~LockOwner();
};

struct LockInfo {
			LockOwner*		fOwner;

			uint64			fStart;
			uint64			fLength;
			LockType		fType;

			LockInfo*		fNext;
			LockInfo*		fCookieNext;

							LockInfo(LockOwner* owner);
							~LockInfo();

			bool			operator==(const struct flock& lock) const;
			bool			operator==(const LockInfo& lock) const;
};

struct Cookie {
			struct RequestEntry {
				RPC::Request*	fRequest;
				RequestEntry*	fNext;
			};

			FileSystem*		fFileSystem;
			RequestEntry*	fRequests;
			mutex			fRequestLock;

			sem_id			fSnoozeCancel;

							Cookie(FileSystem* fileSystem);
	virtual					~Cookie();

			status_t		RegisterRequest(RPC::Request* req);
			status_t		UnregisterRequest(RPC::Request* req);
			status_t		CancelAll();
};

struct OpenStateCookie : public Cookie {
			OpenState*		fOpenState;
			uint32			fMode;

							OpenStateCookie(FileSystem* fileSystem);
};

struct OpenFileCookie : public OpenStateCookie {
			LockInfo*		fLocks;

			void			AddLock(LockInfo* lock);
			void			RemoveLock(LockInfo* lock, LockInfo* prev);

							OpenFileCookie(FileSystem* fileSystem);
};

struct OpenDirCookie : public Cookie {
			int							fSpecial;
			DirectoryCacheSnapshot*		fSnapshot;
			NameCacheEntry*				fCurrent;
			bool						fEOF;

			bool						fAttrDir;

										OpenDirCookie(FileSystem* fileSystem);
										~OpenDirCookie();
};

struct OpenAttrCookie : public OpenStateCookie {
							OpenAttrCookie(FileSystem* fileSystem);
};


#endif	// COOKIE_H

