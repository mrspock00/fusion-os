/*
 * Copyright 2017, Chế Vũ Gia Hy, cvghy116@gmail.com.
 * Copyright 2011, Jérôme Duval, korli@users.berlios.de.
 * Copyright 2008-2010, Axel Dörfler, axeld@pinc-software.de.
 * This file may be used under the terms of the MIT License.
 */
#ifndef VOLUME_H
#define VOLUME_H


#include "btrfs.h"


enum volume_flags {
	VOLUME_READ_ONLY	= 0x0001
};

class BTree;
class Chunk;
class Inode;
class Journal;
class Transaction;
class ExtentAllocator;


class Volume {
public:
								Volume(fs_volume* volume);
								~Volume();

			status_t			Mount(const char* device, uint32 flags);
			status_t			Unmount();
			status_t			Initialize(int fd, const char* label,
									uint32 blockSize, uint32 sectorSize);

			bool				IsValidSuperBlock();
			bool				IsReadOnly() const
									{ return (fFlags & VOLUME_READ_ONLY) != 0; }

			Inode*				RootNode() const { return fRootNode; }
			int					Device() const { return fDevice; }

			dev_t				ID() const
									{ return fFSVolume ? fFSVolume->id : -1; }
			fs_volume*			FSVolume() const { return fFSVolume; }
			const char*			Name() const;
			BTree*				FSTree() const { return fFSTree; }
			BTree*				ExtentTree() const { return fExtentTree; }
			BTree*				RootTree() const { return fRootTree; }

			uint32				SectorSize() const { return fSectorSize; }
			uint32				MaxInlineSize() const
									{ return fSectorSize / 2; }
			uint32				BlockSize() const { return fBlockSize; }
			ino_t				GetNextInodeID() { return ++fLargestInodeID; }
			Chunk*				SystemChunk() const { return fChunk; }
			Journal*			GetJournal() const { return fJournal; }
			ExtentAllocator*	GetAllocator() const { return fExtentAllocator; }

			btrfs_super_block&	SuperBlock() { return fSuperBlock; }

			status_t			LoadSuperBlock();
			status_t			WriteSuperBlock();

			// cache access
			void*				BlockCache() { return fBlockCache; }

	static	status_t			Identify(int fd, btrfs_super_block* superBlock);

			status_t			FindBlock(off_t logical, fsblock_t& physical);
			status_t			FindBlock(off_t logical, off_t& physical);
			status_t			GetNewBlock(uint64& logical, fsblock_t& physical,
									uint64 start = (uint64)-1,
									uint64 flags = BTRFS_BLOCKGROUP_FLAG_METADATA);

private:
			mutex				fLock;
			fs_volume*			fFSVolume;
			int					fDevice;
			ino_t				fLargestInodeID;
			btrfs_super_block	fSuperBlock;
			char				fName[32];

			uint32				fFlags;
			uint32				fSectorSize;
			uint32				fBlockSize;

			void*				fBlockCache;
			Inode*				fRootNode;

			ExtentAllocator*	fExtentAllocator;
			Chunk*				fChunk;
			Journal*			fJournal;
			BTree*				fChunkTree;
			BTree*				fRootTree;
			BTree*				fDevTree;
			BTree*				fExtentTree;
			BTree*				fFSTree;
			BTree*				fChecksumTree;
};


#endif	// VOLUME_H
