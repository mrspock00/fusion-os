/*
 * Copyright 2013-2019, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ingo Weinhold <ingo_weinhold@gmx.de>
 *		Rene Gollent <rene@gollent.com>
 *		Brian Hill <supernova@tycho.email>
 *		Jacob Secunda
 */
#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H


#include <package/DaemonClient.h>
#include <package/manager/PackageManager.h>

#include "constants.h"
#include "SoftwareUpdaterWindow.h"

class ProblemWindow;

//using namespace BPackageKit;
using BPackageKit::BPackageInstallationLocation;
using BPackageKit::BPrivate::BDaemonClient;
using BPackageKit::BManager::BPrivate::BPackageManager;


class UpdateManager : public BPackageManager,
	private BPackageManager::UserInteractionHandler {
public:
								UpdateManager(
									BPackageInstallationLocation location,
									bool verbose);
								~UpdateManager();

			void				CheckNetworkConnection();
			update_type			GetUpdateType();
			void				CheckRepositories();
	virtual	void				JobFailed(BSupportKit::BJob* job);
	virtual	void				JobAborted(BSupportKit::BJob* job);
			void				FinalUpdate(const char* header,
									const char* text);
private:
	// UserInteractionHandler
	virtual	void				HandleProblems();
	virtual	void				ConfirmChanges(bool fromMostSpecific);
	virtual	void				Warn(status_t error, const char* format, ...);

	virtual	void				ProgressPackageDownloadStarted(
									const char* packageName);
	virtual	void				ProgressPackageDownloadActive(
									const char* packageName,
									float completionValue,
									off_t bytes, off_t totalBytes);
	virtual	void				ProgressPackageDownloadComplete(
									const char* packageName);
	virtual	void				ProgressPackageChecksumStarted(
									const char* packageName);
	virtual	void				ProgressPackageChecksumComplete(
									const char* packageName);

	virtual	void				ProgressStartApplyingChanges(
									InstalledRepository& repository);
	virtual	void				ProgressTransactionCommitted(
									InstalledRepository& repository,
									const BPackageKit::BCommitTransactionResult& result);
	virtual	void				ProgressApplyingChangesDone(
									InstalledRepository& repository);

private:
			void				_PrintResult(InstalledRepository&
									installationRepository,
									int32& upgradeCount,
									int32& installCount,
									int32& uninstallCount);
			void				_UpdateStatusWindow(const char* header,
									const char* detail);
			void				_UpdateDownloadProgress(const char* header,
									const char* packageName,
									float percentageComplete);
			void				_FinalUpdate(const char* header,
									const char* text);
			void				_SetCurrentStep(int32 step);

private:
			BPackageManager::ClientInstallationInterface
									fClientInstallationInterface;

			SoftwareUpdaterWindow*	fStatusWindow;
			ProblemWindow*			fProblemWindow;
			uint32					fCurrentStep;
			bool					fChangesConfirmed;
			bool					fNewDownloadStarted;
			int32					fPackageDownloadsTotal;
			int32					fPackageDownloadsCount;
			bool					fVerbose;
};


#endif	// UPDATE_MANAGER_H
