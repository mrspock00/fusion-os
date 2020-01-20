/*
 * Copyright 2009-2011, Michael Lotz, mmlr@mlotz.ch.
 * Distributed under the terms of the MIT License.
 */
#ifndef PROTOCOL_HANDLER_H
#define PROTOCOL_HANDLER_H


#include <SupportDefs.h>


#define PROTOCOL_HANDLER_COOKIE_FLAG_CLOSED		0x80000000


class HIDDevice;
class HIDReport;


class ProtocolHandler {
public:
								ProtocolHandler(HIDDevice *device,
									const char *basePath,
									size_t ringBufferSize);
	virtual						~ProtocolHandler();

			status_t			InitCheck() { return fStatus; }

			HIDDevice *			Device() { return fDevice; }

			const char *		BasePath() { return fBasePath; }
			void				SetPublishPath(char *publishPath);
			const char *		PublishPath() { return fPublishPath; }

	static	void				AddHandlers(HIDDevice &device,
									ProtocolHandler *&handlerList,
									uint32 &handlerCount);

	virtual	status_t			Open(uint32 flags, uint32 *cookie);
	virtual	status_t			Close(uint32 *cookie);

	virtual	status_t			Read(uint32 *cookie, off_t position,
									void *buffer, size_t *numBytes);
	virtual	status_t			Write(uint32 *cookie, off_t position,
									const void *buffer, size_t *numBytes);

	virtual	status_t			Control(uint32 *cookie, uint32 op, void *buffer,
									size_t length);

			int32				RingBufferReadable();
			status_t			RingBufferRead(void *buffer, size_t length);
			status_t			RingBufferWrite(const void *buffer,
									size_t length);

			void				SetNextHandler(ProtocolHandler *next);
			ProtocolHandler *	NextHandler() { return fNextHandler; };

protected:
			status_t			fStatus;

private:
			HIDDevice *			fDevice;
			const char *		fBasePath;
			char *				fPublishPath;
			struct ring_buffer *fRingBuffer;

			ProtocolHandler *	fNextHandler;
};


#endif // PROTOCOL_HANDLER_H
