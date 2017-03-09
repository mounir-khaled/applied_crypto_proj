#define N 3
#define MAX_WORD_LEN 28
#define NUM_WORDS 109582
#define LENPT 500
#define NTESTS 200
#define DEBUGGING 5

/* Freq Analysis fitness function types */
typedef struct ngram_s {
  char ngram[N+1];
  int val;
} ngram_t;

typedef struct score_s {
  int score;
  int avgwlen;
} score_t;

/* Decoding types */
typedef struct key_s {
  int a[19];
} key_t;

/* Global vars */
char letters[28] = "abcdefghijklmnopqrstuvwxyz ";
int n = 0;
score_t gbavgscore;
score_t gblowscore;

/* Function declarations */

// form ngram map of input string
int analyse(char *buf, int len, ngram_t target[]);

// initialise ngrams to default vals
void init_ngrams(ngram_t target[]);

// print the ngram map, for debugging
void print_ngrams(ngram_t target[]);

// check that the init worked, for debugging
void test_init(ngram_t target[]);

// read in the dictionary at 'filename' to the given 2d arr
void read_dict(char *filename, char target[NUM_WORDS][MAX_WORD_LEN+1]);

// generate a random sample plaintext of len LENPT
void gen_sample_pt(char source[NUM_WORDS][MAX_WORD_LEN+1], char *target);

// create a large number of LENPT long plaintexts and map their ngrams
void find_base_score(char source[NUM_WORDS][MAX_WORD_LEN+1], ngram_t target[], score_t score_dest[]);

// score a plaintext
score_t score(char *buf, int len, ngram_t sample[]);

// add one ngram map's vals to another, for aggregating
// void ngram_add(ngram_t source[], ngram_t target[]);
