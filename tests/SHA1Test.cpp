/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "Test.h"
#include "SkSHA1.h"

static bool digests_equal(const SkSHA1::Digest& expectedDigest, const SkSHA1::Digest& computedDigest) {
    for (size_t i = 0; i < SK_ARRAY_COUNT(expectedDigest.data); ++i) {
        if (expectedDigest.data[i] != computedDigest.data[i]) {
            return false;
        }
    }
    return true;
}

static struct SHA1Test {
    const char* message;
    const unsigned long int repeatCount;
    const SkSHA1::Digest digest;
} sha1_tests[] = {
    // Reference tests from RFC3174 Section 7.3 ( http://www.ietf.org/rfc/rfc3174.txt )
    { "abc", 1, {{ 0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D }} },
    { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 1, {{ 0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE, 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1 }} },
    { "a", 1000000, {{ 0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4, 0xF6, 0x1E, 0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6F }} },
    { "0123456701234567012345670123456701234567012345670123456701234567", 10, {{ 0xDE, 0xA3, 0x56, 0xA2, 0xCD, 0xDD, 0x90, 0xC7, 0xA7, 0xEC, 0xED, 0xC5, 0xEB, 0xB5, 0x63, 0x93, 0x4F, 0x46, 0x04, 0x52 }} },

    // Reference tests from running GNU sha1sum on test input
    { "The quick brown fox jumps over the lazy dog", 1, {{ 0x2f, 0xd4, 0xe1, 0xc6, 0x7a, 0x2d, 0x28, 0xfc, 0xed, 0x84, 0x9e, 0xe1, 0xbb, 0x76, 0xe7, 0x39, 0x1b, 0x93, 0xeb, 0x12 }} },
    { "", 1, {{ 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09 }} },
};

static void sha1_test(const SHA1Test& test, skiatest::Reporter* reporter) {
    size_t len = strlen(test.message);

    SkSHA1 context;
    for (unsigned long int i = 0; i < test.repeatCount; ++i) {
        context.update(reinterpret_cast<const uint8_t*>(test.message), len);
    }
    SkSHA1::Digest digest;
    context.finish(digest);

    REPORTER_ASSERT(reporter, digests_equal(test.digest, digest));
}

static void TestSHA1(skiatest::Reporter* reporter) {
    for (size_t i = 0; i < SK_ARRAY_COUNT(sha1_tests); ++i) {
        sha1_test(sha1_tests[i], reporter);
    }
}

#include "TestClassDef.h"
DEFINE_TESTCLASS("SHA1", SHA1TestClass, TestSHA1)
