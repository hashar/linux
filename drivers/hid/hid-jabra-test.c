// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Tests for the Jabra USB HID Driver
 */

#include <kunit/test.h>

#define HID_MAPPING_IGNORE_USAGE -1
#define HID_MAPPING_DEFAULT 0
#define HID_MAPPING_HANDLED 1

int hid_jabra_add(int left, int right)
{
	return left + right;
}

static void hid_jabra_add_test(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, 3, hid_jabra_add(1, 2));
	KUNIT_EXPECT_EQ(test, 5, hid_jabra_add(4, 1));
}

struct hid_jabra_vendor_case {
	const char *message;
	const unsigned int hid;
	const int expected;
};

const struct hid_jabra_vendor_case cases[] = {
	{
		.message = "Continues on generic usage",
		.hid = 0x000b002f, // Mute button pressed
		.expected = HID_MAPPING_HANDLED,
	},
	{
		.message = "Ignores lowest vendor range",
		.hid = 0xff000000,
		.expected = HID_MAPPING_IGNORE_USAGE,
	},
	{
		.message = "Ignores highest vendor range",
		.hid = 0xffff0000,
		.expected = HID_MAPPING_IGNORE_USAGE,
	}
};

static void hid_jabra_vendor_case_to_desc(const struct hid_jabra_vendor_case *c, char *desc)
{
	strcpy(desc, c->message);
}

KUNIT_ARRAY_PARAM(hid_jabra_usage_hid, cases, hid_jabra_vendor_case_to_desc);

static void hid_jabra_input_mapping_recognizes_vendor_test(struct kunit *test)
{
	struct hid_jabra_vendor_case *test_param = (struct hid_jabra_vendor_case *)(test->param_value);
	struct hid_field field = {
		.application = 0x000b0005,  // Mute button pressed
	};
	struct hid_usage usage = {
		.collection_index = 0x00,
		.usage_index = 0x00,
		.code = 0x00,
	};
	struct hid_input input;
	struct input_dev input_dev = {
		.name = "dummy_input_dev_0",
	};
	unsigned long *bit = 0;
	int max = 0;

	input.input = &input_dev;
	usage.hid = test_param->hid;

	KUNIT_EXPECT_EQ(
		test,
		test_param->expected,
		jabra_input_mapping(
			NULL, /** hid_device */
			&input, &field, &usage, &bit, &max
		)
	);
	switch (test_param->expected) {
	case HID_MAPPING_HANDLED:
		KUNIT_EXPECT_EQ(test, KEY_MICMUTE, usage.code);
	break;
	case HID_MAPPING_IGNORE_USAGE:
		KUNIT_EXPECT_EQ(test, 0, usage.code);
	break;
	}
}

static struct kunit_case hid_jabra_test_cases[] = {
	KUNIT_CASE(hid_jabra_add_test),
	KUNIT_CASE_PARAM(
		hid_jabra_input_mapping_recognizes_vendor_test,
		hid_jabra_usage_hid_gen_params
	),
};

static struct kunit_suite hid_jabra_test_suite = {
	.name = "hid_jabra_test",
	.test_cases = hid_jabra_test_cases,
};

kunit_test_suite(hid_jabra_test_suite);

MODULE_AUTHOR("Antoine Musso <hashar@free.fr>");
MODULE_DESCRIPTION("KUnit tests for the Jabra USB HID Driver");
MODULE_LICENSE("GPL");
