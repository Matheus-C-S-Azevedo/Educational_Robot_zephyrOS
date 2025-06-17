#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>
#include "wifi_connect.h"
#include "config.h"

LOG_MODULE_REGISTER(wifi_connect, LOG_LEVEL_INF);

/* STA Mode Configuration */
static struct net_if *sta_iface;
static struct wifi_connect_req_params sta_config; // cria uma dado com a estrutura wifi_ conec... que vai ser usado em connect_to_wifi()
static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ip_cb;

volatile bool wifi_conectado = false;

// Este método só é utilizado para gerar logs, em uma versão mais enxuta, poderia ser removida (pode ser utilizada para fazer reconexão)
static void wifi_event_handler(struct net_mgmt_event_callback *cb,
                               uint32_t mgmt_event,
                               struct net_if *iface)
{
    switch (mgmt_event) {
    case NET_EVENT_WIFI_CONNECT_RESULT:
        LOG_INF("Wi-Fi conectado, aguardando IP...");
        break;

    case NET_EVENT_WIFI_DISCONNECT_RESULT:
        wifi_conectado = false;
        LOG_WRN("Wi-Fi desconectado.");
        break;
    }
}

static void ip_event_handler(struct net_mgmt_event_callback *cb,
                             uint32_t mgmt_event,
                             struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_IPV4_ADDR_ADD) {
        wifi_conectado = true;
        LOG_INF("Endereço IP atribuído com sucesso!");
    }
}

int connect_to_wifi(void)
{

    sta_iface = net_if_get_wifi_sta();

	if (!sta_iface) {
		LOG_INF("STA: interface no initialized");
		return -EIO;
	}

	net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler, NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
	net_mgmt_add_event_callback(&wifi_cb);

	net_mgmt_init_event_callback(&ip_cb, ip_event_handler, NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&ip_cb);

	sta_config.ssid = (const uint8_t *)WIFI_SSID;
	sta_config.ssid_length = strlen(WIFI_SSID);
	sta_config.psk = (const uint8_t *)WIFI_PSK;
	sta_config.psk_length = strlen(WIFI_PSK);
	sta_config.security = WIFI_SECURITY_TYPE_PSK;
	sta_config.channel = WIFI_CHANNEL_ANY;
	sta_config.band = WIFI_FREQ_BAND_2_4_GHZ;

	LOG_INF("Connecting to SSID: %s\n", sta_config.ssid);

	int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, sta_iface, &sta_config, sizeof(struct wifi_connect_req_params));
	if (ret) {
		LOG_ERR("Unable to Connect to (%s)", WIFI_SSID);
	}

	return ret;
}


