#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swapPairInPairs(int first_position, int second_position);
int winnerPairVotes(int pair_index);
int loserPairVotes(int pair_index);
bool isCycle(int winner, int loser);
bool isWinner(int candidate_index);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)       // iterate candidates[]
    {
        if (strcmp(candidates[i], name) == 0)       // verifies if cadidate's name exists and if its equal "name"
        {
            for (int j = 0; j < rank; j++)          // iterate rank[]
            {
                if (ranks[j] == i)                  //check for non-repetitions index in rank
                {
                    return false;
                }
            }

            ranks[rank] =  i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)               //iterate over rank for rank's winner and over preferences row
    {
        int candidate_row = ranks[i];

        for (int j = i + 1; j < candidate_count; j++)       //iterate over rank for rank's losers and over preferences row's cols
        {
            int candidate_col = ranks[j];

            preferences[candidate_row][candidate_col] += 1;
        }
    }

    printf("Preferences Matrix:\n");                     //to check preferences matrix

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i", preferences[i][j]);
        }
        printf("\n");
    }

}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)           //iteration over preferences row
    {
        for (int j = 0; j < candidate_count; j++)       //iteration over preferences row's colums
        {
            if (i != j)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    int unsorted_pairs = pair_count;
    int sort_counter = -1;

    while (sort_counter != 0)
    {
        sort_counter = 0;

        for (int i = 0; i < unsorted_pairs; i++)
        {

            int w0_votes = winnerPairVotes(i);          // winner first pair votes
            int w1_votes = winnerPairVotes(i + 1);      // winner second pair votes
            int l0_votes = loserPairVotes(i);           // loser first pair votes
            int l1_votes = loserPairVotes(i + 1);       // loser second pair votes

            if (w0_votes < w1_votes)
            {
                swapPairInPairs(i, i + 1);
                sort_counter += 1;
            }

            if (w0_votes == w1_votes && l0_votes > l1_votes)
            {
                swapPairInPairs(i, i + 1);
                sort_counter += 1;
            }
        }

        unsorted_pairs--;
    }

}

int winnerPairVotes(int pair_index)
{
    int w = pairs[pair_index].winner;       // winner index
    int l = pairs[pair_index].loser;        // loser index

    return preferences[w][l];               // winner pair votes
}

int loserPairVotes(int pair_index)
{
    int w = pairs[pair_index].winner;       // winner index
    int l = pairs[pair_index].loser;        // loser index

    return preferences[l][w];               // loser pair votes
}

void swapPairInPairs(int first_position, int second_position)
{
    pair aux[1];

    aux[0] = pairs[first_position];

    pairs[first_position] = pairs[second_position];

    pairs[second_position] = aux[0];

}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int w = pairs[i].winner;
        int l = pairs[i].loser;

        if (isCycle(w, l) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Print the winner of the election

bool isCycle(int winner, int loser)
{

    if (locked[loser][winner] == true)
    {
        return true;
    }

    for (int i = 0; i < pair_count; i++)
    {
        int w0 = pairs[i].winner;

        if (locked[w0][winner] == true)
        {
            return isCycle(w0, loser);
        }
    }

    return false;
}

void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (isWinner(i))
        {
            printf("%s\n", candidates[i]);
        }
    }
}

bool isWinner(int candidate_index)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][candidate_index] == true)
        {
            return false;
        }
    }

    return true;

}

