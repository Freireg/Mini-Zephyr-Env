/**
 * @file main.c
 * @author Guilherme Freire (freireg1503@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

static void start_scan(void);

static struct bt_conn *default_conn;
static struct bt_conn_info *conn_info;

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
    char addr_str[BT_ADDR_LE_STR_LEN];
    int err;

    if (default_conn)
    {
        return;
    }

    /*  Only interested in connectable events   */
    if(type != BT_GAP_ADV_TYPE_ADV_IND && 
        type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND)
    {
        return;
    }
    
    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
    printk("Device found: %s (RSSI %d)\n", addr_str, rssi);

    /*  Connect only to near devices */
    if (rssi < -70)
    {
        return;
    }

    /*  Stop the scan   */
    if (bt_le_scan_stop())
    {
        printk("Stop LE scan failed\n");
        return;
    }

    /*  Create the connection   */
    err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN,
                            BT_LE_CONN_PARAM_DEFAULT, &default_conn);
    if (err)
    {
        printk("Create conn to %s failed (%d)\n", addr_str, err);
        start_scan();
    }
    
}

static void start_scan(void)
{
    int err;
    /*  Start the scan  */
    /*  Using the device_found as the callback function */
    err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);

    if(err)
    {
        printk("Starting scanning failed (err %d)\n", err);
        return;
    }
    
    printk("Scanning successfully started\n");
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if(err)
    {
        printk("Failed to connect to %s (%u)\n", addr, err);
        bt_conn_unref(default_conn);
        default_conn = NULL;
        start_scan();
        return;
    }

    if(conn != default_conn)
    {
        return;
    }
    printk("Connected: %s\n", addr);
}

static void disconnect(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Disconnected: %s (reason %u)\n", addr, err);

    bt_conn_unref(default_conn);
    default_conn = NULL;

    start_scan();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnect,
};

int main(void)
{
    int err;

    err = bt_enable(NULL);
    if(err)
    {
        printk("Bluetooth init failed (err %d)\n", err);
        return -1;
    }

    printk("Starting Bluetoot Central\n");
    start_scan();

    while (1)
    {
        // bt_conn_get_info(conn, conn_info);
        // if (conn_info->state == BT_CONN_STATE_CONNECTED)
        // {
        //     printk("Already connected\n");
        //     k_sleep(K_MSEC(1000));
        // }
        // else
        // {
        //     printk("Not connected, starting scan\n");
        //     start_scan();
        // }
    }  
    return 0;
}