/*
 * Copyright 2020 Intel Corporation
 * SPDX-License-Identifier: Apache 2.0
 */

/*
 * Abstraction Layer
 *
 * This file is a header implementation of network abstraction layer for Linux
 * OS and ESP32.
 *
 */
#ifndef __NETWORK_AL_H__
#define __NETWORK_AL_H__

#include "fdotypes.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#define IPV4_ADDR_LEN 4

#ifndef TARGET_OS_MBEDOS
typedef void *fdo_con_handle;
#define FDO_CON_INVALID_HANDLE NULL
#endif

#if defined(TARGET_OS_MBEDOS)
#include "mbed_net_al.h"
#endif

/*
 * Network Connection Setup.
 *
 * @param[in] medium: specified network medium to connect to.
 * @param[in] params: parameters(if any) supported for 'medium'.
 * @param[in] count: number of valid string in params
 * @retval -1 on failure, 0 on success.
 */
int32_t fdo_con_setup(char *medium, char **params, uint32_t count);

/*
 * Perform a DNS look for a specified host.
 *
 * @param[in] url: host's URL.
 * @param[out] ip_list: IP address list for specified host URL.
 * @param[out] ip_list_size: number of IP address in ip_list
 * @retval -1 on failure, 0 on success.
 */
int32_t fdo_con_dns_lookup(const char *url, fdo_ip_address_t **ip_list,
			   uint32_t *ip_list_size);

/*
 * Open a connection specified by IP address and port.
 *
 * @param[in] addr: IP Address to connect to.
 * @param[in] port: port number to connect to.
 * @param[in] ssl: SSL handler in case of tls connection.
 * @retval -1 on failure, connection handle on success.
 */
fdo_con_handle fdo_con_connect(fdo_ip_address_t *addr, uint16_t port,
			       void **ssl);

/*
 * Disconnect the connection.
 *
 * @param[in] handle: connection handler (for ex: socket-id)
 * @param[in] ssl: SSL handler in case of tls connection.
 * @retval -1 on failure, 0 on success.
 */
int32_t fdo_con_disconnect(fdo_con_handle handle, void *ssl);

/*
 * Receive(read) length of incoming fdo packet.
 *
 * @param[in] handle: connection handler (for ex: socket-id)
 * @param[out] protocol_version: FDO protocol version
 * @param[out] message_type: message type of incoming FDO message.
 * @param[out] msglen: length of incoming message.
 * @param[in] ssl handler in case of tls connection.
 * @retval -1 on failure, 0 on success.
 */
int32_t fdo_con_recv_msg_header(fdo_con_handle handle,
				uint32_t *protocol_version,
				uint32_t *message_type, uint32_t *msglen,
				void *ssl);

/*
 * Receive(read) incoming fdo packet.
 *
 * @param[in] handle: connection handler (for ex: socket-id)
 * @param[out] buf: data buffer to read into.
 * @param[in] length: Number of received bytes to be read.
 * @param[in] ssl handler in case of tls connection.
 * @retval -1 on failure, 0 on success.
 */
int32_t fdo_con_recv_msg_body(fdo_con_handle handle, uint8_t *buf,
			      size_t length, void *ssl);

/*
 * Send(write) data.
 *
 * @param[in] handle: connection handler (for ex: socket-id)
 * @param[in] protocol_version: FDO protocol version
 * @param[in] message_type: message type of outgoing FDO message.
 * @param[in] buf: data buffer to write from.
 * @param[in] length: Number of sent bytes.
 * @param[in] ssl handler in case of tls connection.
 * @retval -1 on failure, 0 on success.
 */
int32_t fdo_con_send_message(fdo_con_handle handle, uint32_t protocol_version,
			     uint32_t message_type, const uint8_t *buf,
			     size_t length, void *ssl);

/*
 * Network Connection tear down.
 * This API is counter to fdo_con_setup().
 *
 */
int32_t fdo_con_teardown(void);

/* put FDO device in Low power mode */
// FIXME: we might have to find a suitable place for this API
void fdo_sleep(int sec);

/* Convert from Network to Host byte order */
uint32_t fdo_net_to_host_long(uint32_t value);

/* Convert from Host to Network byte order */
uint32_t fdo_host_to_net_long(uint32_t value);

/* Convert from ASCII to Network byte order format */
int32_t fdo_printable_to_net(const char *src, void *addr);

/* get device model number */
const char *get_device_model(void);

/* get device serial number */
const char *get_device_serial_number(void);

/* generate random number */
int fdo_random(void);

#endif /* __NETWORK_AL_H__ */
