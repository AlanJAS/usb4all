/* No executable code. These checks are compiled by the target compiler. */
#include <stddef.h>
#include "system/usb/usb.h"
#include "user/adminModule.h"
#include "user/pnp.h"
#include "user/port.h"
#include "user/usr_ax.h"
#include "user/usr_button.h"
#include "user/usr_hackpoints.h"
#include "user/usr_motors.h"
#include "user/usr_butia.h"
#include "user/usr_modAct.h"
#include "user/usr_modSen.h"
#define CHECK(name, expression) typedef char name[(expression) ? 1 : -1]
CHECK(byte_width, sizeof(byte) == 1);
CHECK(word_width, sizeof(word) == 2);
CHECK(dword_width, sizeof(dword) == 4);
CHECK(bd_width, sizeof(BDT) == 4);
CHECK(bd_address_offset, __builtin_offsetof(BDT, ADR) == 2);
CHECK(bd_address_width, sizeof(((BDT *)0)->ADR) == 2);
CHECK(setup_width, sizeof(CTRL_TRF_SETUP) == 8);
CHECK(control_width, sizeof(CTRL_TRF_DATA) == EP0_BUFF_SIZE);
CHECK(device_descriptor_width, sizeof(USB_DEV_DSC) == 18);
CHECK(config_descriptor_width, sizeof(USB_CFG_DSC) == 9);
CHECK(interface_descriptor_width, sizeof(USB_INTF_DSC) == 9);
CHECK(endpoint_descriptor_width, sizeof(USB_EP_DSC) == 7);
CHECK(configuration_width, sizeof(CFG01) == 32);
CHECK(handler_header_width, sizeof(HM_DATA_PACKET_HEADER) == 3);
CHECK(handler_operation_width, sizeof(HANDLER_OPTYPE) == 1);
CHECK(admin_cmd_offset, __builtin_offsetof(AM_PACKET, CMD) == 0);
CHECK(admin_payload_offset, __builtin_offsetof(AM_PACKET, payload) == 1);
CHECK(admin_module_offset, __builtin_offsetof(AM_PACKET, moduleId) == 3);
CHECK(admin_response_offset, __builtin_offsetof(AM_PACKET, response) == 1);
CHECK(admin_response_width, sizeof(((AM_PACKET *)0)->response) == 1);
CHECK(admin_packet_width, sizeof(AM_PACKET) == HM_PACKET_SIZE);
#define PACKET_CHECK(type) \
    CHECK(type##_width, sizeof(type) == USBGEN_EP_SIZE); \
    CHECK(type##_cmd_width, sizeof(((type *)0)->CMD) == 1); \
    CHECK(type##_len_offset, __builtin_offsetof(type, len) == 1)
PACKET_CHECK(PNP_DATA_PACKET);
PACKET_CHECK(PORT_DATA_PACKET);
PACKET_CHECK(AX_DATA_PACKET);
PACKET_CHECK(BUTTON_DATA_PACKET);
PACKET_CHECK(HACK_POINTS_DATA_PACKET);
PACKET_CHECK(MOTORS_DATA_PACKET);
PACKET_CHECK(BUTIA_DATA_PACKET);
PACKET_CHECK(MODACT_DATA_PACKET);
PACKET_CHECK(MODSEN_DATA_PACKET);
