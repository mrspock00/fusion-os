/*
 * Copyright 2019, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _FBSD_COMPAT_SYS_METHOD_IDS_H
#define _FBSD_COMPAT_SYS_METHOD_IDS_H


enum device_method_ids {
	ID_device_register = 1,
	ID_device_probe,
	ID_device_attach,
	ID_device_detach,
	ID_device_suspend,
	ID_device_resume,
	ID_device_shutdown,

	ID_miibus_readreg,
	ID_miibus_writereg,
	ID_miibus_statchg,
	ID_miibus_linkchg,
	ID_miibus_mediainit,

	ID_bus_child_location_str,
	ID_bus_child_pnpinfo_str,
	ID_bus_hinted_child,
	ID_bus_print_child,
	ID_bus_read_ivar,
	ID_bus_get_dma_tag,

	ID_ifdi_knlist_add,
	ID_ifdi_knote_event,
	ID_ifdi_object_info_get,
	ID_ifdi_attach_pre,
	ID_ifdi_attach_post,
	ID_ifdi_reinit_pre,
	ID_ifdi_reinit_post,
	ID_ifdi_cloneattach,
	ID_ifdi_detach,
	ID_ifdi_suspend,
	ID_ifdi_shutdown,
	ID_ifdi_resume,
	ID_ifdi_tx_queues_alloc,
	ID_ifdi_rx_queues_alloc,
	ID_ifdi_queues_free,
	ID_ifdi_rx_clset,
	ID_ifdi_init,
	ID_ifdi_stop,
	ID_ifdi_msix_intr_assign,
	ID_ifdi_intr_enable,
	ID_ifdi_intr_disable,
	ID_ifdi_rx_queue_intr_enable,
	ID_ifdi_tx_queue_intr_enable,
	ID_ifdi_link_intr_enable,
	ID_ifdi_multi_set,
	ID_ifdi_mtu_set,
	ID_ifdi_mac_set,
	ID_ifdi_media_set,
	ID_ifdi_promisc_set,
	ID_ifdi_crcstrip_set,
	ID_ifdi_vflr_handle,
	ID_ifdi_iov_init,
	ID_ifdi_iov_uninit,
	ID_ifdi_iov_vf_add,
	ID_ifdi_update_admin_status,
	ID_ifdi_media_status,
	ID_ifdi_media_change,
	ID_ifdi_get_counter,
	ID_ifdi_priv_ioctl,
	ID_ifdi_i2c_req,
	ID_ifdi_txq_setup,
	ID_ifdi_rxq_setup,
	ID_ifdi_timer,
	ID_ifdi_watchdog_reset,
	ID_ifdi_watchdog_reset_queue,
	ID_ifdi_led_func,
	ID_ifdi_vlan_register,
	ID_ifdi_vlan_unregister,
	ID_ifdi_sysctl_int_delay,
	ID_ifdi_debug,
};


#endif /* _FBSD_COMPAT_SYS_METHOD_IDS_H */
