/*
 * Copyright 2019, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Augustin Cavalier <waddlesplash>
 */


#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <AutoDeleter.h>
#include <kernel.h>
#include <util/AutoLock.h>

#include <fs/devfs.h>
#include <bus/PCI.h>

extern "C" {
#include <libnvme/nvme.h>
}


//#define TRACE_NVME_DISK
#ifdef TRACE_NVME_DISK
#	define TRACE(x...) dprintf("nvme_disk: " x)
#else
#	define TRACE(x...) ;
#endif
#define TRACE_ALWAYS(x...)	dprintf("nvme_disk: " x)
#define TRACE_ERROR(x...)	dprintf("\33[33mnvme_disk:\33[0m " x)
#define CALLED() 			TRACE("CALLED %s\n", __PRETTY_FUNCTION__)


static const uint8 kDriveIcon[] = {
	0x6e, 0x63, 0x69, 0x66, 0x08, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x16,
	0x02, 0x3c, 0xc7, 0xee, 0x38, 0x9b, 0xc0, 0xba, 0x16, 0x57, 0x3e, 0x39,
	0xb0, 0x49, 0x77, 0xc8, 0x42, 0xad, 0xc7, 0x00, 0xff, 0xff, 0xd3, 0x02,
	0x00, 0x06, 0x02, 0x3c, 0x96, 0x32, 0x3a, 0x4d, 0x3f, 0xba, 0xfc, 0x01,
	0x3d, 0x5a, 0x97, 0x4b, 0x57, 0xa5, 0x49, 0x84, 0x4d, 0x00, 0x47, 0x47,
	0x47, 0xff, 0xa5, 0xa0, 0xa0, 0x02, 0x00, 0x16, 0x02, 0xbc, 0x59, 0x2f,
	0xbb, 0x29, 0xa7, 0x3c, 0x0c, 0xe4, 0xbd, 0x0b, 0x7c, 0x48, 0x92, 0xc0,
	0x4b, 0x79, 0x66, 0x00, 0x7d, 0xff, 0xd4, 0x02, 0x00, 0x06, 0x02, 0x38,
	0xdb, 0xb4, 0x39, 0x97, 0x33, 0xbc, 0x4a, 0x33, 0x3b, 0xa5, 0x42, 0x48,
	0x6e, 0x66, 0x49, 0xee, 0x7b, 0x00, 0x59, 0x67, 0x56, 0xff, 0xeb, 0xb2,
	0xb2, 0x03, 0xa7, 0xff, 0x00, 0x03, 0xff, 0x00, 0x00, 0x04, 0x01, 0x80,
	0x07, 0x0a, 0x06, 0x22, 0x3c, 0x22, 0x49, 0x44, 0x5b, 0x5a, 0x3e, 0x5a,
	0x31, 0x39, 0x25, 0x0a, 0x04, 0x22, 0x3c, 0x44, 0x4b, 0x5a, 0x31, 0x39,
	0x25, 0x0a, 0x04, 0x44, 0x4b, 0x44, 0x5b, 0x5a, 0x3e, 0x5a, 0x31, 0x0a,
	0x04, 0x22, 0x3c, 0x22, 0x49, 0x44, 0x5b, 0x44, 0x4b, 0x08, 0x02, 0x27,
	0x43, 0xb8, 0x14, 0xc1, 0xf1, 0x08, 0x02, 0x26, 0x43, 0x29, 0x44, 0x0a,
	0x05, 0x44, 0x5d, 0x49, 0x5d, 0x60, 0x3e, 0x5a, 0x3b, 0x5b, 0x3f, 0x08,
	0x0a, 0x07, 0x01, 0x06, 0x00, 0x0a, 0x00, 0x01, 0x00, 0x10, 0x01, 0x17,
	0x84, 0x00, 0x04, 0x0a, 0x01, 0x01, 0x01, 0x00, 0x0a, 0x02, 0x01, 0x02,
	0x00, 0x0a, 0x03, 0x01, 0x03, 0x00, 0x0a, 0x04, 0x01, 0x04, 0x10, 0x01,
	0x17, 0x85, 0x20, 0x04, 0x0a, 0x06, 0x01, 0x05, 0x30, 0x24, 0xb3, 0x99,
	0x01, 0x17, 0x82, 0x00, 0x04, 0x0a, 0x05, 0x01, 0x05, 0x30, 0x20, 0xb2,
	0xe6, 0x01, 0x17, 0x82, 0x00, 0x04
};


#define NVME_DISK_DRIVER_MODULE_NAME 	"drivers/disk/nvme_disk/driver_v1"
#define NVME_DISK_DEVICE_MODULE_NAME 	"drivers/disk/nvme_disk/device_v1"
#define NVME_DISK_DEVICE_ID_GENERATOR	"nvme_disk/device_id"

#define NVME_MAX_QPAIRS					(8)


static device_manager_info* sDeviceManager;

typedef struct {
	device_node*			node;
	pci_info				info;

	struct nvme_ctrlr*		ctrlr;

	struct nvme_ns*			ns;
	uint64					capacity;
	uint32					block_size;
	size_t					max_transfer_size;
	status_t				media_status;

	struct qpair_info {
		struct nvme_qpair*	qpair;
		mutex				mtx;
	}						qpairs[NVME_MAX_QPAIRS];
	uint32					qpair_count;
	uint32					next_qpair;
} nvme_disk_driver_info;
typedef nvme_disk_driver_info::qpair_info qpair_info;


typedef struct {
	nvme_disk_driver_info*		info;
} nvme_disk_handle;


static status_t
get_geometry(nvme_disk_handle* handle, device_geometry* geometry)
{
	nvme_disk_driver_info* info = handle->info;

	devfs_compute_geometry_size(geometry, info->capacity, info->block_size);

	geometry->device_type = B_DISK;
	geometry->removable = false;

	geometry->read_only = false;
	geometry->write_once = false;

	TRACE("get_geometry(): %" B_PRId32 ", %" B_PRId32 ", %" B_PRId32 ", %" B_PRId32 ", %d, %d, %d, %d\n",
		geometry->bytes_per_sector, geometry->sectors_per_track,
		geometry->cylinder_count, geometry->head_count, geometry->device_type,
		geometry->removable, geometry->read_only, geometry->write_once);

	return B_OK;
}


static int
log2(uint32 x)
{
	int y;

	for (y = 31; y >= 0; --y) {
		if (x == ((uint32)1 << y))
			break;
	}

	return y;
}


static void
nvme_disk_set_capacity(nvme_disk_driver_info* info, uint64 capacity,
	uint32 blockSize)
{
	TRACE("set_capacity(device = %p, capacity = %" B_PRIu64 ", blockSize = %" B_PRIu32 ")\n",
		info, capacity, blockSize);

	// get log2, if possible
	uint32 blockShift = log2(blockSize);

	if ((1UL << blockShift) != blockSize)
		blockShift = 0;

	info->capacity = capacity;
	info->block_size = blockSize;
}


//	#pragma mark - device module API


static status_t
nvme_disk_init_device(void* _info, void** _cookie)
{
	CALLED();
	nvme_disk_driver_info* info = (nvme_disk_driver_info*)_info;

	pci_device_module_info* pci;
	pci_device* pcidev;
	device_node* parent = sDeviceManager->get_parent_node(info->node);
	sDeviceManager->get_driver(parent, (driver_module_info**)&pci,
		(void**)&pcidev);
	pci->get_pci_info(pcidev, &info->info);
	sDeviceManager->put_node(parent);

	// construct the libnvme pci_device struct
	pci_device* device = new pci_device;
	device->vendor_id = info->info.vendor_id;
	device->device_id = info->info.device_id;
	device->subvendor_id = 0;
	device->subdevice_id = 0;

	device->domain = 0;
	device->bus = info->info.bus;
	device->dev = info->info.device;
	device->func = info->info.function;

	device->pci_info = &info->info;

	// open the controller
	info->ctrlr = nvme_ctrlr_open(device, NULL);
	if (info->ctrlr == NULL) {
		TRACE_ERROR("failed to open the controller!\n");
		return B_ERROR;
	}

	struct nvme_ctrlr_stat cstat;
	int err = nvme_ctrlr_stat(info->ctrlr, &cstat);
	if (err != 0) {
		TRACE_ERROR("failed to get controller information!\n");
		return err;
	}

	TRACE_ALWAYS("attached to NVMe device \"%s (%s)\"\n", cstat.mn, cstat.sn);
	TRACE_ALWAYS("\tmaximum transfer size: %" B_PRIuSIZE "\n", cstat.max_xfer_size);
	TRACE_ALWAYS("\tqpair count: %d\n", cstat.io_qpairs);

	// TODO: export more than just the first namespace!
	info->ns = nvme_ns_open(info->ctrlr, cstat.ns_ids[0]);
	if (info->ns == NULL) {
		TRACE_ERROR("failed to open namespace!\n");
		return B_ERROR;
	}

	struct nvme_ns_stat nsstat;
	err = nvme_ns_stat(info->ns, &nsstat);
	if (err != 0) {
		TRACE_ERROR("failed to get namespace information!\n");
		return err;
	}

	// store capacity information
	nvme_disk_set_capacity(info, nsstat.sectors, nsstat.sector_size);
	info->max_transfer_size = ROUNDDOWN(cstat.max_xfer_size,
		nsstat.sector_size);

	TRACE("capacity: %" B_PRIu64 ", block_size %" B_PRIu32 "\n",
		info->capacity, info->block_size);

	// allocate qpairs
	info->qpair_count = info->next_qpair = 0;
	for (uint32 i = 0; i < NVME_MAX_QPAIRS && i < cstat.io_qpairs; i++) {
		info->qpairs[i].qpair = nvme_ioqp_get(info->ctrlr,
			(enum nvme_qprio)0, 0);
		if (info->qpairs[i].qpair == NULL)
			break;

		mutex_init(&info->qpairs[i].mtx, "qpair mutex");
		info->qpair_count++;
	}
	if (info->qpair_count == 0) {
		TRACE_ERROR("failed to allocate qpairs!\n");
		return B_NO_MEMORY;
	}

	*_cookie = info;
	return B_OK;
}


static void
nvme_disk_uninit_device(void* _cookie)
{
	CALLED();
	nvme_disk_driver_info* info = (nvme_disk_driver_info*)_cookie;
}


static status_t
nvme_disk_open(void* _info, const char* path, int openMode, void** _cookie)
{
	CALLED();

	nvme_disk_driver_info* info = (nvme_disk_driver_info*)_info;
	nvme_disk_handle* handle = (nvme_disk_handle*)malloc(
		sizeof(nvme_disk_handle));
	if (handle == NULL)
		return B_NO_MEMORY;

	handle->info = info;

	*_cookie = handle;
	return B_OK;
}


static status_t
nvme_disk_close(void* cookie)
{
	CALLED();

	nvme_disk_handle* handle = (nvme_disk_handle*)cookie;
	return B_OK;
}


static status_t
nvme_disk_free(void* cookie)
{
	CALLED();

	nvme_disk_handle* handle = (nvme_disk_handle*)cookie;
	free(handle);
	return B_OK;
}


// #pragma mark - I/O functions


static qpair_info*
get_next_qpair(nvme_disk_driver_info* info)
{
	return &info->qpairs[atomic_add((int32*)&info->next_qpair, 1)
		% info->qpair_count];
}


static void
disk_io_callback(status_t* status, const struct nvme_cpl* cpl)
{
	*status = nvme_cpl_is_error(cpl) ? B_IO_ERROR : B_OK;
}


static void
await_status(struct nvme_qpair* qpair, status_t& status)
{
	while (status == EINPROGRESS) {
		// nvme_ioqp_poll uses locking internally on the entire device,
		// not just this qpair, so it is entirely possible that it could
		// return 0 (i.e. no completions processed) and yet our status
		// changed, because some other thread processed the completion
		// before we got to it. So, recheck it before sleeping.
		if (nvme_ioqp_poll(qpair, 0) == 0 && status == EINPROGRESS)
			snooze(5);
	}
}


static status_t
do_nvme_io(nvme_disk_driver_info* info, off_t rounded_pos, void* buffer,
	size_t* rounded_len, bool write = false)
{
	CALLED();
	const size_t block_size = info->block_size;

	status_t status = EINPROGRESS;

	qpair_info* qpinfo = get_next_qpair(info);
	mutex_lock(&qpinfo->mtx);
	int ret = -1;
	if (write) {
		ret = nvme_ns_write(info->ns, qpinfo->qpair, buffer,
			rounded_pos / block_size, *rounded_len / block_size,
			(nvme_cmd_cb)disk_io_callback, &status, 0);
	} else {
		ret = nvme_ns_read(info->ns, qpinfo->qpair, buffer,
			rounded_pos / block_size, *rounded_len / block_size,
			(nvme_cmd_cb)disk_io_callback, &status, 0);
	}
	mutex_unlock(&qpinfo->mtx);
	if (ret != 0) {
		TRACE_ERROR("attempt to queue %s I/O at %" B_PRIdOFF " of %" B_PRIuSIZE
			" bytes failed!", write ? "write" : "read", rounded_pos, *rounded_len);
		return ret;
	}

	await_status(qpinfo->qpair, status);

	if (status != B_OK) {
		TRACE_ERROR("%s at %" B_PRIdOFF " of %" B_PRIuSIZE " bytes failed!",
			write ? "write" : "read", rounded_pos, *rounded_len);
		*rounded_len = 0;
	}
	return status;
}


static status_t
do_nvme_segmented_io(nvme_disk_driver_info* info, off_t rounded_pos,
	void* buffer, size_t* rounded_len, bool write = false)
{
	// The max transfer size is already a multiple of the block size,
	// so divide and iterate appropriately. In the case where the length
	// is less than the maximum transfer size, we'll wind up with 0 in the
	// division, and only one transfer to take care of.
	const size_t max_xfer = info->max_transfer_size;
	int32 transfers = *rounded_len / max_xfer;
	if ((*rounded_len % max_xfer) != 0)
		transfers++;

	size_t transferred = 0;
	for (int32 i = 0; i < transfers; i++) {
		size_t transfer_len = max_xfer;
		// The last transfer will usually be smaller.
		if (i == (transfers - 1))
			transfer_len = *rounded_len - transferred;

		status_t status = do_nvme_io(info, rounded_pos, buffer,
			&transfer_len, write);
		if (status != B_OK) {
			*rounded_len = transferred;
			return transferred > 0 ? (write ? B_PARTIAL_WRITE : B_PARTIAL_READ)
				: status;
		}

		transferred += transfer_len;
		rounded_pos += transfer_len;
		buffer = ((int8*)buffer) + transfer_len;
	}
	return B_OK;
}


static status_t
nvme_disk_read(void* cookie, off_t pos, void* buffer, size_t* length)
{
	CALLED();
	nvme_disk_handle* handle = (nvme_disk_handle*)cookie;
	const size_t block_size = handle->info->block_size;

	// libnvme does transfers in units of device sectors, so if we have to
	// round either the position or the length, we will need a bounce buffer.
	const off_t rounded_pos = ROUNDDOWN(pos, block_size);
	size_t rounded_len = ROUNDUP((*length) + (pos - rounded_pos), block_size);
	if (rounded_pos != pos || rounded_len != *length
			|| IS_USER_ADDRESS(buffer)) {
		void* bounceBuffer = malloc(rounded_len);
		MemoryDeleter _(bounceBuffer);
		if (bounceBuffer == NULL) {
			*length = 0;
			return B_NO_MEMORY;
		}

		status_t status = do_nvme_segmented_io(handle->info, rounded_pos,
			bounceBuffer, &rounded_len);
		if (status != B_OK) {
			// The "rounded_len" will be the actual transferred length, but
			// of course it will contain the padding.
			*length = std::min(*length, (size_t)std::max((off_t)0,
				(off_t)rounded_len - (off_t)(pos - rounded_pos)));
			if (*length == 0)
				return status;
		}

		void* offsetBuffer = ((int8*)bounceBuffer) + (pos - rounded_pos);
		if (IS_USER_ADDRESS(buffer))
			status = user_memcpy(buffer, offsetBuffer, *length);
		else
			memcpy(buffer, offsetBuffer, *length);
		return status;
	}

	// If we got here, that means the arguments are already rounded to LBAs
	// and the buffer is a kernel one, so just do the I/O directly.
	return do_nvme_segmented_io(handle->info, pos, buffer, length);
}


static status_t
nvme_disk_write(void* cookie, off_t pos, const void* buffer, size_t* length)
{
	CALLED();
	nvme_disk_handle* handle = (nvme_disk_handle*)cookie;
	const size_t block_size = handle->info->block_size;

	const off_t rounded_pos = ROUNDDOWN(pos, block_size);
	size_t rounded_len = ROUNDUP((*length) + (pos - rounded_pos), block_size);
	if (rounded_pos != pos || rounded_len != *length
			|| IS_USER_ADDRESS(buffer)) {
		void* bounceBuffer = malloc(rounded_len);
		MemoryDeleter _(bounceBuffer);
		if (bounceBuffer == NULL) {
			*length = 0;
			return B_NO_MEMORY;
		}

		// Since we rounded, we need to read in the first and last logical
		// blocks before we copy our information to the bounce buffer.
		// TODO: This would be faster if we queued both reads at once!
		size_t readlen = block_size;
		status_t status;
		if (rounded_pos != pos) {
			status = do_nvme_io(handle->info, rounded_pos, bounceBuffer,
				&readlen);
			if (status != B_OK) {
				*length = 0;
				return status;
			}
		}
		if (rounded_len > block_size) {
			off_t offset = rounded_len - block_size;
			status = do_nvme_io(handle->info, rounded_pos + offset,
				((int8*)bounceBuffer) + offset, &readlen);
			if (status != B_OK) {
				*length = 0;
				return status;
			}
		}

		// Now we can copy in the actual data to be written.
		void* offsetBuffer = ((int8*)bounceBuffer) + (pos - rounded_pos);
		if (IS_USER_ADDRESS(buffer))
			status = user_memcpy(offsetBuffer, buffer, *length);
		else
			memcpy(offsetBuffer, buffer, *length);
		if (status != B_OK) {
			*length = 0;
			return status;
		}

		status = do_nvme_segmented_io(handle->info, rounded_pos, bounceBuffer,
			&rounded_len, true);
		if (status != B_OK) {
			*length = std::min(*length, (size_t)std::max((off_t)0,
				(off_t)rounded_len - (off_t)(pos - rounded_pos)));
		}
		return status;
	}

	// If we got here, that means the arguments are already rounded to LBAs,
	// so just do the I/O directly.
	return do_nvme_segmented_io(handle->info, pos, (void*)buffer, length, true);
}


static status_t
nvme_disk_flush(nvme_disk_driver_info* info)
{
	status_t status = EINPROGRESS;

	qpair_info* qpinfo = get_next_qpair(info);
	mutex_lock(&qpinfo->mtx);
	int ret = nvme_ns_flush(info->ns, qpinfo->qpair,
		(nvme_cmd_cb)disk_io_callback, &status);
	mutex_unlock(&qpinfo->mtx);
	if (ret != 0)
		return ret;

	await_status(qpinfo->qpair, status);
	return status;
}


static status_t
nvme_disk_ioctl(void* cookie, uint32 op, void* buffer, size_t length)
{
	CALLED();
	nvme_disk_handle* handle = (nvme_disk_handle*)cookie;
	nvme_disk_driver_info* info = handle->info;

	TRACE("ioctl(op = %" B_PRId32 ")\n", op);

	switch (op) {
		case B_GET_MEDIA_STATUS:
		{
			*(status_t *)buffer = info->media_status;
			info->media_status = B_OK;
			return B_OK;
			break;
		}

		case B_GET_DEVICE_SIZE:
		{
			size_t size = info->capacity * info->block_size;
			return user_memcpy(buffer, &size, sizeof(size_t));
		}

		case B_GET_GEOMETRY:
		{
			if (buffer == NULL /*|| length != sizeof(device_geometry)*/)
				return B_BAD_VALUE;

		 	device_geometry geometry;
			status_t status = get_geometry(handle, &geometry);
			if (status != B_OK)
				return status;

			return user_memcpy(buffer, &geometry, sizeof(device_geometry));
		}

		case B_GET_ICON_NAME:
			return user_strlcpy((char*)buffer, "devices/drive-harddisk",
				B_FILE_NAME_LENGTH);

		case B_GET_VECTOR_ICON:
		{
			device_icon iconData;
			if (length != sizeof(device_icon))
				return B_BAD_VALUE;
			if (user_memcpy(&iconData, buffer, sizeof(device_icon)) != B_OK)
				return B_BAD_ADDRESS;

			if (iconData.icon_size >= (int32)sizeof(kDriveIcon)) {
				if (user_memcpy(iconData.icon_data, kDriveIcon,
						sizeof(kDriveIcon)) != B_OK)
					return B_BAD_ADDRESS;
			}

			iconData.icon_size = sizeof(kDriveIcon);
			return user_memcpy(buffer, &iconData, sizeof(device_icon));
		}

		case B_FLUSH_DRIVE_CACHE:
			return nvme_disk_flush(info);
	}

	return B_DEV_INVALID_IOCTL;
}


//	#pragma mark - driver module API


static float
nvme_disk_supports_device(device_node *parent)
{
	CALLED();

	const char* bus;
	uint16 baseClass, subClass;

	if (sDeviceManager->get_attr_string(parent, B_DEVICE_BUS, &bus, false) != B_OK
		|| sDeviceManager->get_attr_uint16(parent, B_DEVICE_TYPE, &baseClass, false) != B_OK
		|| sDeviceManager->get_attr_uint16(parent, B_DEVICE_SUB_TYPE, &subClass, false) != B_OK)
		return -1.0f;

	if (strcmp(bus, "pci") != 0 || baseClass != PCI_mass_storage)
		return 0.0f;

	if (subClass != PCI_nvm)
		return 0.0f;

	TRACE("NVMe device found!\n");
	return 1.0f;
}


static status_t
nvme_disk_register_device(device_node* parent)
{
	CALLED();

	device_attr attrs[] = {
		{ NULL }
	};

	return sDeviceManager->register_node(parent, NVME_DISK_DRIVER_MODULE_NAME,
		attrs, NULL, NULL);
}


static status_t
nvme_disk_init_driver(device_node* node, void** cookie)
{
	CALLED();

	int ret = nvme_lib_init((enum nvme_log_level)0, (enum nvme_log_facility)0, NULL);
	if (ret != 0) {
		TRACE_ERROR("libnvme initialization failed!\n");
		return ret;
	}

	nvme_disk_driver_info* info = (nvme_disk_driver_info*)malloc(
		sizeof(nvme_disk_driver_info));
	if (info == NULL)
		return B_NO_MEMORY;

	memset(info, 0, sizeof(*info));

	info->media_status = B_OK;
	info->node = node;

	*cookie = info;
	return B_OK;
}


static void
nvme_disk_uninit_driver(void* _cookie)
{
	CALLED();

	nvme_disk_driver_info* info = (nvme_disk_driver_info*)_cookie;
	free(info);
}


static status_t
nvme_disk_register_child_devices(void* _cookie)
{
	CALLED();

	nvme_disk_driver_info* info = (nvme_disk_driver_info*)_cookie;
	status_t status;

	int32 id = sDeviceManager->create_id(NVME_DISK_DEVICE_ID_GENERATOR);
	if (id < 0)
		return id;

	char name[64];
	snprintf(name, sizeof(name), "disk/nvme/%" B_PRId32 "/raw",
		id);

	status = sDeviceManager->publish_device(info->node, name,
		NVME_DISK_DEVICE_MODULE_NAME);

	return status;
}


//	#pragma mark -


module_dependency module_dependencies[] = {
	{B_DEVICE_MANAGER_MODULE_NAME, (module_info**)&sDeviceManager},
	{}
};

struct device_module_info sNvmeDiskDevice = {
	{
		NVME_DISK_DEVICE_MODULE_NAME,
		0,
		NULL
	},

	nvme_disk_init_device,
	nvme_disk_uninit_device,
	NULL, // remove,

	nvme_disk_open,
	nvme_disk_close,
	nvme_disk_free,
	nvme_disk_read,
	nvme_disk_write,
	NULL,
	nvme_disk_ioctl,

	NULL,	// select
	NULL,	// deselect
};

struct driver_module_info sNvmeDiskDriver = {
	{
		NVME_DISK_DRIVER_MODULE_NAME,
		0,
		NULL
	},

	nvme_disk_supports_device,
	nvme_disk_register_device,
	nvme_disk_init_driver,
	nvme_disk_uninit_driver,
	nvme_disk_register_child_devices,
	NULL,	// rescan
	NULL,	// removed
};

module_info* modules[] = {
	(module_info*)&sNvmeDiskDriver,
	(module_info*)&sNvmeDiskDevice,
	NULL
};
