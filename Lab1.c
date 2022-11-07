#include <stdio.h>
#include <stdint.h>
#include <string.h>

const unsigned int Rcon[] = { 0x00000000, 
           0x01000000, 0x02000000, 0x04000000, 0x08000000, 
           0x10000000, 0x20000000, 0x40000000, 0x80000000, 
           0x1B000000, 0x36000000, 0x6C000000, 0xD8000000, 
           0xAB000000, 0x4D000000, 0x9A000000, 0x2F000000, 
           0x5E000000, 0xBC000000, 0x63000000, 0xC6000000, 
           0x97000000, 0x35000000, 0x6A000000, 0xD4000000, 
           0xB3000000, 0x7D000000, 0xFA000000, 0xEF000000, 
           0xC5000000, 0x91000000, 0x39000000, 0x72000000, 
           0xE4000000, 0xD3000000, 0xBD000000, 0x61000000, 
           0xC2000000, 0x9F000000, 0x25000000, 0x4A000000, 
           0x94000000, 0x33000000, 0x66000000, 0xCC000000, 
           0x83000000, 0x1D000000, 0x3A000000, 0x74000000, 
           0xE8000000, 0xCB000000, 0x8D000000};

const uint8_t Sbox[16][16] = {
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 } ,
    { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 } ,
    { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 } ,
    { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 } ,
    { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 } ,
    { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf } ,
    { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 } ,
    { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 } ,
    { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 } ,
    { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb } ,
    { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 } ,
    { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 } ,
    { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a } ,
    { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e } ,
    { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf } ,
    { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
    };

const uint8_t InvSbox[16][16] = {
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb } ,
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb } ,
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e } ,
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 } ,
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 } ,
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 } ,
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 } ,
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b } ,
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 } ,
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e } ,
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b } ,
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 } ,
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f } ,
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef } ,
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 } ,
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
    };

const uint8_t mixCol[4][4] = {
                        {0x02, 0x03, 0x01, 0x01},
                        {0x01, 0x02, 0x03, 0x01},
                        {0x01, 0x01, 0x02, 0x03},
                        {0x03, 0x01, 0x01, 0x02}
                        };

const uint8_t invMixCol[4][4] = {
                        {0x0e, 0x0b, 0x0d, 0x09},
                        {0x09, 0x0e, 0x0b, 0x0d},
                        {0x0d, 0x09, 0x0e, 0x0b},
                        {0x0b, 0x0d, 0x09, 0x0e}
                        };

uint8_t oneb = 0x1b;
int Nb = 4;
int Nk = 4;
int Nr;
int appC = 0;

// Prints the state. Used in Appendix C debugging
void printState(uint8_t state[4][4]){
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            printf("%02x", state[y][x]);
        }
    }
}

// Prints the Next Keys. Used in Appendix C debugging
void printKeys(unsigned int w[74], int val){
    // printf("Printing from printKeys\n");
    // printf("val = %d, (val * 4) + i = %d\n", val, val*4);

    for(int i = 0; i < 4; i++){
        printf("%08x", w[(val * 4) + i]);
    }
}

// Combines four uint8_t's to form one unsigned int "word"
unsigned int word(uint8_t one, uint8_t two, uint8_t three, uint8_t four){
    unsigned int res = 0;

    res = res | four;
    res = res | (three<<8 & 0xff00);
    res = res | (two<<16 & 0xff0000);
    res = res | (one<<24 & 0xff000000);
    // printf("printed from word function. constructed word is 0x%x\n", res);
    return res;
}

uint8_t xtime(uint8_t value){
    int isSet = value & (1<<7);
    uint8_t res = value << 1;
    if (isSet){
        res = res ^ oneb;
    }
    return res;
}

uint8_t ffAdd(uint8_t bit1, uint8_t bit2){
    return bit1 ^ bit2;
}

uint8_t ffMultiply(uint8_t *stateByte, uint8_t constant){
    // possible time save by giving an auto response to 0x02
    if(constant == 0x01){
        return *stateByte;
    }
    uint8_t xtimeRes[8];
    int counter = 0;
    xtimeRes[counter] = *stateByte;
    counter++;
    do{
        xtimeRes[counter] = xtime(xtimeRes[counter - 1]);
        counter++;
    } while(counter < 8);   // Why is this 7??
    //printf("\n\n values from ffMultiply inside of xtimeRes for 0x%x\n", *stateByte);
    // for(int p = 0; p < 8; p++){
    //     printf("%d = 0x%x\n", p, xtimeRes[p]);
    // }
    counter = 0;
    uint8_t res = 0x00;
    while (counter < 7){    // Why is this 7??
        if (constant & (1 << counter)){
            // printf("was triggered in ffMultiply for counter = %d\n", counter);
            // printf("xtimeRes[counter] = 0x%x\n", xtimeRes[counter]);
            res = res ^ xtimeRes[counter];
        }
        counter++;
    }
    return res;
}

unsigned int SubWord(unsigned int word){
    uint8_t byteOne = (word & 0xff000000) >> 24;
    uint8_t byteTwo = (word & 0xff0000) >> 16;
    uint8_t byteThree = (word & 0xff00) >> 8;
    uint8_t byteFour = (word & 0xff);
    unsigned int res = 0;

    uint8_t temp = Sbox[byteFour >> 4][byteFour & 0x0f];
    res = res | temp;
    temp = Sbox[byteThree >> 4][byteThree & 0x0f];
    res = res | (temp<<8 & 0xff00);
    temp = Sbox[byteTwo >> 4][byteTwo & 0x0f];
    res = res | (temp<<16 & 0xff0000);
    temp = Sbox[byteOne >> 4][byteOne & 0x0f];
    res = res | (temp<<24 & 0xff000000);

    return res;
};

unsigned int RotWord(unsigned int word){
    unsigned int res = word & 0x0000ffffff;
    res = (res << 8) | ((word >> 24) & 0x00000000ff);
    return res;
}

void SubBytes(uint8_t state[4][4]){
    int x = 0;
    int y = 0;
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            state[x][y] = Sbox[state[x][y] >> 4][state[x][y] & 0x0f];
        }
    }
}

void ShiftRows(uint8_t state[4][4]){

    int x = 0;
    for(x = 0; x < 4; x++){
        if(x == 0){
            continue;
        }
        if(x == 1){
            //shift one
            uint8_t temp = state[x][0];
            state[x][0] = state[x][1];
            state[x][1] = state[x][2];
            state[x][2] = state[x][3];
            state[x][3] = temp;
        }
        else if(x == 2){
            //shift two
            uint8_t temp = state[x][0];
            uint8_t temp2 = state[x][1];
            state[x][0] = state[x][2];
            state[x][1] = state[x][3];
            state[x][2] = temp;
            state[x][3] = temp2;
        }
        else if(x == 3){
            //shift three
            uint8_t temp = state[x][0];
            uint8_t temp2 = state[x][1];
            uint8_t temp3 = state[x][2];
            state[x][0] = state[x][3];
            state[x][1] = temp;
            state[x][2] = temp2;
            state[x][3] = temp3;
        }
    }
}

void MixColumns(uint8_t state[4][4]){
    uint8_t tempState[4][4];
    // printf("\n\n\n printing out value to make sure I get the ones i want\n");
    for(int column = 0; column < 4; column++){
        for(int byte = 0; byte < 4; byte++){
            uint8_t newByte = 0x00;
            for(int byteColumn = 0; byteColumn < 4; byteColumn++){
                // printf("state[column][byteColumn]: 0x%x\n", state[byteColumn][column]);
                // printf("mixCol[byte][byteColumn] = 0x%x\n", mixCol[byte][byteColumn]);
                // printf("result of ffMultiply: 0x%x\n", ffMultiply(&state[byteColumn][column], mixCol[byte][byteColumn]));
                // printf("STATE VALUE AFTER: 0x%x\n", state[byteColumn][column]);
                uint8_t res = ffMultiply(&state[byteColumn][column], mixCol[byte][byteColumn]);
                newByte = ffAdd(newByte, res);
            }
            // printf("newByte product: 0x%x\n", newByte);
            tempState[byte][column] = newByte;
        }
    }
    /* 
    printf("printing out contents of state after mixing columns\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", tempState[x][y]);
        }
        printf("\n");
    }   
    */
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            state[x][y] = tempState[x][y];
        }
    }
}

void AddRoundKey(uint8_t state[4][4], unsigned int w[Nb*(Nr+1)], int offset){
    uint8_t tempState[4][4];
    //outside loop is for interations
    for(int i = 0; i < 4; i++){
        // on iteration 0, i take the first unsigned int from w
        unsigned int word = w[i + offset];
        uint8_t byteOne = (word & 0xff000000) >> 24;
        uint8_t byteTwo = (word & 0xff0000) >> 16;
        uint8_t byteThree = (word & 0xff00) >> 8;
        uint8_t byteFour = (word & 0xff);
        uint8_t bytes[4] = {byteOne, byteTwo, byteThree, byteFour};
        // After choping up the word, I am able to ffAdd() the bytes and place them into the the tempState
        for(int x = 0; x < 4; x++){
            for(int y = 0; y < 4; y++){
                // if the iteration (word) is the same column, then ffAdd and place in tempState. bytes[x] should give me the respective "row" with the right byte
                if(i == y){
                    tempState[x][y] = ffAdd(state[x][y], bytes[x]);
                    // printf("state[x][y] = 0x%x, bytes[x] = 0x%x, tempState[x][y] = 0x%x\n", state[x][y], bytes[x], tempState[x][y]);
                }
            }
        }
    }
    // printf("printing values in AddRoundKey inside of tempState:\n");
    for(int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            // printf("tempState[x][y] = 0x%x\n", tempState[x][y]);
            state[x][y] = tempState[x][y];
        }
    }
}

void KeyExpansion(uint8_t key[4*Nk], unsigned int w[Nb*(Nr+1)]){
    unsigned int temp;
    int i = 0;
    while (i < Nk){
        w[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
        i = i+1;
    }
    i = Nk;
    while (i < (Nb * (Nr+1))){
        temp = w[i-1];
        if (i % Nk == 0){
            temp = SubWord(RotWord(temp)) ^ Rcon[i/Nk];
        }
        else if (Nk > 6 && i % Nk == 4){
            temp = SubWord(temp);
        }
        w[i] = w[i-Nk] ^ temp;
        i = i + 1;
    }
}

void cipher(uint8_t in[4*Nb], uint8_t out[4*Nb], unsigned int w[Nb*(Nr+1)]){

    uint8_t state[4][Nb];
    int x = 0;
    int y = 0;
    // Initializes the state from the input.
    for(x = 0; x < 4; x++){
        for (y = 0; y < Nb; y++){
            state[y][x] = in[(x * 4) + y];
        }
    }
    int round = 0;

    if(appC){
        printf("round[ %d].input ", round);
        printState(state);
        printf("\n");
        printf("round[ %d].k_sch ", round);
        printKeys(w, round);
        printf("\n");
    }
    // printf("DEBUGGING: printing initial state thing:\n");
    // for(x = 0; x < 4; x++){
    //     for (y = 0; y < Nb; y++){
    //         printf("0x%x ", state[x][y]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // instead of splicing, just pass in how far along the offset it?
    AddRoundKey(state, w, 0); 
    // printf("DEBUGGING: printing initial state xord with state:\n");
    // for(x = 0; x < 4; x++){
    //     for (y = 0; y < Nb; y++){
    //         printf("0x%x ", state[x][y]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    round = round + 1; 
    while (round < Nr) {
        if(appC){
            printf("round[ %d].start ", round);
            printState(state);
            printf("\n");
        }
        SubBytes(state);    // See Sec. 5.1.1
        if(appC){
            printf("round[ %d].s_box ", round);
            printState(state);
            printf("\n");
        }
        ShiftRows(state);   // See Sec. 5.1.2
        if(appC){
            printf("round[ %d].s_row ", round);
            printState(state);
            printf("\n");
        }
        MixColumns(state);  // See Sec. 5.1.3
        if(appC){
            printf("round[ %d].m_col ", round);
            printState(state);
            printf("\n");
        }
        AddRoundKey(state, w, round*Nb);
        if(appC){
            printf("round[ %d].k_sch ", round);
            printKeys(w, round);
            printf("\n");
        }
        round = round + 1;
        // printf("DEBUGGING: round number: %d\n", round);
        // for(x = 0; x < 4; x++){
        //     for (y = 0; y < Nb; y++){
        //         printf("0x%x ", state[x][y]);
        //     }
        //     printf("\n");
        // }
    }
    if(appC){
        printf("round[%d].start ", round);
        printState(state);
        printf("\n");
    }
    SubBytes(state);
    if(appC){
        printf("round[%d].s_box ", round);
        printState(state);
        printf("\n");
    }
    ShiftRows(state);
    if(appC){
        printf("round[%d].s_row ", round);
        printState(state);
        printf("\n");
    }
    AddRoundKey(state, w, round*Nb);
    if(appC){
        printf("round[%d].k_sch ", round);
        printKeys(w, round);
        printf("\n");
    }
    x = 0;
    y = 0;

    // printf("DEBUGGING: Final round values\n\n");
    // for(x = 0; x < 4; x++){
    //     for (y = 0; y < Nb; y++){
    //         printf("0x%x ", state[x][y]);
    //     }
    //     printf("\n");
    // }

    if(appC){
        printf("round[%d].output ", round);
        printState(state);
        printf("\n");
    }

    // Placing values from the state into the 
    for(x = 0; x < 4; x++){
        for (y = 0; y < Nb; y++){
            out[(x * 4) + y] = state[y][x];
        }
    }
}

void InvShiftRows(uint8_t state[4][4]){
    // Instead of taking the first and placing it last, take the last and place it first
    int x = 0;
    for(x = 0; x < 4; x++){
        if(x == 0){
            continue;
        }
        if(x == 1){
            //shift one
            uint8_t temp3 = state[x][3];
            state[x][3] = state[x][2];
            state[x][2] = state[x][1];
            state[x][1] = state[x][0];
            state[x][0] = temp3;
        }
        else if(x == 2){
            //shift two
            uint8_t temp3 = state[x][3];
            uint8_t temp2 = state[x][2];
            state[x][2] = state[x][0];
            state[x][3] = state[x][1];
            state[x][0] = temp2;
            state[x][1] = temp3;
        }
        else if(x == 3){
            //shift three
            uint8_t temp3 = state[x][3];
            uint8_t temp2 = state[x][2];
            uint8_t temp1 = state[x][1];
            state[x][3] = state[x][0];
            state[x][0] = temp1;
            state[x][1] = temp2;
            state[x][2] = temp3;
        }
    }
}

void InvSubBytes(uint8_t state[4][4]){
    int x = 0;
    int y = 0;
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            state[x][y] = InvSbox[state[x][y] >> 4][state[x][y] & 0x0f];
        }
    }
}

void InvMixColumns(uint8_t state[4][4]){
    uint8_t tempState[4][4];
    for(int column = 0; column < 4; column++){
        for(int byte = 0; byte < 4; byte++){
            uint8_t newByte = 0x00;
            for(int byteColumn = 0; byteColumn < 4; byteColumn++){
                // printf("state[column][byteColumn]: 0x%x\n", state[byteColumn][column]);
                // printf("invMixCol[byte][byteColumn] = 0x%x\n", invMixCol[byte][byteColumn]);
                // printf("result of ffMultiply: 0x%x\n", ffMultiply(&state[byteColumn][column], invMixCol[byte][byteColumn]));
                // printf("STATE VALUE AFTER: 0x%x\n", state[byteColumn][column]);
                uint8_t res = ffMultiply(&state[byteColumn][column], invMixCol[byte][byteColumn]);
                newByte = ffAdd(newByte, res);
            }
            // printf("newByte product: 0x%x\n", newByte);
            tempState[byte][column] = newByte;
        }
    }
    // printf("printing out contents of state after inversing mixed columns\n");
    // for(int x = 0; x < 4; x++){
    //     for(int y = 0; y < 4; y++){
    //         printf("0x%x ", tempState[x][y]);
    //     }
    //     printf("\n");
    // }   

    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            state[x][y] = tempState[x][y];
        }
    }
}

void InvCipher(uint8_t in[4*Nb], uint8_t out[4*Nb], unsigned int w[Nb*(Nr+1)]){
    uint8_t state[4][Nb];
    int x = 0;
    int y = 0;
    // Initializes the state from the input.
    for(x = 0; x < 4; x++){
        for (y = 0; y < Nb; y++){
            state[y][x] = in[(x * 4) + y];
        }
    }

    int counter = 0;
    int round = Nr - 1;

    if(appC){
        printf("round[ %d].iinput ", counter);
        printState(state);
        printf("\n");
        printf("round[ %d].ik_sch ", counter);
        printKeys(w, (round+1));
        printf("\n");
    }

    AddRoundKey(state, w, Nr*Nb); // See Sec. 5.1.4

    // printf("DEBUGGING: printing initial state xord with state:\n");
    // for(x = 0; x < 4; x++){
    //     for (y = 0; y < Nb; y++){
    //         printf("0x%x ", state[x][y]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");


    counter = counter + 1;
    while(round > 0){             //  round = Nr-1 step -1 downto 1 
        if(appC){
            printf("round[ %d].istart ", counter);
            printState(state);
            printf("\n");
        }
        InvShiftRows(state); // See Sec. 5.3.1
        if(appC){
            printf("round[ %d].is_row ", counter);
            printState(state);
            printf("\n");
        }
        InvSubBytes(state); // See Sec. 5.3.2
        if(appC){
            printf("round[ %d].is_box ", counter);
            printState(state);
            printf("\n");
        }
        if(appC){
            printf("round[ %d].ik_sch ", counter);
            printKeys(w, round);
            printf("\n");
        }
        AddRoundKey(state, w, round*Nb);
        if(appC){
            printf("round[ %d].ik_add ", counter);
            printState(state);
            printf("\n");
        }
        InvMixColumns(state); // See Sec. 5.3.3
        round = round - 1;
        counter = counter + 1;
        // printf("DEBUGGING: round number: %d\n", round);
        // for(x = 0; x < 4; x++){
        //     for (y = 0; y < Nb; y++){
        //         printf("0x%x ", state[x][y]);
        //     }
        //     printf("\n");
        // }
    }
    if(appC){
        printf("round[%d].istart ", counter);
        printState(state);
        printf("\n");
    }
    InvShiftRows(state);
    if(appC){
        printf("round[%d].is_row ", counter);
        printState(state);
        printf("\n");
    }
    InvSubBytes(state);
    if(appC){
        printf("round[%d].is_box ", counter);
        printState(state);
        printf("\n");
    }
    if(appC){
        printf("round[%d].ik_sch ", counter);
        printKeys(w, round);
        printf("\n");
    }
    AddRoundKey(state, w, 0);

    // printf("DEBUGGING: Final round values\n\n");
    // for(x = 0; x < 4; x++){
    //     for (y = 0; y < Nb; y++){
    //         printf("0x%x ", state[x][y]);
    //     }
    //     printf("\n");
    // }

    if(appC){
        printf("round[%d].ioutput ", counter);
        printState(state);
        printf("\n");
    }

    for(x = 0; x < 4; x++){
        for (y = 0; y < Nb; y++){
            out[(x * 4) + y] = state[y][x];
        }
    }
}

// My print debug statements for developing functions while following the spec.
void DebugStatementsSpecs(){
    //ffAdd print verification
    printf("ffAdd test: 0x57 ^ 0x83 = 0x%x\n", ffAdd(0x57,0x83));   // 0xd4

    //xtime print verification
    printf("xtime tests:\n xtime(0x57) = 0x%x\n", xtime(0x57));     // 0xae
    printf("xtime(0xae) = 0x%x\n", xtime(0xae));                    // 0x47
    printf("xtime(0x47) = 0x%x\n", xtime(0x47));                    // 0x8e
    printf("xtime(0x8e) = 0x%x\n", xtime(0x8e));                    // 0x07

    //ffMultiply print verification
    uint8_t testByte = 0x57;
    printf("ffMultiply test: ffMultiply(0x57,0x13) = 0x%x\n", ffMultiply(&testByte, 0x13)); // 0xfe

    // USED TO KEY EXPANSION
    printf("SubWord Tests:\n");
    printf("SubWord(0x00102030) = 0x%x == 0x63cab704\n", SubWord(0x00102030));    // 0x63cab704
    printf("SubWord(0x40506070) = 0x%x == 0x0953d051\n", SubWord(0x40506070));    // 0x0953d051
    printf("SubWord(0x8090a0b0) = 0x%x == 0xcd60e0e7\n", SubWord(0x8090a0b0));    // 0xcd60e0e7
    printf("SubWord(0xc0d0e0f0) = 0x%x == 0xba70e18c\n\n", SubWord(0xc0d0e0f0));  // 0xba70e18c

    printf("RotWord Tests:\n");
    printf("RotWord(0x09cf4f3c) = 0x%x == 0xcf4f3c09\n", RotWord(0x09cf4f3c));      // 0xcf4f3c09
    printf("RotWord(0x2a6c7605) = 0x%x == 0x6c76052a\n\n", RotWord(0x2a6c7605));    // 0x6c76052a

    uint8_t key[16] =       { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                            0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint32_t expanded[44] = { 0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c,
                            0xa0fafe17, 0x88542cb1, 0x23a33939, 0x2a6c7605,
                            0xf2c295f2, 0x7a96b943, 0x5935807a, 0x7359f67f,
                            0x3d80477d, 0x4716fe3e, 0x1e237e44, 0x6d7a883b,
                            0xef44a541, 0xa8525b7f, 0xb671253b, 0xdb0bad00,
                            0xd4d1c6f8, 0x7c839d87, 0xcaf2b8bc, 0x11f915bc,
                            0x6d88a37a, 0x110b3efd, 0xdbf98641, 0xca0093fd,
                            0x4e54f70e, 0x5f5fc9f3, 0x84a64fb2, 0x4ea6dc4f,
                            0xead27321, 0xb58dbad2, 0x312bf560, 0x7f8d292f,
                            0xac7766f3, 0x19fadc21, 0x28d12941, 0x575c006e,
                            0xd014f9a8, 0xc9ee2589, 0xe13f0cc8, 0xb6630ca6 };

    uint32_t w[44];
    KeyExpansion(key,w);
    printf("KeyExpansion test:\n");
    int i = 0;    
    int areEqual = 1;
    printf("printing out expanded w from key expansion");
    for(i = 0; i < 44; i++){
        if(i % 4 == 0){
            printf("\n");
        }
        if(w[i] != expanded[i]){
            areEqual = 0;
        }
        printf("0x%x ", w[i]);
    }
    printf("\nKeyExpansion(key,w): w == expanded? %d\n\n", areEqual);

    // USED FOR CIPHER

    uint8_t state[4][4] =  { {0x19,0xa0,0x9a,0xe9},
                         {0x3d,0xf4,0xc6,0xf8},
                         {0xe3,0xe2,0x8d,0x48},
                         {0xbe,0x2b,0x2a,0x08}};

    uint8_t sub[4][4] =    { {0xd4,0xe0,0xb8,0x1e},
                            {0x27,0xbf,0xb4,0x41},
                            {0x11,0x98,0x5d,0x52},
                            {0xae,0xf1,0xe5,0x30}};

    uint8_t shift[4][4] =  { {0xd4, 0xe0, 0xb8, 0x1e},
                            {0xbf, 0xb4, 0x41, 0x27},
                            {0x5d, 0x52, 0x11, 0x98},
                            {0x30, 0xae, 0xf1, 0xe5}};

    uint8_t mix[4][4] =    { {0x04, 0xe0, 0x48, 0x28},
                            {0x66, 0xcb, 0xf8, 0x06},
                            {0x81, 0x19, 0xd3, 0x26},
                            {0xe5, 0x9a, 0x7a, 0x4c}};

    uint8_t round[4][4] =   { {0xa4, 0x68, 0x6b, 0x02},
                            {0x9c, 0x9f, 0x5b, 0x6a},
                            {0x7f, 0x35, 0xea, 0x50},
                            {0xf2, 0x2b, 0x43, 0x49}};

    printf("SubBytes Test:\n");
    SubBytes(state);
    int x = 0;
    int y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", state[x][y]);
            if(state[x][y] != sub[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("SubBytes are equal? %d\n\n", areEqual);
    
    printf("ShiftRows Test:\n");
    ShiftRows(state);
    x = 0;
    y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", state[x][y]);
            if(state[x][y] != shift[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("ShiftBytes are equal? %d\n\n", areEqual);
    /* Test that state == shift */
    
    printf("MixColumns Test:\n");
    MixColumns(state);
    x = 0;
    y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", state[x][y]);
            if(state[x][y] != mix[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("MixColumns are equal? %d\n\n", areEqual);

    /* Test that state == mix */
    // Was this, not passing in 4 anymore
    // AddRoundKey(state,w,4);
    printf("AddRoundKey Test:\n");
    AddRoundKey(state, w, 4);       // passing in 4 as the offset after the first time through.
    x = 0;
    y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", state[x][y]);
            if(state[x][y] != round[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("RoundKey is equal? %d\n\n", areEqual);

    uint8_t in[16]  = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                        0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
    uint8_t out[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t result[16] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
                        0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
    cipher(in, out, w);
    /* Test that out == result */
    areEqual = 1;
    for(int z = 0; z < 16; z++){
        if(out[z] != result[z]){
            areEqual = 0;
        }
    }
    printf("Cipher complete? %d\n\n", areEqual);

    printf("InvCipher Functions\n\n");

    uint8_t stateInv[4][4] =  { {0x19,0xa0,0x9a,0xe9},
                                {0x3d,0xf4,0xc6,0xf8},
                                {0xe3,0xe2,0x8d,0x48},
                                {0xbe,0x2b,0x2a,0x08}};
    uint8_t InvShift[4][4] =  { {0x19,0xa0,0x9a,0xe9},
                                {0xf8,0x3d,0xf4,0xc6},
                                {0x8d,0x48,0xe3,0xe2},
                                {0x2b,0x2a,0x08,0xbe}};
    uint8_t InvSub[4][4] =    { {0x8e,0x47,0x37,0xeb},
                                {0xe1,0x8b,0xba,0xc7},
                                {0xb4,0xd4,0x4d,0x3b},
                                {0x0b,0x95,0xbf,0x5a}};

    printf("InvShiftRows Test:\n");
    InvShiftRows(stateInv);
    x = 0;
    y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", stateInv[x][y]);
            if(stateInv[x][y] != InvShift[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("InvShiftRows are equal? %d\n\n", areEqual);

    printf("InvSubBytes Test:\n");
    InvSubBytes(stateInv);
    x = 0;
    y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", stateInv[x][y]);
            if(stateInv[x][y] != InvSub[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("InvSubBytes are equal? %d\n\n", areEqual);

    //idea, mix the state, and then inverse it, should be the same right?
    printf("InvMixCol Test:\n");
    MixColumns(stateInv);

    printf("printing values in state After mixing columns:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", stateInv[x][y]);
        }
        printf("\n");
    }
    printf("\n");

    InvMixColumns(stateInv);
    x = 0;
    y = 0;
    areEqual = 1;
    printf("printing values in state:\n");
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            printf("0x%x ", stateInv[x][y]);
            if(stateInv[x][y] != InvSub[x][y]){
                areEqual = 0;
                break;
            }
        }
        printf("\n");
    }
    printf("InvMixColumns are equal? %d\n\n", areEqual);

    uint8_t outInv[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t resultInv[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                        0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };

    printf("InvCipher tests:\n");
    
    InvCipher(result, outInv, w);
    /* Test that out == result */
    areEqual = 1;
    for(int z = 0; z < 16; z++){
        if(outInv[z] != resultInv[z]){
            areEqual = 0;
        }
    }
    printf("invCipher complete? %d\n\n", areEqual);
}

// Function used to print out values found in Appendix C
void AppendixC(){

    printf("Appendix C 128\n\n");
    Nk = 4;
    Nr = Nk + 6;

    // Given Key for 128
    // 00010203 04050607 08090a0b 0c0d0e0f
    uint8_t key128[16] =  { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f }; 

    // Given Plain Text
    // 00112233445566778899aabbccddeeff
    uint8_t plainText128[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

    uint8_t outEncrypted128[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t outDecrypted128[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    //128
    uint32_t w128[74];
    KeyExpansion(key128,w128);

    printf("CIPHER 128 (ENCRYPT)\n");
    cipher(plainText128, outEncrypted128, w128);
    printf("\n");

    printf("INVCIPHER 128 (DECRYPT)\n");
    InvCipher(outEncrypted128, outDecrypted128, w128);
    printf("\n\n");

    // 192
    printf("Appendix C 192\n\n");
    Nk = 6;
    Nr = Nk + 6;

    // 00010203 04050607 08090a0b 0c0d0e0f 10111213 14151617
    uint8_t key192[24] =  { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 }; 

    // Given Plain Text
    // 00112233445566778899aabbccddeeff
    uint8_t plainText192[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

    uint8_t outEncrypted192[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t outDecrypted192[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    uint32_t w192[74];

    printf("CIPHER 192 (ENCRYPT)\n");
    KeyExpansion(key192,w192);
    cipher(plainText192, outEncrypted192, w192);
    printf("\n");

    printf("INVCIPHER 192 (DECRYPT)\n");
    InvCipher(outEncrypted192, outDecrypted192, w192);
    printf("\n\n");

    // 256
    printf("Appendix C 256\n\n");
    Nk = 8;
    Nr = Nk + 6;

    // 00010203 04050607 08090a0b 0c0d0e0f 10111213 14151617 18191a1b 1c1d1e1f
    uint8_t key256[32] =  { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f }; 

    // Given Plain Text
    // 00112233445566778899aabbccddeeff
    uint8_t plainText256[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

    uint8_t outEncrypted256[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t outDecrypted256[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    uint32_t w256[74];
    KeyExpansion(key256,w256);

    printf("CIPHER 256 (ENCRYPT)\n");
    cipher(plainText256, outEncrypted256, w256);
    printf("\n");

    printf("INVCIPHER 256 (DECRYPT)\n");
    InvCipher(outEncrypted256, outDecrypted256, w256);
    printf("\n\n");
}

// Function used for inputing values from the auto grader and printing out results.
void AutoGrader(){

    Nk = 4;
    Nr = Nk + 6;
    // Given Key for 128
    // 2a3e591f 3cc7c8d0 4f4afd5c 5d51c578
    uint8_t key128[16]  = { 0x2a, 0x3e, 0x59, 0x1f, 0x3c, 0xc7, 0xc8, 0xd0,
                        0x4f, 0x4a, 0xfd, 0x5c, 0x5d, 0x51, 0xc5, 0x78 };

    // Given Cipher Text
    // 685d03b8 1a806a31 0b49c41a 5847d1d3
    uint8_t cipherText128[16] = { 0x68, 0x5d, 0x03, 0xb8, 0x1a, 0x80, 0x6a, 0x31,
                        0x0b, 0x49, 0xc4, 0x1a, 0x58, 0x47, 0xd1, 0xd3 };

    // Given Plaintext
    // 8bc881d7 cf62294d 0561a1a3 1e170084
    uint8_t plainText128[16] = { 0x8b, 0xc8, 0x81, 0xd7, 0xcf, 0x62, 0x29, 0x4d,
                        0x05, 0x61, 0xa1, 0xa3, 0x1e, 0x17, 0x00, 0x84 };

    // Expanding Key and then Encrypting and Decrypting 128

    uint8_t outEncrypted128[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t outDecrypted128[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // Expanding Key and then Encrypting and Decrypting 128
    uint32_t w128[74];
    KeyExpansion(key128,w128);

    InvCipher(cipherText128, outDecrypted128, w128);
    printf("Printing Output of 128 Encrypted-->Decrypted\n");
    for(int x = 0; x < 16; x++){
        printf("%02x", outDecrypted128[x]);
    }
    printf("\n");

    cipher(plainText128, outEncrypted128, w128);
    printf("Printing Output of 128 Decrypted-->Encrypted\n");
    for(int x = 0; x < 16; x++){
        printf("%02x", outEncrypted128[x]);
    }
    printf("\n\n");

    // 192
    Nk = 6;
    Nr = Nk + 6;

    // Given Key
    // 2a3e591f 3cc7c8d0 4f4afd5c 5d51c578 d4d1c6b5 702565ae
    uint8_t key192[24]  = { 0x2a, 0x3e, 0x59, 0x1f, 0x3c, 0xc7, 0xc8, 0xd0,
                            0x4f, 0x4a, 0xfd, 0x5c, 0x5d, 0x51, 0xc5, 0x78,
                            0xd4, 0xd1, 0xc6, 0xb5, 0x70, 0x25, 0x65, 0xae };

    // Given Cipher Text
    // b6bb855c 7e75d5d7 22d6ffe6 7a120d4b
    uint8_t cipherText192[16] = { 0xb6, 0xbb, 0x85, 0x5c, 0x7e, 0x75, 0xd5, 0xd7,
                            0x22, 0xd6, 0xff, 0xe6, 0x7a, 0x12, 0x0d, 0x4b };

    // Given Plaintext
    // 9dc64cfe de3f5eaa 192cd2e5 9cf5959b
    uint8_t plainText192[16] = { 0x9d, 0xc6, 0x4c, 0xfe, 0xde, 0x3f, 0x5e, 0xaa,
                            0x19, 0x2c, 0xd2, 0xe5, 0x9c, 0xf5, 0x95, 0x9b };

    uint8_t outEncrypted192[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t outDecrypted192[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



    uint32_t w192[74];
    KeyExpansion(key192,w192);

    InvCipher(cipherText192, outDecrypted192, w192);
    printf("Printing Output of 192 Encrypted-->Decrypted\n");
    for(int x = 0; x < 16; x++){
        printf("%02x", outDecrypted192[x]);
    }
    printf("\n");

    cipher(plainText192, outEncrypted192, w192);
    printf("Printing Output of 192 Decrypted-->Encrypted\n");
    for(int x = 0; x < 16; x++){
        printf("%02x", outEncrypted192[x]);
    }
    printf("\n\n");

    // 256
    Nk = 8;
    Nr = Nk + 6;

    // Given Key
    // 2a3e591f 3cc7c8d0 4f4afd5c 5d51c578 d4d1c6b5 702565ae 8636cf7f 0d5010e0
    uint8_t key256[32]  = { 0x2a, 0x3e, 0x59, 0x1f, 0x3c, 0xc7, 0xc8, 0xd0,
                            0x4f, 0x4a, 0xfd, 0x5c, 0x5d, 0x51, 0xc5, 0x78,
                            0xd4, 0xd1, 0xc6, 0xb5, 0x70, 0x25, 0x65, 0xae,
                            0x86, 0x36, 0xcf, 0x7f, 0x0d, 0x50, 0x10, 0xe0 };

    // Given Cipher Text
    // 1465c805 df8ccaa1 0ab4dfc8 79669ede
    uint8_t cipherText256[16] = { 0x14, 0x65, 0xc8, 0x05, 0xdf, 0x8c, 0xca, 0xa1,
                            0x0a, 0xb4, 0xdf, 0xc8, 0x79, 0x66, 0x9e, 0xde };

    // Given Plaintext
    // e664878f d671ab9d ae956338 5b1a64e4
    uint8_t plainText256[16] = { 0xe6, 0x64, 0x87, 0x8f, 0xd6, 0x71, 0xab, 0x9d,
                            0xae, 0x95, 0x63, 0x38, 0x5b, 0x1a, 0x64, 0xe4 };

    uint8_t outEncrypted256[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t outDecrypted256[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



    uint32_t w256[74];
    KeyExpansion(key256,w256);

    InvCipher(cipherText256, outDecrypted256, w256);
    printf("Printing Output of 256 Encrypted-->Decrypted\n");
    for(int x = 0; x < 16; x++){
        printf("%02x", outDecrypted256[x]);
    }
    printf("\n");

    cipher(plainText256, outEncrypted256, w256);
    printf("Printing Output of 256 Decrypted-->Encrypted\n");
    for(int x = 0; x < 16; x++){
        printf("%02x", outEncrypted256[x]);
    }
    printf("\n\n");
}

int main(int argc, char *argv[])  {
    // Default functionality is to run the AppendixC values, include 1 for Auto Grader, 2 for AppendixC without debug statements, 3 for My Debugging.
    appC = 1;

    if(argc == 1) {
        // Default functionality with 128
        Nr = Nk + 6;
        AppendixC();
    }
    else {
        // Branching point, 1-->Input/output of the Autograder, 2-->Appendix C values, 3-->My Debugging values from making the functions.
        if(argv[1][0] == '1'){
            appC = 0;
            AutoGrader();
        }
        else if(argv[1][0] == '2'){
            appC = 0;
            AppendixC();
        }
        else if(argv[1][0] == '3'){
            appC = 0;
            DebugStatementsSpecs();
        }
    }  
}