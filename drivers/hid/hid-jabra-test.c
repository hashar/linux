// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Tests for the Jabra USB HID Driver
 */

#include <kunit/test.h>
#include "hid-jabra.h"

int hid_jabra_add(int left, int right)
{
	return left + right;
}

static void hid_jabra_add_test(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, 3, hid_jabra_add(1, 2));
	KUNIT_EXPECT_EQ(test, 5, hid_jabra_add(4, 1));
}

static void hid_jabra_input_mapping_test(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, 0,
		jabra_input_mapping(
			NULL /** hid_device */
			, NULL /** hid_input */
			, NULL /** hid_field */
			, NULL /** hid_usage */
			, NULL /** bit */
			, 0 /** max */
		)
	);
}

static struct kunit_case hid_jabra_test_cases[] = {
	KUNIT_CASE(hid_jabra_add_test),
	KUNIT_CASE(hid_jabra_input_mapping_test),
};

static struct kunit_suite hid_jabra_test_suite = {
	.name = "hid_jabra_test",
	.test_cases = hid_jabra_test_cases,
};

kunit_test_suite(hid_jabra_test_suite);

MODULE_AUTHOR("Antoine Musso <hashar@free.fr>");
MODULE_DESCRIPTION("KUnit tests for the Jabra USB HID Driver");
MODULE_LICENSE("GPL");
