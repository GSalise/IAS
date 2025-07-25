#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Nb 4
#define Nk 4
#define Nr 10

/* 
		Why not int? Apparently because int is not accurate enough
		
		uint8_t 
		- always guarantees a size of 8bits (1 byte)
		- safer in general
			- behavior is predictable (no overflow surprises)
			- bitwise operations are controlled
			- neccessary for precise crypto operations
			
		int 
		- has 32 bits but is not guaranteed
		- unpredictable
*/

typedef uint8_t byte;

/*
		What is sbox?
		- The substitution box is a FIXED 256-byte look up table used in the SubBytes() step of AES encryption
		- Maps each byte in the AES state to another byte
			- For example: 
				if a byte is 0xA1
				then it will be substituted by the x and y axis of the sbox based on the byte (A and 1)
				so 0xA1 becimes 0xA2
*/

byte sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16   // F
	};

/*
		What is rsbox?
		- The inverse of sbox
		- used in InvSubBytes() step
*/
    
byte rsbox[256] = {
	// 0    1      2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,  //0
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,  //1
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,  //2
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,  //3
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,  //4
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,  //5
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,  //6
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,  //7
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,  //8
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,  //9
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,  //A
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,  //B
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,  //C
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,  //D
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,  //E
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D   //F
	};

/*
		What is Rcon?
		- A set of PREDEFINED constants used in the Key Expansion algorithm
		- Helps introduce variation and non-linearity between each AES round key.
*/

byte Rcon[11] = {
  0x00, 0x01, 0x02, 0x04, 0x08,
  0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};


byte state[4][4];
byte RoundKey[176];
byte Key[16]; // 128-bit key

/*
		State 
		- stores input plaintext or ciphertext
		- AES operates on a 4x4 matrix bytes
		
		Sample:
		Plaintext/Ciphertext = Input (hex): 32 88 31 e0 43 5a 31 37 f6 30 98 07 a8 8d a2 34
		
		The plaintext will be stored as this:
		state[4][4] =
		  32 43 f6 a8
		  88 5a 30 8d
		  31 31 98 a2
		  e0 37 07 34
		  
		RoundKey
		- stores the expanded key schedule
		- since AES128 uses 11 round keys, each 16 bytes long then it will have the size of 176 (11*16=176)

		Key
		- stores the user defined key
*/


/*
=======================================================================

						Encryption Implementation

=======================================================================
*/

void SubBytes() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[j][i] = sbox[state[j][i]];
}

void ShiftRows() {
	byte temp, temp2;
	
	// 2nd row - 1 byte circular shift
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;
	
	// 3rd row - 2 bytes circular shift
	temp = state[2][0];
	temp2 = state[2][1];
	state[2][0] = state[2][2];
	state[2][1] = state[2][3];
	state[2][2] = temp;
	state[2][3] = temp2;
	
	// 4th row - 3 bytes circular shift
	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;
	
}

// Multiply for MixColumns
byte xtime(byte x) {
    return (x << 1) ^ ((x >> 7) * 0x1b);
}

void MixColumns() {
    byte Tmp, Tm, t;
    for (int i = 0; i < 4; i++) {
        t = state[0][i];
        Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        
        Tm = state[0][i] ^ state[1][i];
        Tm = xtime(Tm);
        state[0][i] ^= Tm ^ Tmp;

        Tm = state[1][i] ^ state[2][i];
        Tm = xtime(Tm);
        state[1][i] ^= Tm ^ Tmp;

        Tm = state[2][i] ^ state[3][i];
        Tm = xtime(Tm);
        state[2][i] ^= Tm ^ Tmp;

        Tm = state[3][i] ^ t;
        Tm = xtime(Tm);
        state[3][i] ^= Tm ^ Tmp;
    }
    
    /*
		Loop through the entire matrix by column
		Store the top element of the column
		XOR all of the four bytes in the column
		??? nawala nako
	*/
}


// AddRoundKey step
void AddRoundKey(int round) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[j][i] ^= RoundKey[round * Nb * 4 + i * Nb + j];
}

// AES Encrypt one block (16 bytes)
void AES_encrypt(byte* input, byte* output) {
    for (int i = 0; i < 16; i++)
        state[i % 4][i / 4] = input[i];

    AddRoundKey(0);

    for (int round = 1; round < Nr; round++) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }

    SubBytes();
    ShiftRows();
    AddRoundKey(Nr);

    for (int i = 0; i < 16; i++)
        output[i] = state[i % 4][i / 4];
}

// Key Expansion (simplified for AES-128)
void KeyExpansion() {
    int i, j;
    byte temp[4], k;

    for (i = 0; i < Nk; i++) {
        RoundKey[i * 4] = Key[i * 4];
        RoundKey[i * 4 + 1] = Key[i * 4 + 1];
        RoundKey[i * 4 + 2] = Key[i * 4 + 2];
        RoundKey[i * 4 + 3] = Key[i * 4 + 3];
    }

    while (i < Nb * (Nr + 1)) {
        for (j = 0; j < 4; j++)
            temp[j] = RoundKey[(i - 1) * 4 + j];

        if (i % Nk == 0) {
            byte ttemp = temp[0];
            temp[0] = sbox[temp[1]];
            temp[1] = sbox[temp[2]];
            temp[2] = sbox[temp[3]];
            temp[3] = sbox[ttemp];
            temp[0] ^= Rcon[i / Nk];
        }

        for (j = 0; j < 4; j++)
            RoundKey[i * 4 + j] = RoundKey[(i - Nk) * 4 + j] ^ temp[j];
        i++;
    }
}

// Padding and ECB loop
void encrypt_ecb(byte* input, int length, byte* key) {
    memcpy(Key, key, 16);
    KeyExpansion();

    byte block[16];
    byte out[16];
    int padded = length % 16 ? length + (16 - (length % 16)) : length;
    byte* data = calloc(padded, 1);
    memcpy(data, input, length);

    printf("Encrypted (hex):\n");
    for (int i = 0; i < padded; i += 16) {
        AES_encrypt(data + i, out);
        for (int j = 0; j < 16; j++)
            printf("%02x", out[j]);
    }
    printf("\n");
    free(data);
}

/*
=======================================================================

						Decryption Implementation

=======================================================================
*/

void InvSubBytes() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[j][i] = rsbox[state[j][i]];
}

void InvShiftRows() {
    byte temp;

    // 2nd row
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    // 3rd row
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // 4th row
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

byte Multiply(byte x, byte y) {
    byte r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x = xtime(x);
        y >>= 1;
    }
    return r;
}

void InvMixColumns() {
    byte a, b, c, d;
    for (int i = 0; i < 4; i++) {
        a = state[0][i];
        b = state[1][i];
        c = state[2][i];
        d = state[3][i];

        state[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        state[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        state[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        state[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

void AES_decrypt(byte* input, byte* output) {
    for (int i = 0; i < 16; i++)
        state[i % 4][i / 4] = input[i];

    AddRoundKey(Nr);

    for (int round = Nr - 1; round >= 1; round--) {
        InvShiftRows();
        InvSubBytes();
        AddRoundKey(round);
        InvMixColumns();
    }

    InvShiftRows();
    InvSubBytes();
    AddRoundKey(0);

    for (int i = 0; i < 16; i++)
        output[i] = state[i % 4][i / 4];
}

/*
=======================================================================

						Main and Hash

=======================================================================
*/

int computeHash(char* plaintext) {
    int hash = 0;
    for (int i = 0; plaintext[i] != '\0'; ++i) {
        hash += plaintext[i];
    }
	return hash;
}


int main() {
    byte key[16] = "mysecretkey12345";
    char plaintext[] = "This is a test AES ECB.";

    int length = strlen(plaintext);
    int padded = length % 16 ? length + (16 - (length % 16)) : length;
    byte* data = calloc(padded, 1);
    memcpy(data, plaintext, length);

    byte encrypted[128];
    byte decrypted[128];

    memcpy(Key, key, 16);
    KeyExpansion();

	printf("Plaintext: %s\nKey (in Hex): ", plaintext);
	for(int i = 0; i < 16; i++){
		printf("%02x", key[i]);
	}
	printf("\nHash Value: %d\n\n", computeHash(plaintext));
	
	
	
    printf("Encrypted (hex):\n");
    for (int i = 0; i < padded; i += 16) {
        AES_encrypt(data + i, encrypted + i);
        for (int j = 0; j < 16; j++)
            printf("%02x", encrypted[i + j]);
    }
    printf("\n");

    for (int i = 0; i < padded; i += 16)
        AES_decrypt(encrypted + i, decrypted + i);

    printf("Decrypted text:\n%s\n", decrypted);
    printf("Hash Value: %d\n\n", computeHash(decrypted));

    free(data);
    return 0;
}

