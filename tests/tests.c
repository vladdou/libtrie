/*
 * tests.c
 * All unit tests
 * */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../utils/miniunit.h"
#include "../src/libtrie.h"


int tests_passed = 0;
int tests_failed = 0;
uint8_t dic_path[1024] = "";


static int test_foo() {
    int res = 0;
    char foo = 7;
    U_ASSERT(res, foo == 7);
    return res;
}

static int test_char_bit_count() {
    int res = 0;
    //one bit dataset
    uint8_t dataset1[] = {
            1,
            1 << 1,
            1 << 2,
            1 << 3,
            1 << 4,
            1 << 5,
            1 << 6,
            1 << 7
    };
    //2 bit dataset
    uint8_t dataset2[] = {
            3, //11
            5, //101
            6, //011
            9, //1001
            10,// 0101
            12, //0011
            17, //10001
            18, //01001
    };

    //4 bit dataset
    uint8_t dataset3[] = {
            0xf,            //0b1111
            0xf ^ 0xff,     //0b11110000
            0xaa,           //0b10101010
            0xaa ^ 0xff,    //0b01010101
    };

    //check dataset1 1 bit

    for (int i = 0, length = sizeof(dataset1); i < length; ++i) {
        uint8_t count = char_bit_count(dataset1[i], 8);
        U_ASSERT(res, count == 1);
    }

    //check dataset2 2 bit
    for (int i = 0, length = sizeof(dataset2); i < length; ++i) {
        uint8_t count = char_bit_count(dataset2[i], 8);
        U_ASSERT(res, count == 2);
    }

    //check dataset3 4 bit
    for (int i = 0, length = sizeof(dataset3); i < length; ++i) {
        uint8_t count = char_bit_count(dataset3[i], 8);
        U_ASSERT(res, count == 4);
    }

    //check 0b10101010
    for (uint8_t i = 0; i <= 8; ++i) {
        uint8_t count = char_bit_count(0b10101010, i);
        U_ASSERT(res, count == i / 2);
    }

    //check 0b100
    U_ASSERT(res, char_bit_count(0b100, 2) == 0);

    return res;
}

static int test_bit_count() {
    int res = 0;
    uint8_t mask[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

    //brute force
    for (int j = 1; j < 97; ++j) { //last bit
        for (int i = 0; i < 96; ++i) { //offset
            if (i > j) {
                continue;
            }
            if (bit_count(mask, i, j) != j - i) {
                printf("%d %d cnt: %d\n", i, j, bit_count(mask, i, j));
            }
            U_ASSERT(res, bit_count(mask, i, j) == j - i);
        }
    }


    return res;
}

static int test_char_bit_get() {
    int res = 0;
    U_ASSERT(res, char_bit_get(0b0, 4) == 0);
    U_ASSERT(res, char_bit_get(0b1, 4) == 0);
    U_ASSERT(res, char_bit_get(0b1, 1) == 1);
    U_ASSERT(res, char_bit_get(0b10, 2) == 1);
    U_ASSERT(res, char_bit_get(0b101, 3) == 1);
    U_ASSERT(res, char_bit_get(0b101, 2) == 0);
    return res;
}

static int test_char_bit_set() {
    int res = 0;
    U_ASSERT(res, char_bit_set(0b1, 2) == 0b11);
    U_ASSERT(res, char_bit_set(0b0, 4) == 0b1000);
    U_ASSERT(res, char_bit_set(0b1, 4) == 0b1001);
    U_ASSERT(res, char_bit_set(0b01111111, 8) == 0b11111111);
    return res;
}

static int test_bit_set() {
    int res = 0;
    uint8_t bitmask[8] = {}; //8 * 8 = 64bit mask
    bit_set(bitmask, 1);
    U_ASSERT(res, bitmask[0] == 0b1);
    bit_set(bitmask, 2);
    U_ASSERT(res, bitmask[0] == 0b11);
    bit_set(bitmask, 64);
    U_ASSERT(res, bitmask[7] == 0b10000000);
    bit_set(bitmask, 63);
    U_ASSERT(res, bitmask[7] == 0b11000000);
    bit_set(bitmask, 57);
    U_ASSERT(res, bitmask[7] == 0b11000001);
    return res;
}

static int test_char_bit_clear() {
    unsigned int res = 0;
    U_ASSERT(res, char_bit_clear(0b10, 2) == 0b0);
    U_ASSERT(res, char_bit_clear(0b1111, 1) == 0b1110);
    return res;
}

static int test_int32_bit_get() {
    int res = 0;
    U_ASSERT(res, int32_bit_get(0b11111111000000001111111100000000, 32) == 1);
    U_ASSERT(res, int32_bit_get(0b11111111000000001111111100000000, 1) == 0);
    U_ASSERT(res, int32_bit_get(0b11111111000000001111111100000000, 9) == 1);
    U_ASSERT(res, int32_bit_get(0b11111111000000001111111100000000, 17) == 0);
    return res;
}

static int test_int32_bit_clear() {
    int res = 0;
    U_ASSERT(res, int32_bit_clear(0b11111111000000001111111100000001, 1) == 0b11111111000000001111111100000000);
    U_ASSERT(res, int32_bit_clear(0b11111111000000001111111100000000, 32) == 0b01111111000000001111111100000000);
    return res;
}

static int test_int32_bit_set() {
    int res = 0;
    U_ASSERT(res, int32_bit_set(0b0, 1) == 0b1);
    U_ASSERT(res, int32_bit_set(0b0, 32) == 0b10000000000000000000000000000000);
    U_ASSERT(res, int32_bit_set(0b10111111000000001111111100000000, 31) == 0b11111111000000001111111100000000);
    return res;
}

static int test_transcode_string() {
    int res = 0;
    uint8_t src[] = "0123456789абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    uint8_t dst[256] = {};
    encode_string(dst, src);
    for (int i = 0; i < 33 + 10; ++i) { //10 digits + 33 letters
        U_ASSERT(res, dst[i] == i + 1); //+1 because index 0 is used by leaf flag
    }

    return res;
}

static int test_node_insert_ref() {
    int res = 0;
    //create trie
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node_s
    trie->nodes_increment++;

    //now we save leaf flag to the root node to check if refs block memory allocation is working good
    trie->nodes[0].mask[0] = 0b1;

    //node 1 save char index 7 node_s id 0
    uint32_t node_id_bit8 = node_insert_ref(7, 0, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 1); //expected 1 because we have just used ref_id 0
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b10000001); //bit 8 and bit 1 raised expected
    //check refs block directly
    U_ASSERT(res, trie->refs[0] == node_id_bit8);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit8);

    //node 2 save char index 6 node_s id 0
    uint32_t node_id_bit7 = node_insert_ref(6, 0, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 3); //expected 3 because allocated block size is 2 (from 1 to 2)
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b11000001); //bit 8, 7, 1 raised expected
    //check refs block directly
    U_ASSERT(res, trie->refs[1] == node_id_bit7);
    U_ASSERT(res, trie->refs[2] == node_id_bit8);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit8);

    //node 3 save char index 5 node_s id 0
    uint32_t node_id_bit6 = node_insert_ref(5, 0, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 6); //expected 6 because allocated block size is 3 (from 3 to 5)
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b11100001); //bit 8, 7, 6, 1 raised expected
    //check refs block directly
    U_ASSERT(res, trie->refs[3] == node_id_bit6);
    U_ASSERT(res, trie->refs[4] == node_id_bit7);
    U_ASSERT(res, trie->refs[5] == node_id_bit8);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 2] == node_id_bit8);


    //node 4 save char index 1 node_s id 0
    uint32_t node_id_bit2 = node_insert_ref(1, 0, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 10); //expected 10 because allocated block size is 4 (from 6 to 9)
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b11100011); //bit 8, 7, 6, 2, 1 raised expected
    //check refs block directly
    U_ASSERT(res, trie->refs[6] == node_id_bit2);
    U_ASSERT(res, trie->refs[7] == node_id_bit6);
    U_ASSERT(res, trie->refs[8] == node_id_bit7);
    U_ASSERT(res, trie->refs[9] == node_id_bit8);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 2] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 3] == node_id_bit8);

    //node 5 save char index 2 node_s id 0 just after char index 1 (pos 1)
    uint32_t node_id_bit3 = node_insert_ref(2, 1, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 15); //expected 15 because allocated block size is 5 (from 10 to 14)
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b11100111); //bit 8, 7, 6, 3, 2, 1 raised expected
    //check refs block directly
    U_ASSERT(res, trie->refs[10] == node_id_bit2);
    U_ASSERT(res, trie->refs[11] == node_id_bit3);
    U_ASSERT(res, trie->refs[12] == node_id_bit6);
    U_ASSERT(res, trie->refs[13] == node_id_bit7);
    U_ASSERT(res, trie->refs[14] == node_id_bit8);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 2] == node_id_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 3] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 4] == node_id_bit8);

    //node 6 char index 94 node_s id 0
    uint32_t node_id_bit95 = node_insert_ref(94, 5, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 21); //expected 21 because allocated block size is 6 (from 15 to 20)
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b11100111); //bit 8, 7, 6, 3, 2, 1 raised expected
    U_ASSERT(res, trie->nodes[0].mask[11] == 0b1000000); //bit 7 raised expected (index 11 * 8 + 7 = 95)
    //check refs block directly
    U_ASSERT(res, trie->refs[15] == node_id_bit2);
    U_ASSERT(res, trie->refs[16] == node_id_bit3);
    U_ASSERT(res, trie->refs[17] == node_id_bit6);
    U_ASSERT(res, trie->refs[18] == node_id_bit7);
    U_ASSERT(res, trie->refs[19] == node_id_bit8);
    U_ASSERT(res, trie->refs[20] == node_id_bit95);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 2] == node_id_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 3] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 4] == node_id_bit8);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 5] == node_id_bit95);

    //node 7 char index 32 node_s id 0
    uint32_t node_id_bit33 = node_insert_ref(32, 5, &trie->nodes[0], trie);
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[0].mask[0] == 0b11100111); //bit 8, 7, 6, 3, 2, 1 raised expected
    U_ASSERT(res, trie->nodes[0].mask[11] == 0b1000000); //bit 7 raised expected (index 11 * 8 + 7 = 95)
    U_ASSERT(res, trie->nodes[0].mask[4] == 0b1); //bit 1 raised expected (index 4 * 8 + 1 = 3)
    //check refs block directly
    U_ASSERT(res, trie->refs[21] == node_id_bit2);
    U_ASSERT(res, trie->refs[22] == node_id_bit3);
    U_ASSERT(res, trie->refs[23] == node_id_bit6);
    U_ASSERT(res, trie->refs[24] == node_id_bit7);
    U_ASSERT(res, trie->refs[25] == node_id_bit8);
    U_ASSERT(res, trie->refs[26] == node_id_bit33);
    U_ASSERT(res, trie->refs[27] == node_id_bit95);
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 2] == node_id_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 3] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 4] == node_id_bit8);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 5] == node_id_bit33);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 6] == node_id_bit95);



    //deallocated memory block reallocation test
    //node 8 char index 1 in the node 1
    U_ASSERT(res, trie->deal[1]->elements == 1); //expected 1
    uint32_t node8_bit2 = node_insert_ref(1, 0, &trie->nodes[1], trie);
    U_ASSERT(res, trie->deal[1]->elements == 0); //expected 0
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[1].mask[0] == 0b10); //bit 2 raised expected
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id] == node8_bit2);


    //node 9 char index 2 pos 1 in the node 1
    U_ASSERT(res, trie->deal[2]->elements == 1); //expected 1
    uint32_t node9_bit3 = node_insert_ref(2, 1, &trie->nodes[1], trie);
    U_ASSERT(res, trie->deal[1]->elements == 1); //expected 1
    U_ASSERT(res, trie->deal[2]->elements == 0); //expected 0
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[1].mask[0] == 0b110); //bit 2,3 raised expected
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id] == node8_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 1] == node9_bit3);


    //node 10 char index 3 pos 2 in the node 1
    U_ASSERT(res, trie->deal[3]->elements == 1); //expected 1
    uint32_t node10_bit4 = node_insert_ref(3, 2, &trie->nodes[1], trie);
    U_ASSERT(res, trie->deal[2]->elements == 1); //expected 1
    U_ASSERT(res, trie->deal[3]->elements == 0); //expected 0
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[1].mask[0] == 0b1110); //bit 2,3,4 raised expected
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id] == node8_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 1] == node9_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 2] == node10_bit4);


    //node 11 char index 4 pos 3 in the node 1
    U_ASSERT(res, trie->deal[4]->elements == 1); //expected 1
    uint32_t node11_bit5 = node_insert_ref(4, 3, &trie->nodes[1], trie);
    U_ASSERT(res, trie->deal[3]->elements == 1); //expected 1
    U_ASSERT(res, trie->deal[4]->elements == 0); //expected 0
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[1].mask[0] == 0b11110); //bit 2,3,4,5 raised expected
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id] == node8_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 1] == node9_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 2] == node10_bit4);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 3] == node11_bit5);


    //node 12 char index 5 pos 4 in the node 1
    U_ASSERT(res, trie->deal[5]->elements == 1); //expected 1
    uint32_t node12_bit6 = node_insert_ref(5, 4, &trie->nodes[1], trie);
    U_ASSERT(res, trie->deal[4]->elements == 1); //expected 1
    U_ASSERT(res, trie->deal[5]->elements == 0); //expected 0
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[1].mask[0] == 0b111110); //bit 2,3,4,5,6 raised expected
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id] == node8_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 1] == node9_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 2] == node10_bit4);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 3] == node11_bit5);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 4] == node12_bit6);


    //node 13 char index 6 pos 5 in the node 1
    U_ASSERT(res, trie->deal[6]->elements == 1); //expected 1
    uint32_t node13_bit7 = node_insert_ref(6, 5, &trie->nodes[1], trie);
    U_ASSERT(res, trie->deal[5]->elements == 1); //expected 1
    U_ASSERT(res, trie->deal[6]->elements == 0); //expected 0
    //check refs block memory allocation
    U_ASSERT(res, trie->refs_increment == 28); //expected 28 because allocated block size is 7 (from 21 to 27)
    //check mask
    U_ASSERT(res, trie->nodes[1].mask[0] == 0b1111110); //bit 2,3,4,5,6,7 raised expected
    //check refs block from the node_s ref_id
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id] == node8_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 1] == node9_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 2] == node10_bit4);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 3] == node11_bit5);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 4] == node12_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[1].ref_id + 5] == node13_bit7);


    //node 0
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id] == node_id_bit2);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 1] == node_id_bit3);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 2] == node_id_bit6);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 3] == node_id_bit7);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 4] == node_id_bit8);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 5] == node_id_bit33);
    U_ASSERT(res, trie->refs[trie->nodes[0].ref_id + 6] == node_id_bit95);


    yatrie_free(trie);
    return res;
}

static int test_trie_char_add() {
    int res = 0;
    //create trie
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));

    //create empty root node_s
    trie->nodes_increment++;

    uint32_t node_id_inserted;

    //add char index 95 to the node_s 0, returns 1
    node_id_inserted = trie_char_add(0, 95, trie);
    U_ASSERT(res, node_id_inserted == 1);

    //add char index 95 to the node_s 0 again. Must return same node_id 1
    node_id_inserted = trie_char_add(0, 95, trie);
    U_ASSERT(res, node_id_inserted == 1);

    //add second char index 2 to the node_s 1, returns 2
    node_id_inserted = trie_char_add(1, 2, trie);
    U_ASSERT(res, node_id_inserted == 2);

    //add third char index 50 to the node_s 2, returns 3
    node_id_inserted = trie_char_add(2, 50, trie);
    U_ASSERT(res, node_id_inserted == 3);

    free(trie);
    return res;
}

static int test_bit_get() {
    int res = 0;
    uint8_t b = 0b10101010;
    uint8_t bitmask[12] = {b, b, b, b, b, b, b, b, b, b, b, b};
    uint8_t bitmask2[12] = {0};
    bit_set(bitmask2, 27);


    U_ASSERT(res, bit_get(bitmask, 8) == 1);
    U_ASSERT(res, bit_get(bitmask, 1) == 0);
    U_ASSERT(res, bit_get(bitmask, 2) == 1);
    U_ASSERT(res, bit_get(bitmask, 3) == 0);
    U_ASSERT(res, bit_get(bitmask, 48) == 1);
    U_ASSERT(res, bit_get(bitmask, 10) == 1);
    U_ASSERT(res, bit_get(bitmask, 11) == 0);
    U_ASSERT(res, bit_get(bitmask, 16) == 1);
    U_ASSERT(res, bit_get(bitmask, 96) == 1);


    return res;
}

static int test_trie_add() {
    int res = 0;
    //create trie
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node_s
    trie->nodes_increment++;

    uint32_t inserted[2][96] = {};


    uint32_t nodes[5000] = {};
    int c = 0;
    uint8_t string[512] = {};
    FILE *file;
    file = fopen("./dic/english.txt", "r");
    if (file) {
        for (int j = 0, i = 0; (c = getc(file)) != EOF; ++j) {
            if (c == '\n' || c == '\r') {
                nodes[i] = trie_add(string, 0, trie);
                memset(string, 0, 512);
                j = -1;
                ++i;
                continue;
            }

            string[j] = c;
        }

        rewind(file);

        for (int j = 0, i = 0; (c = getc(file)) != EOF; ++j) {
            if (c == '\n' || c == '\r') {
                U_ASSERT(res, trie_get_id(string, 0, trie) == nodes[i]);
                memset(string, 0, 512);
                j = -1;
                ++i;
                continue;
            }

            string[j] = c;
        }

        fclose(file);
    }


    yatrie_free(trie);
    return res;
}

static int test_char_mask_get_bits_raised_pre() {

    uint32_t *indicator = &tests_passed;

    int res = 0;
    uint8_t bits[9]; //index 0 is a number of raised bits
    uint8_t expected1[9] = {8, 1, 2, 3, 4, 5, 6, 7, 8}; //(0b11111111) full byte
    uint8_t expected2[9] = {1, 1};                      //(0b00000001) only first byte
    uint8_t expected3[9] = {1, 8};                      //(0b10000000) only last byte
    uint8_t expected4[9] = {4, 82, 84, 86, 88};         //(0b10101010)
    uint8_t expected5[9] = {2, 1, 8};                   //(0b10000001) first and last

    //dataset 1
    memset(bits, 0, 9);//clear array
    memcpy(bits, char_mask_get_bits_raised_pre(0b11111111, 0), 9);
    U_ASSERT(res, memcmp(expected1, bits, 9) == 0);

    //dataset 2
    memset(bits, 0, 9);//clear array
    memcpy(bits, char_mask_get_bits_raised_pre(0b1, 0), 9);
    U_ASSERT(res, memcmp(expected2, bits, 9) == 0);

    //dataset 3
    memset(bits, 0, 9);//clear array
    memcpy(bits, char_mask_get_bits_raised_pre(0b10000000, 0), 9);
    U_ASSERT(res, memcmp(expected3, bits, 9) == 0);

    //dataset 4
    memset(bits, 0, 9);//clear array
    memcpy(bits, char_mask_get_bits_raised_pre(0b10101010, 10), 9);
    U_ASSERT(res, memcmp(expected4, bits, 9) == 0);

    //dataset 5
    memset(bits, 0, 9);//clear array
    memcpy(bits, char_mask_get_bits_raised_pre(0b10000001, 0), 9);
    U_ASSERT(res, memcmp(expected5, bits, 9) == 0);

    return res;
}

static int test_char_mask_get_bits_raised() {
    int res = 0;

    uint8_t bits[8];
    uint8_t expected1[8] = {1, 2, 3, 4, 5, 6, 7, 8}; //full byte (0b11111111)
    uint8_t expected2[8] = {1}; //only first byte (0b1)
    uint8_t expected3[8] = {8}; //only last byte (0b10000000)

    memset(bits, 0, 8);//clear array
    char_mask_get_bits_raised(bits, 0b11111111); //get raised bits
    U_ASSERT(res, memcmp(expected1, bits, 8) == 0); //compare bits with expected

    memset(bits, 0, 8);
    char_mask_get_bits_raised(bits, 0b1);
    U_ASSERT(res, memcmp(expected2, bits, 8) == 0);

    memset(bits, 0, 8);
    char_mask_get_bits_raised(bits, 0b10000000);
    U_ASSERT(res, memcmp(expected3, bits, 8) == 0);

    return res;
}

static int test_mask_get_bits_raised_pre() {
    int res = 0;

    uint8_t mask[MASK_INDEX];
    uint8_t bits[MASK_BITS];
    uint8_t expected1[MASK_BITS] = {1, 2, 3};
    uint8_t expected2[MASK_BITS] = {10, 20, 30};
    uint8_t expected3[MASK_BITS] = {11, 22, 33};
    uint8_t expected4[MASK_BITS] = {1, 8, 9, 16, 17, 24, 25,
                                    32, 33, 40, 41, 48, 49,
                                    56, 57, 64, 65, 72, 73,
                                    80, 81, 88, 89, 96};

    //dataset 1
    memset(mask, 0, MASK_INDEX); //clear array
    memset(bits, 0, MASK_BITS); //clear array
    mask[0] = 0b111;
    mask_get_bits_raised_pre(bits, mask); //calculation
    U_ASSERT(res, memcmp(expected1, bits, MASK_BITS) == 0);

    //dataset 2
    memset(mask, 0, MASK_INDEX); //clear array
    memset(bits, 0, MASK_BITS); //clear array
    mask[1] = 0b10; //bit 10
    mask[2] = 0b1000; //bit 20
    mask[3] = 0b100000; //bit 30
    mask_get_bits_raised_pre(bits, mask); //calculation
    U_ASSERT(res, memcmp(expected2, bits, MASK_BITS) == 0);

    //dataset 3
    memset(mask, 0, MASK_INDEX); //clear array
    memset(bits, 0, MASK_BITS); //clear array
    mask[1] = 0b100; //bit 11
    mask[2] = 0b100000; //bit 22
    mask[4] = 0b1; //bit 33
    mask_get_bits_raised_pre(bits, mask); //calculation
    U_ASSERT(res, memcmp(expected3, bits, MASK_BITS) == 0);

    //dataset 4
    memset(mask, 0, MASK_INDEX); //clear array
    memset(bits, 0, MASK_BITS); //clear array
    mask[0] = 0b10000001; //bit 1,8
    mask[1] = 0b10000001; //bit 9,16
    mask[2] = 0b10000001; //bit 17,24
    mask[3] = 0b10000001; //bit 25,32
    mask[4] = 0b10000001; //bit 33,40
    mask[5] = 0b10000001; //bit 41,48
    mask[6] = 0b10000001; //bit 49,56
    mask[7] = 0b10000001; //bit 57,64
    mask[8] = 0b10000001; //bit 63,72
    mask[9] = 0b10000001; //bit 73,80
    mask[10] = 0b10000001; //bit 81,88
    mask[11] = 0b10000001; //bit 89,96
    mask_get_bits_raised_pre(bits, mask); //calculation
    U_ASSERT(res, memcmp(expected4, bits, MASK_BITS) == 0);


    return res;
}

static int test_node_get_children() {
    int res = 0;

    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node_s
    trie->nodes_increment++;

    //add some data to the root node
    trie->nodes[0].mask[0] = 0b10111; //raise bits 2,3,5
    trie->nodes[0].ref_id = 0; //save ref_id
    uint8_t expected11[MASK_BITS] = {1, 2, 3, 5}; //raise bits 2,3,4
    uint32_t expected12[MASK_BITS] = {1111, 2222, 3333}; //raise bits 2,3,4
    trie->refs[0] = 1111; //ref_id 0
    trie->refs[1] = 2222; //ref_id 1
    trie->refs[2] = 3333; //ref_id 2

    //create children container
    children_s *children = (children_s *) calloc(1, sizeof(children_s));

    //get children
    node_get_children(children, 0, trie);

    //check saved values
    U_ASSERT(res, children->length == 3); //check counter
    U_ASSERT(res, memcmp(children->letters, expected11, 3) == 0); //check letters
    U_ASSERT(res, memcmp(children->nodes, expected12, 3 << REF_SIZE_POWER) == 0); //check nodes

    yatrie_free(trie);
    free(children);
    return res;
}

static int test_node_traverse() {
    int res = 0;

    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node_s
    trie->nodes_increment++;

    uint8_t word[] = "ёж";
    uint32_t node_id;

    trie_add("ёжистый", 0, trie); //7 letter
    trie_add("ёж", 0, trie); //2 letters
    trie_add("ёжисты", 0, trie); //6 letters
    trie_add("ёжистая", 0, trie); //7 letters
    trie_add("ёжистее", 0, trie); //7 letters


    //create words letters number array
    uint32_t expected_letters = 2 + 6 + 7 + 7 + 7;

    //create words container
    words_s words = {};

    //head container
    string_s head = {};

    node_traverse(&words, 0, &head, trie);

    //words letters counter
    int length = 0;
    for (int i = 0; i < words.counter; ++i) {
        int word_length = 0;
        while (words.words[i][word_length]) {
            length++;
            word_length++;
        }
    }
    U_ASSERT(res, length == expected_letters);


    yatrie_free(trie);
    return res;
}

static int test_node_traverse_decode() {
    int res = 0;

    printf("TRIE NODE TRAVERSE TEST\n"
           "This test will add a few words to the dictionary.\n"
           "Then it will try to traverse all nodes starting\n"
           "from the given one, gathering all the nodes with\n"
           "the \"leaf\" flag into chains.\n\n");

    //create trie
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node
    trie->nodes_increment++;


    uint8_t words[][20] = {
            "ёж",
            "ёжистая",
            "ёжистее",
            "ёжисты",
            "ёжистый",
            "few english words",
    };

    for (int i = 0, len = sizeof(words) / 20; i < len; ++i) {
        uint32_t id = trie_add(words[i], 0, trie);
        printf("added word: %s with node id %d\n", words[i], id);
    }

    //create returned words container
    words_s words_returned = {};

    //head container
    string_s head = {};

    node_traverse(&words_returned, 0, &head, trie);

    printf("\nTraverse root node completed\n"
           "Check passed and returned words\n");

    //words letters counter
    int length = 0;
    for (int i = 0; i < words_returned.counter; ++i) {
        int word_length = 0;
        uint8_t dst[256] = {};
        decode_string(dst, words_returned.words[i]);

        //check saved to trie and returned word from
        printf("returned and decoded: %s expected: %s\n", dst, words[i]);
        U_ASSERT(res, memcmp(dst, words[i], sizeof(words[i])) == 0);
    }

    printf("\n\n");

    yatrie_free(trie);
    return res;
}

static int test_trie_get_id() {
    int res = 0;
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    trie->nodes_increment++;

    uint32_t nodes[256] = {};
    uint8_t words[][64] = {
            "11-й",
            "11-го",
            "11-му",
            "14-ый",
            "14-ого",
            "14-ому",
            "40-м",
            "40-я",
            "40-ю",
            "40-е",
            "40-ой",
            "40-ого",
            "40-ому",
            "40-ым",
            "40-ом",
            "40-ая",
            "40-ую",
            "40-ое",
    };


    for (int i = 0, size = sizeof(words) / 64; i < size; ++i) {
        nodes[i] = trie_add(words[i], 0, trie);
    }


    for (int i = 0, size = sizeof(words) / 64; i < size; ++i) {
        U_ASSERT(res, nodes[i] = trie_get_id(words[i], 0, trie));
    }


    yatrie_free(trie);
    return res;
}

static int complex_test() {
    int res = 0;
    time_t begin = 0;
    time_t end = 0;

    uint32_t node_id = 0;
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    trie->nodes_increment++;

    printf("COMPLEX TEST\n"
           "This test will create trie from a dictionary file, \n"
           "then compare each word leaf node id with returned \n"
           "node id by the trie_get_id() function \n\n");
    printf("Trie settings:\n"
           "node size:       %u bytes\n", sizeof(node_s));
    printf("trie size:       %u kb\n", sizeof(trie_s) / 1024);
    printf("trie size nodes: %u kb\n", sizeof(trie->nodes) / 1024);
    printf("trie size refs:  %u kb\n\n", sizeof(trie->refs) / 1024);

    uint8_t string[512] = {};
#define MAX_NODES 6000000
    uint32_t *added_nodes = (uint32_t *) calloc(MAX_NODES, 4);
    uint32_t *getted_nodes = (uint32_t *) calloc(MAX_NODES, 4);


    int c = 0;
    FILE *file;
    file = fopen(dic_path, "r");
    if (file) {
        begin = clock();
        for (int j = 0, i = 0; (c = getc(file)) != EOF; ++j) {
            if (c == '\n' || c == '\r') {
                ++i;

                added_nodes[i] = trie_add(string, 0, trie);

                memset(string, 0, 512);
                j = -1;
                (i & 262143) == 0 ? printf("a:%d\n", i) : 0;
                continue;
            }
            string[j] = c;
        }
        end = clock();

        rewind(file);

        for (int j = 0, i = 0; (c = getc(file)) != EOF; ++j) {
            if (c == '\n' || c == '\r') {
                ++i;
                getted_nodes[i] = trie_get_id(string, 0, trie);

                memset(string, 0, 512);
                j = -1;
                (i & 262143) == 0 ? printf("g:%d\n", i) : 0;
                continue;
            }
            string[j] = c;
        }

        fclose(file);
    }


    //check added nodes array and getted nodes array
    U_ASSERT(res, memcmp(added_nodes, getted_nodes, MAX_NODES * 4) == 0);

    printf("trie creation time: %fs\n", (end - begin) / (float) 1000000);
    printf("nodes created: %d\nreferences created: %d\n", trie->nodes_increment, trie->refs_increment);


    free(added_nodes);
    free(getted_nodes);

    yatrie_free(trie);
    return res;
}

static int test_decode_string() {
    int res = 0;
    uint8_t src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t dst[256] = {};
    decode_string(dst, src);
    U_ASSERT(res, memcmp(dst, "0123456789", sizeof("0123456789")) == 0);
    return res;
}

static int test_trie_save_load() {
    time_t begin1 = 0;
    time_t end1 = 0;


    int res = 0;
    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node_s
    trie->nodes_increment++;

    //fill trie
    int c = 0;
    uint8_t string[512] = {};

    FILE *file;
    file = fopen("../dic/english.txt", "r");
    if (file) {
        begin1 = clock();
        for (int j = 0, i = 0; (c = getc(file)) != EOF; ++j) {
            if (c == '\n' || c == '\r') {
                trie_add(string, 0, trie);
                memset(string, 0, 512);
                j = -1;
                ++i;
                continue;
            }

            string[j] = c;
        }
        end1 = clock();

        fclose(file);
    }

    //save trie to a file
    yatrie_save("/tmp/test.trie", trie);


    trie_s *trie2 = (trie_s *) calloc(1, sizeof(trie_s));
    yatrie_load("/tmp/test.trie", trie2);

    //compare
    U_ASSERT(res, trie->nodes_increment == trie2->nodes_increment); //nodes counter
    U_ASSERT(res, trie->refs_increment == trie2->refs_increment); //refs counter
    U_ASSERT(res, memcmp(trie->nodes, trie2->nodes, sizeof(trie->nodes)) == 0); //nodes block
    U_ASSERT(res, memcmp(trie->refs, trie2->refs, sizeof(trie->refs)) == 0); //refs block


    //printf("yatrie creation time: %fs\n", (end1 - begin1) / (float) 1000000);

    yatrie_free(trie);
    yatrie_free(trie2);
    return res;
}

static int another_node_traverse() {
    time_t begin1 = 0;
    time_t end1 = 0;


    int res = 0;
    printf("\n\nANOTHER NODE TRAVERSE\n");


    trie_s *trie = (trie_s *) calloc(1, sizeof(trie_s));
    //create empty root node_s
    trie->nodes_increment++;

    //fill trie
    int c = 0;
    uint8_t string[512] = {};

    FILE *file;
    file = fopen(dic_path, "r");
    if (file) {
        begin1 = clock();
        for (int j = 0, i = 0; (c = getc(file)) != EOF; ++j) {
            if (c == '\n' || c == '\r') {
                trie_add(string, 0, trie);
                memset(string, 0, 512);
                j = -1;
                ++i;
                continue;
            }

            string[j] = c;
        }
        end1 = clock();

        fclose(file);
    }

    printf("Trie creation time: %fs\n", (end1 - begin1) / (float) 1000000);
    printf("TRAVERSE ALL NODES STARTING FROM 'af' NODE\n\n");
    //get 'af' node id
    uint8_t word[] = "af";
    uint32_t id = trie_get_id(word, 0, trie);

    //create returned words container
    words_s words_returned = {};

    //head container
    uint8_t encoded[100] = {};

    //encode word chars to indexes
    encode_string(encoded, word);

    //init head structure
    string_s head = {.length = 2};
    //copy word chars indexes to the head structure
    memcpy(head.letters, encoded, 2);

    //traverse node with given id
    node_traverse(&words_returned, id, &head, trie);

    //words letters counter
    for (int i = 0; i < words_returned.counter; ++i) {
        uint8_t dst[256] = {};
        decode_string(dst, words_returned.words[i]);
        printf("returned: %s\n", dst);
    }

    printf("\n\n");


    yatrie_free(trie);
    return res;
}

static int all_tests() {
    U_RUN(test_foo);
    U_RUN(test_node_traverse_decode);
    U_RUN(test_trie_save_load);
    U_RUN(test_decode_string);
    U_RUN(test_node_insert_ref);
    U_RUN(test_trie_get_id);
    U_RUN(test_node_traverse);
    U_RUN(test_bit_get);
    U_RUN(test_bit_count);
    U_RUN(test_char_bit_count);
    U_RUN(test_trie_add);
    U_RUN(test_node_get_children);
    U_RUN(test_mask_get_bits_raised_pre);
    U_RUN(test_char_mask_get_bits_raised_pre);
    U_RUN(test_char_mask_get_bits_raised);
    U_RUN(test_trie_char_add);
    U_RUN(test_char_bit_get);
    U_RUN(test_char_bit_set);
    U_RUN(test_bit_set);
    U_RUN(test_char_bit_clear);
    U_RUN(test_int32_bit_get);
    U_RUN(test_int32_bit_set);
    U_RUN(test_int32_bit_clear);
    U_RUN(test_transcode_string);
    U_RUN(complex_test);
    U_RUN(another_node_traverse);

    return tests_failed;
}


int main(int argc, char **argv) {
    printf("*** LIBTRIE UNIT TESTS AND DEMO ***\n\n\n");
    //set dic_path if given
    if(argc > 1) {
        int len = 0;
        while (argv[1][len]){
            ++len;
        };
        memcpy(&dic_path, argv[1], len);
    } else{
        memcpy(&dic_path, "../dic/english.txt", 18);
    }

    if ((fopen(dic_path, "r")) == NULL) {
        printf("Unable to read dic: %s\n", dic_path);
        return 1;
    }

    all_tests() ? printf("Results. Tests failed/passed: %d/%d\n", tests_failed, tests_passed)
                : printf("Very good results! Tests passed: %d/%d\n", tests_passed, tests_passed);

    return tests_failed ? 1 : 0;
}