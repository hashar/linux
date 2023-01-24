/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _HID_JABRA_H
#define _HID_JABRA_H

#include <linux/hid.h>

extern int jabra_input_mapping(struct hid_device *hdev,
		struct hid_input *hi,
		struct hid_field *field,
		struct hid_usage *usage,
		unsigned long **bit, int *max);

#endif /* _HID_JABRA_H */
