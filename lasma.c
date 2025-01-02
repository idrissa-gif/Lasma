#include<stdio.h>
#include<string.h>

#define U64 unsigned long long

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};
enum{ white, black };
const char * square_to_coordinates[] = {
"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

/*********************************\
 ==================================
        Bit manipulations
 ==================================
 \*********************************/
 #define get_bit(bitboard,square) (bitboard & (1ULL << square))
 #define set_bit(bitboard,square) (bitboard |= (1ULL << square))
 #define pop_bit(bitboard,square) (get_bit(bitboard,square) ? bitboard ^= (1ull << square) : 0)

//Count bits within a bitboard
static inline int count_bits(U64 bitboard)
{
    //bit counter
    int count = 0;

    while(bitboard)
    {
        count++;
        bitboard &= bitboard - 1;
    }
    return count;
}

//get least significiant 1 st bit index
static inline int get_ls1b_index(U64 biitboard)
{
    if(biitboard){
        return count_bits((biitboard & -biitboard) -1);
    }
    else
        return -1;
}
/*********************************\
 ==================================
        Input & output
 ==================================
 \*********************************/

void print_bitboard(U64 bitboard)
{
    printf("\n");
    for (int rank = 0 ; rank < 8 ; rank++)
    {
        for(int file = 0 ; file < 8 ; file++)
        {
            // convert file & rank into square index
            int square = rank * 8 + file;
            if (!file)
                printf(" %d ",8-rank);
            printf(" %d", get_bit(bitboard,square) ? 1 : 0);
        }
        printf("\n");
    }

    printf("\n    a b c d e f g h \n\n");

    printf("  Bitboard: %llud\n\n",bitboard);

}

/*********************************\
 ==================================
            Attacks
 ==================================
 \*********************************/
/*
    not_A_file
 8  0 1 1 1 1 1 1 1
 7  0 1 1 1 1 1 1 1
 6  0 1 1 1 1 1 1 1
 5  0 1 1 1 1 1 1 1
 4  0 1 1 1 1 1 1 1
 3  0 1 1 1 1 1 1 1
 2  0 1 1 1 1 1 1 1
 1  0 1 1 1 1 1 1 1

    not_ab_file
 8  0 0 1 1 1 1 1 1
 7  0 0 1 1 1 1 1 1
 6  0 0 1 1 1 1 1 1
 5  0 0 1 1 1 1 1 1
 4  0 0 1 1 1 1 1 1
 3  0 0 1 1 1 1 1 1
 2  0 0 1 1 1 1 1 1
 1  0 0 1 1 1 1 1 1

    a b c d e f g h

    not_h_file
    a b c d e f g h
 8  1 1 1 1 1 1 1 0
 7  1 1 1 1 1 1 1 0
 6  1 1 1 1 1 1 1 0
 5  1 1 1 1 1 1 1 0
 4  1 1 1 1 1 1 1 0
 3  1 1 1 1 1 1 1 0
 2  1 1 1 1 1 1 1 0
 1  1 1 1 1 1 1 1 0

    a b c d e f g h

    not_hg_file
 8  1 1 1 1 1 1 0 0
 7  1 1 1 1 1 1 0 0
 6  1 1 1 1 1 1 0 0
 5  1 1 1 1 1 1 0 0
 4  1 1 1 1 1 1 0 0
 3  1 1 1 1 1 1 0 0
 2  1 1 1 1 1 1 0 0
 1  1 1 1 1 1 1 0 0

    a b c d e f g h


*/
 //Not A file constant
const U64 not_a_file = 18374403900871474942ULL;
const U64 not_ab_file = 18229723555195321596ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;

//Relevancy occupancy bit count for every square on board
const int bishop_relevant_bits[64] ={
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};

const int rook_relevant_bits[64] ={
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};


U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];

//Generate pawn attacks
U64 mask_pawn_attacks(int side, int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    //white pawns
    if (!side){
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if((bitboard>>9) & not_h_file) attacks |= (bitboard >> 9);
    }
    //black pawns
    else{
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if((bitboard<<9) & not_a_file) attacks |= (bitboard << 9);

    }

    // return attack map
    return attacks;

}

//Generate knight attacks
U64 mask_knight_attacks(int square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    if ((bitboard<<17) & not_a_file) attacks |= (bitboard<<17);
    if ((bitboard<<15) & not_h_file) attacks |= (bitboard<<15);
    if ((bitboard<<10) & not_ab_file) attacks |= (bitboard<<10);
    if ((bitboard<<6) & not_hg_file) attacks |= (bitboard<<6);

    return attacks;
}


//Generate king attacks
U64 mask_king_attacks(int square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    if(bitboard >> 8) attacks |= (bitboard >> 8);
    if((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);

    if(bitboard << 8) attacks |= (bitboard << 8);
    if((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    return attacks;
}

// Mask Bishop attacks
U64 mask_bishop_attacks(int square)
{
    U64 attacks = 0ULL;


    int rank, file;

    int tagetrank = square / 8;
    int targetfile = square % 8;

    for (rank= tagetrank + 1 , file = targetfile + 1 ; rank <= 6 && file <= 6 ; rank++, file++) attacks |= (1ULL<<(rank*8 + file));
    for (rank= tagetrank - 1 , file = targetfile + 1 ; rank >= 1 && file <= 6 ; rank--, file++) attacks |= (1ULL<<(rank*8 + file));
    for (rank= tagetrank + 1 , file = targetfile - 1 ; rank <= 6  && file >= 1 ; rank++, file--) attacks |= (1ULL<<(rank*8 + file));
    for (rank= tagetrank - 1 , file = targetfile - 1 ; rank >= 1  && file >= 1 ; rank--, file--) attacks |= (1ULL<<(rank*8 + file));

    return attacks;
}

// Generate Bishop attacks on the fly
U64 mask_bishop_attacks_on_the_fly(int square, U64 block)
{
    U64 attacks = 0ULL;


    int rank, file;

    int tagetrank = square / 8;
    int targetfile = square % 8;

    for (rank= tagetrank + 1 , file = targetfile + 1 ; rank <= 7 && file <= 7 ; rank++, file++)
    {
        attacks |= (1ULL<<(rank*8 + file));
        if((1ULL<<(rank*8 + file)) & block) break;
    }
    for (rank= tagetrank - 1 , file = targetfile + 1 ; rank >= 0 && file <= 7 ; rank--, file++)
    {
        attacks |= (1ULL<<(rank*8 + file));
        if((1ULL<<(rank*8 + file)) & block) break;
    }
    for (rank= tagetrank + 1 , file = targetfile - 1 ; rank <= 7  && file >= 0 ; rank++, file--)
    {
        attacks |= (1ULL<<(rank*8 + file));
        if((1ULL<<(rank*8 + file)) & block) break;
    }
    for (rank= tagetrank - 1 , file = targetfile - 1 ; rank >= 0  && file >= 0 ; rank--, file--)
    {
        attacks |= (1ULL<<(rank*8 + file));
        if(1ULL<<(rank*8 + file)) break;
    }

    return attacks;
}

// Mask Rook attacks
U64 mask_rook_attacks(int square)
{
    U64 attacks = 0ULL;


    int rank, file;

    int tagetrank = square / 8;
    int targetfile = square % 8;

    for(rank= tagetrank + 1; rank <= 6; rank++) attacks |= (1ULL << (rank * 8 + targetfile));
    for(rank= tagetrank - 1; rank >= 1; rank--) attacks |= (1ULL << (rank * 8 + targetfile));
    for(file= targetfile + 1; file <= 6; file++) attacks |= (1ULL << (tagetrank * 8 + file));
    for(file= targetfile - 1; file >= 1; file--) attacks |= (1ULL << (tagetrank * 8 + file));
    return attacks;
}

// Generate Rook attacks on the fly
U64 mask_rook_attacks_on_the_fly(int square, U64 block)
{
    U64 attacks = 0ULL;


    int rank, file;

    int tagetrank = square / 8;
    int targetfile = square % 8;

    for(rank= tagetrank + 1; rank <= 7; rank++)
    {
        attacks |= (1ULL << (rank * 8 + targetfile));
        if((1ULL << (rank * 8 + targetfile) & block)) break;
    }
    for(rank= tagetrank - 1; rank >= 0; rank--)
    {
        attacks |= (1ULL << (rank * 8 + targetfile));
        if((1ULL << (rank * 8 + targetfile)) & block) break;
    }
    for(file= targetfile + 1; file <= 7; file++)
    {
        attacks |= (1ULL << (tagetrank * 8 + file));
        if((1ULL << (tagetrank * 8 + file))& block) break;
    }
    for(file= targetfile - 1; file >= 0; file--)
    {
        attacks |= (1ULL << (tagetrank * 8 + file));
        if((1ULL << (tagetrank * 8 + file)) & block) break;
    }
    return attacks;
}

// set occupancies
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask)
{
    U64 occupancy = 0ULL;

    for (int count = 0 ; count < bits_in_mask ; count++)
    {
        //get Least significant bit index
        int square = get_ls1b_index(attack_mask);

        pop_bit(attack_mask,square);
        //make sure occupancy is on board
        if (index & (1<<count))
        {
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}

void init_leapers_attacks(){
    for (int square = 0 ; square < 64 ; square++)
    {
        //Init Pawn Attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        // Init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        //Init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

/*********************************\
 ==================================
           Random Functions
 ==================================
 \*********************************/
  // Pseudo random number state
 unsigned int random_state = 1804289383;

 //generate 32 bit pseudo legal numbers
 unsigned int get_random_U32_number()
 {
    unsigned int number = random_state;
    //XOR shift algorigthm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    random_state  = number;

    return random_state;
 }

 U64 get_random_U64_number()
 {
    U64 n1, n2, n3, n4;
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;

    return n1 | (n2<<16) | (n3 << 32) | (n4 << 48);
 }

 U64 generate_magic_number()
 {
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
 }

 /*********************************\
 ==================================
           Magics
 ==================================
 \*********************************/
//find appropriate magic number
U64 find_magic_number (int square, int relevant_bits, int bishop)
{
    U64 occcupancies[64];
    U64 attacks[4096];
    U64 used_attacks[4096];

    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    int occupancy_indicies = 1 << relevant_bits;

    for(int index = 0 ; index < occupancy_indicies; index++)
    {
        occcupancies[index] = set_occupancy(index,relevant_bits,attack_mask);
        attacks[index] = bishop ? mask_bishop_attacks_on_the_fly(square,occcupancies[index]) : mask_rook_attacks_on_the_fly(square,occcupancies[index]);
    }
    for(int random_count = 0; random_count < 10000000000 ; random_count++)
    {
        U64 magic_number = generate_magic_number();
        // skip inappropriate magic numbers
        if(count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        memset(used_attacks,0ULL, sizeof(used_attacks));
        // init used attacks

    }
}

/*********************************\
 ==================================
           Main Function
 ==================================
 \*********************************/

int main()
{
    init_leapers_attacks();
    print_bitboard(generate_magic_number());

    return 0;
}